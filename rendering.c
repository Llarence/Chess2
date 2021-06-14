#ifndef RENDERING
#define RENDERING

#include <GL/glut.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <pthread.h> 
#include <time.h>
#include <unistd.h>

#include "game.c"
#include "fen.c"
#include "ai.c"

#define AI_WHITE FALSE
#define AI_BLACK TRUE

Game game;

GLuint texture;

ALuint pieceMove;

float mouseX;
float mouseY;

int holding;
int selectedX;
int selectedY;

int X1 = -1;
int Y1;
int X2 = -1;
int Y2;

int aiMoved = TRUE;
volatile int aiThreadDone = TRUE;

pthread_t threadID;

void createSounds(){
    ALuint buffer;
    buffer = alutCreateBufferFromFile("pieceMove.wav");
    alGenSources(1, &pieceMove);
    alSourcei(pieceMove, AL_BUFFER, buffer);
}

void createTextures(){
    texture = SOIL_load_OGL_texture("pieces.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawPiece(Piece piece, float x1, float x2, float y1, float y2){
    if(piece.type != NONE){
        glColor3ub(0xFF, 0xFF, 0xFF);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);

        float textureX1;
        float textureX2;
        float textureY1;
        float textureY2;

        if(piece.color == WHITE){
            textureY1 = 0.5;
            textureY2 = 1.0;
        }else{
            textureY1 = 0.0;
            textureY2 = 0.5;
        }

        switch (piece.type){
            case PAWN:
                textureX1 = 5.0 / 6.0;
                textureX2 = 1.0;
                break;
            
            case KNIGHT:
                textureX1 = 3.0 / 6.0;
                textureX2 = 4.0 / 6.0;
                break;
            
            case BISHOP:
                textureX1 = 2.0 / 6.0;
                textureX2 = 3.0 / 6.0;
                break;
            
            case ROOK:
                textureX1 = 4.0 / 6.0;
                textureX2 = 5.0 / 6.0;
                break;
            
            case QUEEN:
                textureX1 = 1.0 / 6.0;
                textureX2 = 2.0 / 6.0;
                break;
            
            case KING:
                textureX1 = 0.0;
                textureX2 = 1.0 / 6.0;
                break;
        }
        
        glTexCoord2f(textureX2, textureY2);
        glVertex2f(x2, y2);
        glTexCoord2f(textureX2, textureY1);
        glVertex2f(x2, y1);
        glTexCoord2f(textureX1, textureY1);
        glVertex2f(x1, y1);
        glTexCoord2f(textureX1, textureY2);
        glVertex2f(x1, y2);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }
}

void renderText(){
    glColor3f(0, 0, 0); 
    glRasterPos2f(0, 0);

    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Test");
}

void renderGame(){
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            if(holding && isLegal(&game, (Move){selectedX, selectedY, x, y, QUEEN})){
                if((x + y) % 2 == 1){
                    glColor3ub(0xE2, 0x5C, 0x43);
                }else{
                    glColor3ub(0xE2, 0x43, 0x27);
                }
            }else{
                if((X1 == x && Y1 == y) || (X2 == x && Y2 == y)){
                    if((x + y) % 2 == 1){
                        glColor3ub(0xDE, 0xB5, 0x20);
                    }else{
                        glColor3ub(0xD8, 0xA8, 0x20);
                    }
                }else{
                    if((x + y) % 2 == 1){
                        glColor3ub(0xF2, 0xB8, 0x85);
                    }else{
                        glColor3ub(0xC3, 0x75, 0x3E);
                    }
                }
            }

            float x1;
            float x2;
            float y1;
            float y2;

            if(width < height){
                x1 = -1.0 + (x / 4.0);
                x2 = -3.0 / 4.0 + (x / 4.0);
                y1 = (-1.0 + (y / 4.0)) / (height / (float)width);
                y2 = (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width);
            }else{
                x1 = (-1.0 + (x / 4.0)) / (width / (float)height);
                x2 = (-3.0 / 4.0 + (x / 4.0)) / (width / (float)height);
                y1 = -1.0 + (y / 4.0);
                y2 = -3.0 / 4.0 + (y / 4.0);
            }

            glBegin(GL_QUADS);

            glVertex2f(x1, y1);
            glVertex2f(x1, y2);
            glVertex2f(x2, y2);
            glVertex2f(x2, y1);

            glEnd();

            if(!holding || selectedX != x || selectedY != y){
                drawPiece(game.board[x][y], x1, x2, y1, y2);
            }
        }
    }
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    renderGame();

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    float halfSizeX;
    float halfSizeY;

    if(width < height){
        halfSizeX = (1.0 / 8.0);
        halfSizeY = (1.0 / 8.0) / (height / (float)width);
    }else{
        halfSizeX = (1.0 / 8.0) / (width / (float)height);
        halfSizeY = (1.0 / 8.0);
    }

    if(holding){
        drawPiece(game.board[selectedX][selectedY], mouseX - halfSizeX, mouseX + halfSizeX, mouseY - halfSizeY, mouseY + halfSizeY);
    }

    glFlush();
}

void mouseMove(int x, int y){
    if(aiMoved){
        mouseX = ((x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0) - 1.0;
        mouseY = (1.0 - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0);
        render();
    }
}

void onResize(int x, int y){
    render();
}

void *aiThread(void *args){
    Move move = getAIMove(&game);
    if(tryMove(&game, move)){
        X1 = move.fromX;
        Y1 = move.fromY;
        X2 = move.toX;
        Y2 = move.toY;
        alSourcePlay(pieceMove);
    }

    aiThreadDone = TRUE;
    return NULL;
}

void nextMove(){
    render();
    if(game.turn == WHITE){
        if(AI_WHITE){
            aiMoved = FALSE;
            pthread_create(&threadID, NULL, aiThread, NULL);
        }else{
            aiMoved = TRUE;
        }
    }else{
        if(AI_BLACK){
            aiMoved = FALSE;
            pthread_create(&threadID, NULL, aiThread, NULL);
        }else{
            aiMoved = TRUE;
        }
    }
}

//Allow player to choose promotion
void mouseClick(int button, int state, int x, int y){
    if(aiMoved){
        mouseX = ((x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0) - 1.0;
        mouseY = (1.0 - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0);

        if(button == GLUT_LEFT_BUTTON){
            int width = glutGet(GLUT_WINDOW_WIDTH);
            int height = glutGet(GLUT_WINDOW_HEIGHT);
            
            if(state == GLUT_DOWN) {
                for(int x = 0; x < 8; x++){
                    for(int y = 0; y < 8; y++){
                        float x1;
                        float x2;
                        float y1;
                        float y2;

                        if(width < height){
                            x1 = -1.0 + (x / 4.0);
                            x2 = -3.0 / 4.0 + (x / 4.0);
                            y1 = (-1.0 + (y / 4.0)) / (height / (float)width);
                            y2 = (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width);
                        }else{
                            x1 = (-1.0 + (x / 4.0)) / (width / (float)height);
                            x2 = (-3.0 / 4.0 + (x / 4.0)) / (width / (float)height);
                            y1 = -1.0 + (y / 4.0);
                            y2 = -3.0 / 4.0 + (y / 4.0);
                        }

                        if(x1 < mouseX && mouseX < x2 && y1 < mouseY && mouseY < y2){
                            if(game.turn == game.board[x][y].color){
                                holding = TRUE;
                                selectedX = x;
                                selectedY = y;
                            }
                        }
                    }
                }
            }

            if(state == GLUT_UP) {
                for(int x = 0; x < 8; x++){
                    for(int y = 0; y < 8; y++){
                        float x1;
                        float x2;
                        float y1;
                        float y2;

                        if(width < height){
                            x1 = -1.0 + (x / 4.0);
                            x2 = -3.0 / 4.0 + (x / 4.0);
                            y1 = (-1.0 + (y / 4.0)) / (height / (float)width);
                            y2 = (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width);
                        }else{
                            x1 = (-1.0 + (x / 4.0)) / (width / (float)height);
                            x2 = (-3.0 / 4.0 + (x / 4.0)) / (width / (float)height);
                            y1 = -1.0 + (y / 4.0);
                            y2 = -3.0 / 4.0 + (y / 4.0);
                        }

                        if(x1 < mouseX && mouseX < x2 && y1 < mouseY && mouseY < y2){
                            if(x != selectedX || y != selectedY){
                                if(tryMove(&game, (Move){selectedX, selectedY, x, y, QUEEN})){
                                    X1 = selectedX;
                                    Y1 = selectedY;
                                    X2 = x;
                                    Y2 = y;
                                    alSourcePlay(pieceMove);
                                    nextMove();
                                }
                            }
                        }
                    }
                }

                holding = FALSE;
            }
        }
        
        render();
    }
}

void onExit(){
    alutExit();
}

void initSound(){
    int argc = 0;
    char *argv[0];
    alutInit(&argc, argv);

    createSounds();
}

void loop(){
    usleep(10000);
    render();
    if(aiThreadDone){
        aiThreadDone = FALSE;
        nextMove();
    }
}

void initWindow(){
    initGame(&game);

    initSound();

    int argc = 0;
    char *argv[0];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    createTextures();

    glutDisplayFunc(render);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutIdleFunc(loop);
    atexit(onExit);

    nextMove();

    glutMainLoop();
}

#endif