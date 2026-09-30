To Compile the program :-
 gcc main.c -o tetris.exe -lraylib -lopengl32 -lgdi32 -lwinmm
To Run the program :-
  ./tetris


The game recquired Raylib header files and many of the user defined functions  that was already  built in  the file "raylib.h","raymath.h> etc

The game's screenwidth and screenheight was set considering the specs of my Laptop  which had 1920 x 1200 resolution
the game may go out of the screen or may not function properly if it is compiled and played in a lower resolution laptop .


"" GRID_WIDTH, GRID_HEIGHT,CELL_SIZE "" - Changing the values(lower) of these three macros might help in this problem.