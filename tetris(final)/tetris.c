#include "raylib.h"

#define GRID_WIDTH 15
#define GRID_HEIGHT 20
#define CELL_SIZE 45

#include "menu.c"

#include <stdio.h>
#include <string.h>

char playerName[16] = "";
int nameLength = 0;

char leaderboardNames[3][5][16];  
int leaderboardScores[3][5];
int currentLevel = 1;            

void LoadLeaderboard() {
    for (int lvl = 0; lvl < 3; lvl++) {
        for (int i = 0; i < 5; i++) {
            strcpy(leaderboardNames[lvl][i], "---");
            leaderboardScores[lvl][i] = 0;
        }

        char filename[32];
        sprintf(filename, "leaderboard_%d.txt", lvl + 1);

        FILE *f = fopen(filename, "r");
        if (f == NULL) continue;
        for (int i = 0; i < 5; i++) {
            char line[64];
            if (fgets(line, sizeof(line), f) != NULL) {
              
                char *lastSpace = strrchr(line, ' ');
                if (lastSpace != NULL) {
                    *lastSpace = '\0'; 
                    strncpy(leaderboardNames[lvl][i], line, 15);
                    leaderboardNames[lvl][i][15] = '\0'; 
                    int val = 0;
                    char *ptr = lastSpace + 1;
                    while (*ptr >= '0' && *ptr <= '9') {
                        val = val * 10 + (*ptr - '0');
                        ptr++;
                    }
                    leaderboardScores[lvl][i] = val;
                }
            } else {
                break;
            }
        }
        fclose(f);
    }
}



void SaveLeaderboard() {

    char filename[32];


    sprintf(filename, "leaderboard_%d.txt", currentLevel + 1);
    FILE *f = fopen(filename, "w");

    if (f == NULL) return;
    for (int i = 0; i < 5; i++) {

        fprintf(f, "%s %d\n", leaderboardNames[currentLevel][i], leaderboardScores[currentLevel][i]);
    }
    fclose(f);

}



void AddScoreToLeaderboard(const char *name, int newScore) {

    if (newScore <= leaderboardScores[currentLevel][4]) return;
    strncpy(leaderboardNames[currentLevel][4], name, 15);
    leaderboardNames[currentLevel][4][15] = '\0';
    leaderboardScores[currentLevel][4] = newScore;
    for (int i = 4; i > 0; i--) {

        if (leaderboardScores[currentLevel][i] > leaderboardScores[currentLevel][i - 1]) {

            int tmpScore = leaderboardScores[currentLevel][i];
            leaderboardScores[currentLevel][i] = leaderboardScores[currentLevel][i - 1];
            leaderboardScores[currentLevel][i - 1] = tmpScore;


            char tmpName[16];
            strcpy(tmpName, leaderboardNames[currentLevel][i]);
            strcpy(leaderboardNames[currentLevel][i], leaderboardNames[currentLevel][i - 1]);
            strcpy(leaderboardNames[currentLevel][i - 1], tmpName);
        } else break;
    }

    SaveLeaderboard();

}



void ResetLeaderboard() {

    for (int i = 0; i < 5; i++) {
         strcpy(leaderboardNames[currentLevel][i], "---");
        leaderboardScores[currentLevel][i] = 0;
    }
    SaveLeaderboard();
}



int board[GRID_HEIGHT][GRID_WIDTH] = {0};
int piecenumber = 0;
int lineclear = 0;
float levelcounter = 1;
int p = 1;

int shapes[7][4][4] = {
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,0,1,1},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0}
    }
};

Color pieceColors[7] = {
    (Color){ 0, 240, 240, 255 },    
    (Color){ 255, 165, 0, 255 },   
    (Color){ 160, 32, 240, 255 },   
    (Color){ 30, 90, 255, 255 },    
    (Color){ 50, 205, 50, 255 },    
    (Color){ 255, 215, 0, 255 },    
    (Color){ 220, 20, 60, 255 },    
};

Sound moveSound;
Sound rotateSound;
Sound clearSound;
Sound Gameover;
Music Gameintro;
bool musicMuted = false;

Texture2D photoMahin;
Texture2D photoSonggram;

bool gameover = false;
bool win = false;
bool paused = false;

bool flashActive = false;
float flashTimer = 0.0f;
int flashRows[4];
int flashRowCount = 0;

int score = 0;
int page = 0;

int currentShapeIndex = 0; 
int nextShapeIndex = 0;
int pieceX = 3;             
int pieceY = 0;             

int currentPiece[4][4] = {0};

#define MAX_POPUPS 20
typedef struct {
    float x, y;
    int value;
    float timer;
    bool active;
} ScorePopup;

ScorePopup popups[MAX_POPUPS];

void AddScorePopup(float x, float y, int value) {
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (!popups[i].active) {
            popups[i].active = true;
            popups[i].x = x;
            popups[i].y = y;
            popups[i].value = value;
            popups[i].timer = 1.0f;
            return;
        }
    }
}

void UpdatePopups(float dt) {
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (popups[i].active) {
            popups[i].timer -= dt;
            popups[i].y -= 40.0f * dt;
            if (popups[i].timer <= 0) popups[i].active = false;
        }
    }
}

void DrawPopups() {
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (popups[i].active) {
            float alpha = popups[i].timer;
            if (alpha > 1.0f) alpha = 1.0f;
            if (alpha < 0.0f) alpha = 0.0f;
            Color c = (Color){ 255, 215, 0, (unsigned char)(255 * alpha) };
            Color shadow = (Color){ 0, 0, 0, (unsigned char)(200 * alpha) };
            char txt[16];
            sprintf(txt, "+%d", popups[i].value);
            DrawText(txt, (int)popups[i].x + 2, (int)popups[i].y + 2, 36, shadow);
            DrawText(txt, (int)popups[i].x, (int)popups[i].y, 36, c);
        }
    }
}

void SetPiece(int shape, int ar[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            ar[r][c] = shapes[shape][r][c];
        }
    }
}

int CheckCollision(int targetX, int targetY, int test[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (test[r][c] == 1) {
                int mapX = targetX + c; 
                int mapY = targetY + r; 

                if (mapX < 0 || mapX >= GRID_WIDTH || mapY >= GRID_HEIGHT) {
                    return 1; 
                }
                
                if (mapY >= 0 && board[mapY][mapX] != 0) {
                    return 1; 
                }
            }
        }
    }
    return 0; 
}

void RotatePiece() {
    int rotated[4][4] = {0};

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            rotated[c][3 - r] = currentPiece[r][c];
        }
    }

    if (!CheckCollision(pieceX, pieceY, rotated)) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                currentPiece[r][c] = rotated[r][c];
            }
        }
        PlaySound(rotateSound); 
    }
}

void LockPiece() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentPiece[r][c] == 1) {
                int mapX = pieceX + c;
                int mapY = pieceY + r;
                if (mapY >= 0 && mapY < GRID_HEIGHT && mapX >= 0 && mapX < GRID_WIDTH) {
                    board[mapY][mapX] = currentShapeIndex + 1; 
                }
            }
        }
    }
}

int DetectFullRows() {
    flashRowCount = 0;
    for (int r = 0; r < GRID_HEIGHT; r++) {
        int Full = 1;
        for (int c = 0; c < GRID_WIDTH; c++) {
            if (board[r][c] == 0) { Full = 0; break; }
        }
        if (Full && flashRowCount < 4) {
            flashRows[flashRowCount] = r;
            flashRowCount++;
        }
    }
    return flashRowCount;
}

void ApplyLineClear() {
    
    for (int i = 0; i < flashRowCount; i++) {
        int r = flashRows[i];
        for (int x = 0; x < GRID_WIDTH; x++) {
            board[r][x] = 0;
        }
    }

   
    int writeRow = GRID_HEIGHT - 1;
    for (int readRow = GRID_HEIGHT - 1; readRow >= 0; readRow--) {
        int isRowFull = 0;
        for (int i = 0; i < flashRowCount; i++) {
            if (flashRows[i] == readRow) {
                isRowFull = 1;
                break;
            }
        }

      
        if (!isRowFull) {
            if (writeRow != readRow) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    board[writeRow][x] = board[readRow][x];
                }
            }
            writeRow--;
        }
    }

   
    while (writeRow >= 0) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            board[writeRow][x] = 0;
        }
        writeRow--;
    }

   
    PlaySound(clearSound);
    int gained = (int)(10 * levelcounter * flashRowCount);
    
   
    if (flashRowCount == 3) gained += 20;
    if (flashRowCount == 4) gained += 30; 

    score += gained;
    lineclear += flashRowCount;

   
    if (flashRowCount > 0) {
        AddScorePopup((float)((GRID_WIDTH * CELL_SIZE) / 2 - 20), (float)(flashRows[0] * CELL_SIZE), gained);
    }

    flashRowCount = 0;
}

int nextpiece[4][4];



void SpawnNewPiece() {
    pieceX = 3;
    pieceY = 0;
    
    if (piecenumber == 0) {
        currentShapeIndex = GetRandomValue(0, 6);
        nextShapeIndex = GetRandomValue(0, 6); 
    } else {
        currentShapeIndex = nextShapeIndex;   
        nextShapeIndex = GetRandomValue(0, 6); 
    }  

    SetPiece(currentShapeIndex, currentPiece);
    SetPiece(nextShapeIndex, nextpiece);

    if (CheckCollision(pieceX, pieceY, currentPiece)) {
        for (int r = 0; r < GRID_HEIGHT; r++) {
            for (int c = 0; c < GRID_WIDTH; c++) {
                board[r][c] = 0;
            }
        }
        gameover = true;
        PlaySound(Gameover); 
        AddScoreToLeaderboard(playerName,score);
    }
    piecenumber++;
}


int main(void) {
    int screenWidth = GRID_WIDTH * CELL_SIZE;
    int screenHeight = GRID_HEIGHT * CELL_SIZE;

    InitWindow(screenWidth + 6 * CELL_SIZE, screenHeight, "Tetris");
    SetTargetFPS(60);
    LoadLeaderboard();

    InitAudioDevice();

    moveSound = LoadSound("assets\\movement.mp3");      
    rotateSound = LoadSound("assets\\rotate.mp3");    
    clearSound = LoadSound("assets\\tetris_line_clear.mp3"); 
    Gameover = LoadSound("assets\\gameover.mp3");
    
    Gameintro = LoadMusicStream("assets\\gameintro.mp3");
    photoMahin = LoadTexture("assets\\mahin.png");
    photoSonggram = LoadTexture("assets\\songgram.png");
    GenTextureMipmaps(&photoMahin);
    GenTextureMipmaps(&photoSonggram);
    SetTextureFilter(photoMahin, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(photoSonggram, TEXTURE_FILTER_TRILINEAR);

    float dropTimer = 0.0;   
    
    Rectangle Startgame = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 190, 240, 48 };
    Rectangle credits   = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 250, 240, 48 };
    Rectangle howtoplayBtn = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 310, 240, 48 };
    Rectangle leaderboardBtn = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 370, 240, 48 };
    Rectangle level     = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 430, 240, 48 };
    Rectangle exitb     = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 490, 240, 48 };

    Rectangle l1 = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 220, 240, 55 };
    Rectangle l2 = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 300, 240, 55 };
    Rectangle l3 = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 120 , 380, 240, 55 };
    Rectangle resetBtn = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 100 , 180 + 5 * 55 + 15, 200, 40 };

    SpawnNewPiece(); 
    
    if(page == 0 || page == 2 || page == 4 || page == 3 || page == 6) {  
        Gameintro.looping = true; 
        PlayMusicStream(Gameintro);
    }
    float x = 0.0f; 
    dropTimer = 0.0f;

    bool shouldExitProgram = false;
    bool resetRequested = false;

    while (!WindowShouldClose() && !shouldExitProgram) {
        if(page == 0 || page == 2 || page == 4 || page == 3 || page == 6) UpdateMusicStream(Gameintro);

        Vector2 mouse = GetMousePosition();

        if (IsKeyPressed(KEY_M)) {
            musicMuted = !musicMuted;
            SetMusicVolume(Gameintro, musicMuted ? 0.0f : 1.0f);
        }

        if (page == 5) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 126) && nameLength < 15) {
                    playerName[nameLength] = (char)key;
                    nameLength++;
                    playerName[nameLength] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && nameLength > 0) {
                nameLength--;
                playerName[nameLength] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER) && nameLength > 0) {
            for (int r = 0; r < GRID_HEIGHT; r++) {
                for (int c = 0; c < GRID_WIDTH; c++) {
                    board[r][c] = 0;
                }
            }
            score = 0;
            gameover = false;
            paused = false;
            piecenumber = 0;
            lineclear = 0;
            SpawnNewPiece();
            page = 1;
            }
        }


        float dropInterval = 0.5 - x;

        
        if (page == 1 && !gameover) {
            if (IsKeyPressed(KEY_P)) {
                paused = !paused;
            }

            if (!paused) {
            float deltaTime = GetFrameTime();

            if (flashActive) {
                flashTimer -= deltaTime;
                if (flashTimer <= 0) {
                    ApplyLineClear();
                    flashActive = false;
                    SpawnNewPiece();
                    dropTimer = 0.0f;
                }
            } else {
                dropTimer += deltaTime;

                if (IsKeyDown(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                    if (!CheckCollision(pieceX - 1, pieceY, currentPiece)) { pieceX -= 1; }
                    PlaySound(moveSound);
                }
                if (IsKeyDown(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                    if (!CheckCollision(pieceX + 1, pieceY, currentPiece)) { pieceX += 1; }
                    PlaySound(moveSound); 
                }
                if (IsKeyDown(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    if (!CheckCollision(pieceX, pieceY + 1, currentPiece)) { pieceY += 1; }
                }

                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    RotatePiece();
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    while (!CheckCollision(pieceX, pieceY + 1, currentPiece)) {
                        pieceY++;
                    }
                    LockPiece();
                    if (DetectFullRows() > 0) {
                        flashActive = true;
                        flashTimer = 0.25f;
                    } else {
                        SpawnNewPiece();
                    }
                    dropTimer = 0.0f;
                }

                if (dropTimer >= dropInterval) {
                    dropTimer = 0.0f;
                    if (!CheckCollision(pieceX, pieceY + 1, currentPiece)) {
                        pieceY++; 
                    } else {
                        LockPiece();
                        if (DetectFullRows() > 0) {
                            flashActive = true;
                            flashTimer = 0.25f;
                        } else {
                            SpawnNewPiece();
                        }
                    }
                }
            }
            UpdatePopups(deltaTime);
        }
    }

        
        if (page == 1 && gameover) {
            if (IsKeyPressed(KEY_R)) {
                for (int r = 0; r < GRID_HEIGHT; r++) {
                    for (int c = 0; c < GRID_WIDTH; c++) {
                        board[r][c] = 0;
                    }
                }
                score = 0;
                gameover = false;
                piecenumber = 0;
                lineclear = 0;
                SpawnNewPiece();
            }
            if (IsKeyPressed(KEY_B)) {
                for (int r = 0; r < GRID_HEIGHT; r++) {
                    for (int c = 0; c < GRID_WIDTH; c++) {
                        board[r][c] = 0;
                    }
                }
                piecenumber = 0;
                score = 0;
                gameover = false;
                page = 0;
                lineclear = 0;
                playerName[0] = '\0';
                nameLength = 0;
            }
        }

        if (page == 1 && paused) {
            if (IsKeyPressed(KEY_B)) {
                for (int r = 0; r < GRID_HEIGHT; r++) {
                    for (int c = 0; c < GRID_WIDTH; c++) {
                        board[r][c] = 0;
                    }
                }
                piecenumber = 0;
                score = 0;
                gameover = false;
                paused = false;
                page = 0;
                lineclear = 0;
            }
        }

        
        if (page == 2 || page == 3 || page == 4 || page == 6) {
        if (IsKeyPressed(KEY_B)) page = 0;
        }

        if(page == 4) {
            if(IsKeyPressed(KEY_P)) page = 1;
        }

        
        BeginDrawing();
            ClearBackground((Color){ 15, 18, 28, 255 }); 

            
            if (page == 0) {
                DrawMenu(mouse, Startgame, credits, howtoplayBtn, leaderboardBtn, level, exitb, screenWidth, screenHeight, &page, &shouldExitProgram);
            }

            if (page == 1) {
                if (!gameover) {
                    for (int r = 0; r < GRID_HEIGHT; r++) {
                        for (int c = 0; c < GRID_WIDTH; c++) {
                            if (board[r][c] != 0) {
                                DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, pieceColors[board[r][c] - 1]);
                            }
                        }
                    }

                    if (flashActive) {
                        for (int i = 0; i < flashRowCount; i++) {
                            int fr = flashRows[i];
                            DrawRectangle(0, fr * CELL_SIZE, GRID_WIDTH * CELL_SIZE, CELL_SIZE - 1, WHITE);
                        }
                    }

                    if (!flashActive) {
                    int ghostY = pieceY;
                    while (!CheckCollision(pieceX, ghostY + 1, currentPiece)) {
                        ghostY++;
                    }
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            if (currentPiece[r][c] == 1) {
                                Color ghostColor = pieceColors[currentShapeIndex];
                                ghostColor.a = 120;
                                DrawRectangleLinesEx(
                                    (Rectangle){ (float)((pieceX + c) * CELL_SIZE), (float)((ghostY + r) * CELL_SIZE), (float)(CELL_SIZE - 1), (float)(CELL_SIZE - 1) },
                                    2, ghostColor
                                );
                            }
                        }
                    }
                    }


                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            if (!flashActive && currentPiece[r][c] == 1) {
                                DrawRectangle((pieceX + c) * CELL_SIZE, (pieceY + r) * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, pieceColors[currentShapeIndex]);
                            }
                            if(nextpiece[r][c] == 1){
                                DrawRectangle(screenWidth + CELL_SIZE + (c * CELL_SIZE), (screenHeight - 5 * CELL_SIZE) + (r * CELL_SIZE), CELL_SIZE - 1, CELL_SIZE - 1, pieceColors[nextShapeIndex]);
                            }
                        }
                    }

                    bool keepplaying = false;
                    if(p == 0) keepplaying = true;
                     
                    if (score >= 300 && !keepplaying) {
                        DrawRectangle(0, 0, screenWidth + 6 * CELL_SIZE, screenHeight, (Color){ 0, 0, 0, 200 });
                        int w1 = MeasureText("Congratulations you won the game", 28);
                        int w2 = MeasureText("Press R to Start a new game", 22);
                        int w3 = MeasureText("Press B to return to main menu", 22);
                        int w4 = MeasureText("Press X to Continue Playing", 22);
                        DrawText("Congratulations you won the game", (screenWidth + 6 * CELL_SIZE)/2 - w1/2, screenHeight/2 - 80, 28, GOLD);
                        DrawText("Press R to Start a new game", (screenWidth + 6 * CELL_SIZE)/2 - w2/2, screenHeight/2 - 20, 22, RAYWHITE);
                        DrawText("Press B to return to main menu", (screenWidth + 6 * CELL_SIZE)/2 - w3/2, screenHeight/2 + 20, 22, RAYWHITE);
                        DrawText("Press X to Continue Playing", (screenWidth + 6 * CELL_SIZE)/2 - w4/2, screenHeight/2 + 60, 22, GREEN);

                        if (IsKeyPressed(KEY_R)) {
                            for (int r = 0; r < GRID_HEIGHT; r++) {
                                for (int c = 0; c < GRID_WIDTH; c++) {
                                    board[r][c] = 0;
                                }
                            }
                            score = 0;
                            gameover = false;
                            piecenumber = 0;
                            lineclear = 0;
                            SpawnNewPiece();
                        }
                        if (IsKeyPressed(KEY_B)) {
                            for (int r = 0; r < GRID_HEIGHT; r++) {
                                for (int c = 0; c < GRID_WIDTH; c++) {
                                    board[r][c] = 0;
                                }
                            }
                            piecenumber = 0;
                            score = 0;
                            gameover = false;
                            page = 0;
                            lineclear = 0;
                        }
                        if (IsKeyPressed(KEY_X)) {
                            p = 0;
                        }
                    }

                    DrawRectangleLinesEx((Rectangle){ (float)screenWidth, 0, 6*CELL_SIZE, (float)screenHeight }, 2, GOLD);
                    DrawRectangleLinesEx((Rectangle){ (float)screenWidth, (float)(screenHeight - 6 * CELL_SIZE), 6*CELL_SIZE, 6*CELL_SIZE }, 2, GOLD);

                    DrawText(TextFormat("Score :- %d", score), 10, 10, 22, GOLD);
                    DrawText(TextFormat("Lines :- %d", lineclear), screenWidth + 15, 20, 22, SKYBLUE);
                    DrawText(TextFormat("Pieces :- %d", piecenumber), screenWidth + 15, 50, 22, SKYBLUE);
                    DrawPopups();
                    DrawText("SPACE: Hard Drop", screenWidth + 15, 90, 16, GRAY);
                    DrawText(TextFormat("PLAYER NAME : %s",playerName),screenWidth + 15, 120, 16, PINK);

                    if (paused) {
                        DrawRectangle(0, 0, screenWidth + 6 * CELL_SIZE, screenHeight, (Color){ 0, 0, 0, 180 });
                        int ps1 = MeasureText("PAUSED", 44);
                        int ps2 = MeasureText("Press P to Resume", 22);
                        int ps3 = MeasureText("Press B to return to Main Menu", 20);
                        DrawText("PAUSED", (screenWidth + 6 * CELL_SIZE)/2 - ps1/2, screenHeight/2 - 60, 44, GOLD);
                        DrawText("Press P to Resume", (screenWidth + 6 * CELL_SIZE)/2 - ps2/2, screenHeight/2 + 10, 22, GREEN);
                        DrawText("Press B to return to Main Menu", (screenWidth + 6 * CELL_SIZE)/2 - ps3/2, screenHeight/2 + 45, 20, LIGHTGRAY);
                    }

                } else {
                    int go1 = MeasureText("GAME OVER", 40);
                    int go2 = MeasureText(TextFormat("Final Score: - %d", score), 28);
                    int go3 = MeasureText("Press R to Restart", 22);
                    int go4 = MeasureText("Press B to return to main menu", 22);
                    DrawText("GAME OVER", (screenWidth + 6 * CELL_SIZE)/2 - go1/2, screenHeight/2 - 120, 40, RED);
                    DrawText(TextFormat("Final Score: - %d", score), (screenWidth + 6 * CELL_SIZE)/2 - go2/2, screenHeight/2 - 40, 28, GOLD);
                    DrawText("Press R to Restart", (screenWidth + 6 * CELL_SIZE)/2 - go3/2, screenHeight/2 + 20, 22, GREEN);
                    DrawText("Press B to return to main menu", (screenWidth + 6 * CELL_SIZE)/2 - go4/2, screenHeight/2 + 60, 22, LIGHTGRAY);
                }
            }

            if (page == 2) {
                DrawCredits(photoMahin, photoSonggram, screenWidth, screenHeight);
            }

            if (page == 6) {
                DrawHowToPlay(screenWidth, screenHeight);
            }

            if (page == 3) {
                DrawLeaderboard(mouse, leaderboardNames[currentLevel], leaderboardScores[currentLevel], resetBtn, currentLevel, screenWidth, screenHeight, &resetRequested);
                if (resetRequested) {
                    ResetLeaderboard();
                    resetRequested = false;
                }
            }

            if (page == 4) {
                DrawLevels(mouse, l1, l2, l3, screenWidth, screenHeight, &x, &levelcounter, &currentLevel, &page);
            }

            if (page == 5) {
                DrawNameEntry(playerName, screenWidth, screenHeight);
                
            }
            DrawText(musicMuted ? "MUSIC: OFF (M)" : "MUSIC: ON (M)", 10, screenHeight - 25, 16, GRAY);
        EndDrawing();
    }

    UnloadSound(moveSound);
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadSound(Gameover);
    UnloadMusicStream(Gameintro);
    UnloadTexture(photoMahin);
    UnloadTexture(photoSonggram);
    
    CloseAudioDevice();
    CloseWindow(); 
    return 0;
}