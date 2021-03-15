#ifndef RENDERING
#define RENDERING

#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <pthread.h> 

#include "game.c"
#include "fen.c"
#include "ai.c"

Game game;

GLuint texture;

ALuint pieceMove;

float mouse_x;
float mouse_y;

int holding_selected;
int selected_x;
int selected_y;

int aiMoved = TRUE;

void createSounds(){

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

void renderGame(){
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            Move move;
            move.fromX = selected_x;
            move.fromY = selected_y;
            move.toX = x;
            move.toY = y;
            if(!holding_selected || !isLegal(&game, move)){
                if((x + y) % 2 == 0){
                    glColor3ub(0xF2, 0xB8, 0x85);
                }else{
                    glColor3ub(0xB3, 0x65, 0x2E);
                }
            }else{
                if((x + y) % 2 == 0){
                    glColor3ub(0xE2, 0x5C, 0x43);
                }else{
                    glColor3ub(0xE2, 0x33, 0x17);
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

            if(!holding_selected || selected_x != x || selected_y != y){
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

    if(holding_selected){
        drawPiece(game.board[selected_x][selected_y], mouse_x - halfSizeX, mouse_x + halfSizeX, mouse_y - halfSizeY, mouse_y + halfSizeY);
    }

    glFlush();
}

void mouseMove(int x, int y){
    mouse_x = ((x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0) - 1.0;
    mouse_y = (1 - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0);
    render();
}

void *aiThread(void *args){
    if(tryMove(&game, maxMove(&game, BLACK, 4).move)){
        aiMoved = TRUE;
        alSourcePlay(pieceMove);
    }

    return NULL;
}

void mouseClick(int button, int state, int x, int y){
    mouse_x = ((x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0) - 1.0;
    mouse_y = (1 - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0);

    if(button == GLUT_LEFT_BUTTON && game.turn == WHITE){
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

                    if(x1 < mouse_x && mouse_x < x2 && y1 < mouse_y && mouse_y < y2){
                        if(game.turn == game.board[x][y].color){
                            holding_selected = TRUE;
                            selected_x = x;
                            selected_y = y;
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

                    if(x1 < mouse_x && mouse_x < x2 && y1 < mouse_y && mouse_y < y2){
                        if(x != selected_x || y != selected_y){
                            if(tryMove(&game, (Move){selected_x, selected_y, x, y, QUEEN})){
                                alSourcePlay(pieceMove);
                                pthread_t threadID;
                                pthread_create(&threadID, NULL, aiThread, NULL);
                                aiMoved = FALSE;
                            }
                        }
                    }
                }
            }

            holding_selected = FALSE;
        }
    }
    
    render();
}

void onExit(){
    alutExit();
}

void initSound(){
    int argc = 0;
    char *argv[0];
    alutInit(&argc, argv);

    ALuint buffer;
    buffer = alutCreateBufferFromFile("pieceMove.wav");
    alGenSources(1, &pieceMove);
    alSourcei(pieceMove, AL_BUFFER, buffer);
}

void checkAI(){
    if(aiMoved == TRUE){
        render();
        aiMoved = FALSE;
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
    glutIdleFunc(checkAI);
    atexit(onExit);

    glutMainLoop();
}

#endif