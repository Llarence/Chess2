#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "fen.c"

int texture;

Game game;

void create_texture(){
    texture = SOIL_load_OGL_texture("pieces.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

void draw_piece(Piece piece, float x1, float x2, float y1, float y2){
    if(piece.type != NONE){
        glColor3ub(0xFF, 0xFF, 0xFF);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, texture);

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

void render_game(Game *game){
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int width = glutGet(GLUT_WINDOW_WIDTH);
            int height = glutGet(GLUT_WINDOW_HEIGHT);

            if((x + y) % 2 == 0){
                glColor3ub(0xF2, 0xB8, 0x85);
            }else{
                glColor3ub(0xB3, 0x65, 0x2E);
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

            if((x + y) % 2 == 0){
                glColor3ub(0xF2, 0xB8, 0x85);
            }else{
                glColor3ub(0xB3, 0x65, 0x2E);
            }

            glBegin(GL_QUADS);

            glVertex2f(x1, y1);
            glVertex2f(x1, y2);
            glVertex2f(x2, y2);
            glVertex2f(x2, y1);

            glEnd();

            draw_piece(game->board[x][y], x1, x2, y1, y2);
        }
    }
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    render_game(&game);
    glFlush();
}

void init_window(int argc, char** argv){
    init_game(&game);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    create_texture();
    glutDisplayFunc(render);

    glutMainLoop();
}