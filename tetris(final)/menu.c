#include "raylib.h"
#include <stdio.h>
#include<string.h>



#ifndef CELL_SIZE
#define CELL_SIZE 10
#endif


void DrawMenu(Vector2 mouse, Rectangle Startgame, Rectangle credits, Rectangle howtoplayBtn, Rectangle leaderboardBtn, Rectangle level, Rectangle exitb, int screenWidth, int screenHeight, int *page, bool *shouldExit) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, Startgame)) *page = 5;
        if (CheckCollisionPointRec(mouse, credits))   *page = 2;
        if (CheckCollisionPointRec(mouse, howtoplayBtn)) *page = 6;
        if (CheckCollisionPointRec(mouse, leaderboardBtn)) *page = 3;
        if (CheckCollisionPointRec(mouse, level))     *page = 4;
        if (CheckCollisionPointRec(mouse, exitb))     *shouldExit = true;
    }

    int titleWidth = MeasureText("TETRIS", 55);
    DrawText("TETRIS", (screenWidth + 6 * CELL_SIZE)/2 - titleWidth/2 , 90, 55, GOLD);


    bool hStart = CheckCollisionPointRec(mouse, Startgame);
    Rectangle rStart = hStart ? (Rectangle){ Startgame.x - 6, Startgame.y - 3, Startgame.width + 12, Startgame.height + 6 } : Startgame;
    Color colStart = hStart ? (Color){ 255, 87, 100, 255 } : (Color){ 230, 57, 70, 255 };
    DrawRectangleRounded(rStart, 0.4f, 8, colStart);
    DrawRectangleRoundedLinesEx(rStart, 0.4f, 8, hStart ? 3.0f : 2.0f, WHITE);
    int t1 = MeasureText("START GAME", hStart ? 22 : 20);
    DrawText("START GAME", rStart.x + rStart.width/2 - t1/2, rStart.y + (rStart.height - (hStart ? 22 : 20))/2, hStart ? 22 : 20, RAYWHITE);

    // CREDITS Button
    bool hCredits = CheckCollisionPointRec(mouse, credits);
    Rectangle rCredits = hCredits ? (Rectangle){ credits.x - 6, credits.y - 3, credits.width + 12, credits.height + 6 } : credits;
    Color colCredits = hCredits ? (Color){ 66, 226, 212, 255 } : (Color){ 46, 196, 182, 255 };
    DrawRectangleRounded(rCredits, 0.4f, 8, colCredits);
    DrawRectangleRoundedLinesEx(rCredits, 0.4f, 8, hCredits ? 3.0f : 2.0f, WHITE);
    int t2 = MeasureText("CREDITS", hCredits ? 22 : 20);
    DrawText("CREDITS", rCredits.x + rCredits.width/2 - t2/2, rCredits.y + (rCredits.height - (hCredits ? 22 : 20))/2, hCredits ? 22 : 20, RAYWHITE);

    // HOW TO PLAY Button
    bool hHow = CheckCollisionPointRec(mouse, howtoplayBtn);
    Rectangle rHow = hHow ? (Rectangle){ howtoplayBtn.x - 6, howtoplayBtn.y - 3, howtoplayBtn.width + 12, howtoplayBtn.height + 6 } : howtoplayBtn;
    Color colHow = hHow ? (Color){ 100, 180, 255, 255 } : (Color){ 70, 150, 230, 255 };
    DrawRectangleRounded(rHow, 0.4f, 8, colHow);
    DrawRectangleRoundedLinesEx(rHow, 0.4f, 8, hHow ? 3.0f : 2.0f, WHITE);
    int t6 = MeasureText("HOW TO PLAY", hHow ? 22 : 20);
    DrawText("HOW TO PLAY", rHow.x + rHow.width/2 - t6/2, rHow.y + (rHow.height - (hHow ? 22 : 20))/2, hHow ? 22 : 20, RAYWHITE);

    // LEADERBOARD Button
    bool hLead = CheckCollisionPointRec(mouse, leaderboardBtn);
    Rectangle rLead = hLead ? (Rectangle){ leaderboardBtn.x - 6, leaderboardBtn.y - 3, leaderboardBtn.width + 12, leaderboardBtn.height + 6 } : leaderboardBtn;
    Color colLead = hLead ? (Color){ 189, 147, 249, 255 } : (Color){ 155, 109, 255, 255 };
    DrawRectangleRounded(rLead, 0.4f, 8, colLead);
    DrawRectangleRoundedLinesEx(rLead, 0.4f, 8, hLead ? 3.0f : 2.0f, WHITE);
    int t5 = MeasureText("LEADERBOARD", hLead ? 20 : 18);
    DrawText("LEADERBOARD", rLead.x + rLead.width/2 - t5/2, rLead.y + (rLead.height - (hLead ? 20 : 18))/2, hLead ? 20 : 18, RAYWHITE);

    // LEVELS Button
    bool hLevel = CheckCollisionPointRec(mouse, level);
    Rectangle rLevel = hLevel ? (Rectangle){ level.x - 6, level.y - 3, level.width + 12, level.height + 6 } : level;
    Color colLevel = hLevel ? (Color){ 255, 189, 58, 255 } : (Color){ 255, 159, 28, 255 };
    DrawRectangleRounded(rLevel, 0.4f, 8, colLevel);
    DrawRectangleRoundedLinesEx(rLevel, 0.4f, 8, hLevel ? 3.0f : 2.0f, WHITE);
    int t3 = MeasureText("LEVELS", hLevel ? 22 : 20);
    DrawText("LEVELS", rLevel.x + rLevel.width/2 - t3/2, rLevel.y + (rLevel.height - (hLevel ? 22 : 20))/2, hLevel ? 22 : 20, RAYWHITE);

    // EXIT Button
    bool hExit = CheckCollisionPointRec(mouse, exitb);
    Rectangle rExit = hExit ? (Rectangle){ exitb.x - 6, exitb.y - 3, exitb.width + 12, exitb.height + 6 } : exitb;
    Color colExit = hExit ? WHITE : (Color){ 241, 250, 238, 255 };
    DrawRectangleRounded(rExit, 0.4f, 8, colExit);
    DrawRectangleRoundedLinesEx(rExit, 0.4f, 8, hExit ? 3.0f : 2.0f, hExit ? GOLD : LIGHTGRAY);
    int t4 = MeasureText("EXIT", hExit ? 22 : 20);
    DrawText("EXIT", rExit.x + rExit.width/2 - t4/2, rExit.y + (rExit.height - (hExit ? 22 : 20))/2, hExit ? 22 : 20, (Color){ 29, 53, 87, 255 });
}

// Credits Page (Page 2)
void DrawCredits(Texture2D photo1, Texture2D photo2, int screenWidth, int screenHeight) {
    int c1 = MeasureText("TETRIS GAME", 40);
    DrawText("TETRIS GAME", (screenWidth + 6 * CELL_SIZE)/2 - c1/2, 60, 40, GREEN);

    int centerX = (screenWidth + 6 * CELL_SIZE) / 2;
    int photoSize = 300;
    int gap = 60;

    // Person 1 
    int x1 = centerX - gap/2 - photoSize;
    Rectangle dest1 = { (float)x1, 150, (float)photoSize, (float)photoSize };
    Rectangle src1 = { 0, 0, (float)photo1.width, (float)photo1.height };
    DrawTexturePro(photo1, src1, dest1, (Vector2){0,0}, 0.0f, WHITE);
    DrawRectangleLinesEx(dest1, 10, GOLD);

    const char *name1 = "Mahin Alam";
    const char *id1   = "ID: 2505021";   
    int nw1 = MeasureText(name1, 22);
    int iw1 = MeasureText(id1, 18);
    DrawText(name1, x1 + photoSize/2 - nw1/2, 150 + photoSize + 12, 22, RAYWHITE);
    DrawText(id1,   x1 + photoSize/2 - iw1/2, 150 + photoSize + 40, 18, SKYBLUE);

    //Person 2
    int x2 = centerX + gap/2;
    Rectangle dest2 = { (float)x2, 150, (float)photoSize, (float)photoSize };
    Rectangle src2 = { 0, 0, (float)photo2.width, (float)photo2.height };
    DrawTexturePro(photo2, src2, dest2, (Vector2){0,0}, 0.0f, WHITE);
    DrawRectangleLinesEx(dest2, 10, GOLD);

    const char *name2 = "Songgram Ghosh";
    const char *id2   = "ID: 2505017";   
    int nw2 = MeasureText(name2, 22);
    int iw2 = MeasureText(id2, 18);
    DrawText(name2, x2 + photoSize/2 - nw2/2, 150 + photoSize + 12, 22, RAYWHITE);
    DrawText(id2,   x2 + photoSize/2 - iw2/2, 150 + photoSize + 40, 18, SKYBLUE);

    int c3 = MeasureText("Press B to return to Main Menu", 20);
    DrawText("Press B to return to Main Menu", centerX - c3/2, 150 + photoSize + 90, 20, GRAY);
}

// Levels Page (Page 4)
void DrawLevels(Vector2 mouse, Rectangle l1, Rectangle l2, Rectangle l3, int screenWidth, int screenHeight, float *x, float *levelcounter, int *currentLevel, int *page) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, l1)) { *x = -0.25f; *levelcounter = 0.5f; *currentLevel = 0; *page = 5; }
        if (CheckCollisionPointRec(mouse, l2)) { *x = 0.0f;   *levelcounter = 1.0f; *currentLevel = 1; *page = 5; }
        if (CheckCollisionPointRec(mouse, l3)) { *x = 0.25f;  *levelcounter = 1.5f; *currentLevel = 2; *page = 5; }
    }

    if (IsKeyPressed(KEY_P)) {
        *page = 5; 
    }

    int lvlTitle = MeasureText("Difficulty", 45);
    DrawText("Difficulty", (screenWidth + 6 * CELL_SIZE)/2 - lvlTitle/2, 120, 45, GOLD);

    // EASY
    bool hl1 = CheckCollisionPointRec(mouse, l1);
    Rectangle rl1 = hl1 ? (Rectangle){ l1.x - 6, l1.y - 3, l1.width + 12, l1.height + 6 } : l1;
    DrawRectangleRounded(rl1, 0.4f, 8, hl1 ? (Color){ 100, 220, 130, 255 } : (Color){ 70, 190, 100, 255 });
    DrawRectangleRoundedLinesEx(rl1, 0.4f, 8, hl1 ? 3.0f : 2.0f, WHITE);
    int lt1 = MeasureText("EASY", hl1 ? 26 : 24);
    DrawText("EASY", rl1.x + rl1.width/2 - lt1/2, rl1.y + (rl1.height - (hl1 ? 26 : 24))/2, hl1 ? 26 : 24, RAYWHITE);

    // MEDIUM
    bool hl2 = CheckCollisionPointRec(mouse, l2);
    Rectangle rl2 = hl2 ? (Rectangle){ l2.x - 6, l2.y - 3, l2.width + 12, l2.height + 6 } : l2;
    DrawRectangleRounded(rl2, 0.4f, 8, hl2 ? (Color){ 255, 189, 58, 255 } : (Color){ 255, 159, 28, 255 });
    DrawRectangleRoundedLinesEx(rl2, 0.4f, 8, hl2 ? 3.0f : 2.0f, WHITE);
    int lt2 = MeasureText("MEDIUM", hl2 ? 26 : 24);
    DrawText("MEDIUM", rl2.x + rl2.width/2 - lt2/2, rl2.y + (rl2.height - (hl2 ? 26 : 24))/2, hl2 ? 26 : 24, RAYWHITE);

    // HARD
    bool hl3 = CheckCollisionPointRec(mouse, l3);
    Rectangle rl3 = hl3 ? (Rectangle){ l3.x - 6, l3.y - 3, l3.width + 12, l3.height + 6 } : l3;
    DrawRectangleRounded(rl3, 0.4f, 8, hl3 ? (Color){ 255, 87, 100, 255 } : (Color){ 230, 57, 70, 255 });
    DrawRectangleRoundedLinesEx(rl3, 0.4f, 8, hl3 ? 3.0f : 2.0f, WHITE);
    int lt3 = MeasureText("HARD", hl3 ? 26 : 24);
    DrawText("HARD", rl3.x + rl3.width/2 - lt3/2, rl3.y + (rl3.height - (hl3 ? 26 : 24))/2, hl3 ? 26 : 24, RAYWHITE);

    int nav1 = MeasureText("Press B to return to Main Menu", 20);
    int nav2 = MeasureText("Press P to start the game", 20);
    DrawText("Press B to return to Main Menu", (screenWidth + 6 * CELL_SIZE)/2 - nav1/2, 500, 20, GRAY);
    DrawText("Press P to start the game", (screenWidth + 6 * CELL_SIZE)/2 - nav2/2, 540, 20, GREEN);
}
// Name Entry Page (Page 5)
void DrawNameEntry(char *name, int screenWidth, int screenHeight) {
    int t1 = MeasureText("Enter Your Name", 40);
    DrawText("Enter Your Name", (screenWidth + 6 * CELL_SIZE)/2 - t1/2, 180, 40, GOLD);

    Rectangle box = { (float)(screenWidth + 6 * CELL_SIZE)/2 - 150, 260, 300, 50 };
    DrawRectangleRounded(box, 0.3f, 8, (Color){ 30, 30, 40, 255 });
    DrawRectangleRoundedLinesEx(box, 0.3f, 8, 2.0f, GOLD);

    DrawText(name, box.x + 15, box.y + 12, 26, RAYWHITE);

    if (((int)(GetTime() * 2)) % 2 == 0) {
        int w = MeasureText(name, 26);
        DrawText("_", box.x + 15 + w, box.y + 12, 26, RAYWHITE);
    }

    int t2 = MeasureText("Press ENTER to continue", 20);
    DrawText("Press ENTER to continue", (screenWidth + 6 * CELL_SIZE)/2 - t2/2, 340, 20, GRAY);
}

// Leaderboard Page (Page 3)
void DrawLeaderboard(Vector2 mouse, char names[5][16], int scores[5], Rectangle resetBtn, int currentLevel, int screenWidth, int screenHeight, bool *resetRequested) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, resetBtn)) *resetRequested = true;
    }

    const char *levelNames[3] = { "EASY", "MEDIUM", "HARD" };
    char title[40];
    sprintf(title, "LEADERBOARD - %s", levelNames[currentLevel]);
    int t1 = MeasureText(title, 36);
    DrawText(title, (screenWidth + 6 * CELL_SIZE)/2 - t1/2, 100, 36, GOLD);

    for (int i = 0; i < 5; i++) {
        char line[64];
        sprintf(line, "%d. %s - %d", i + 1, names[i], scores[i]);
        int w = MeasureText(line, 26);
        DrawText(line, (screenWidth + 6 * CELL_SIZE)/2 - w/2, 180 + i * 55, 26, RAYWHITE);
    }

    // RESET Button
    bool hReset = CheckCollisionPointRec(mouse, resetBtn);
    Rectangle rReset = hReset ? (Rectangle){ resetBtn.x - 6, resetBtn.y - 3, resetBtn.width + 12, resetBtn.height + 6 } : resetBtn;
    Color colReset = hReset ? (Color){ 255, 87, 100, 255 } : (Color){ 200, 50, 60, 255 };
    DrawRectangleRounded(rReset, 0.4f, 8, colReset);
    DrawRectangleRoundedLinesEx(rReset, 0.4f, 8, hReset ? 3.0f : 2.0f, WHITE);
    int tr = MeasureText("RESET", hReset ? 22 : 20);
    DrawText("RESET", rReset.x + rReset.width/2 - tr/2, rReset.y + (rReset.height - (hReset ? 22 : 20))/2, hReset ? 22 : 20, RAYWHITE);

    int t2 = MeasureText("Press B to return to Main Menu", 20);
    DrawText("Press B to return to Main Menu", (screenWidth + 6 * CELL_SIZE)/2 - t2/2, 180 + 5 * 55 + 65, 20, GRAY);
}
// How to Play Page (Page 6)
void DrawHowToPlay(int screenWidth, int screenHeight) {
    int t1 = MeasureText("HOW TO PLAY", 40);
    DrawText("HOW TO PLAY", (screenWidth + 6 * CELL_SIZE)/2 - t1/2, 60, 40, GOLD);

    const char *lines[] = {
        "LEFT / A       -  Move Left",
        "RIGHT / D      -  Move Right",
        "DOWN / S       -  Soft Drop",
        "UP / W         -  Rotate Piece",
        "SPACE          -  Hard Drop",
        "P              -  Pause / Resume",
        "      R        -  Restart (after Game Over)",
        "    B          -  Return to Main Menu",
    };
    int lineCount = 8;

    int startY = 140;
    for (int i = 0; i < lineCount; i++) {
        int w = MeasureText(lines[i], 22);
        DrawText(lines[i], (screenWidth + 6 * CELL_SIZE)/2 - w/2, startY + i * 38, 22, RAYWHITE);
    }

    int t2 = MeasureText("Press B to return to Main Menu", 20);
    DrawText("Press B to return to Main Menu", (screenWidth + 6 * CELL_SIZE)/2 - t2/2, startY + lineCount * 38 + 30, 20, GRAY);
}