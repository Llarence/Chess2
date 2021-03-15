#include <stdio.h>
#include <AL/al.h>
#include <AL/alut.h>

#include "rendering.c"

int main(int argc, char** argv){
    initWindow();
    Game game;

    initGame(&game);
    tryMove(&game, maxMove(&game, BLACK, 2).move);

    return 0;
}