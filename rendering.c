#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "fen.c"

const float WHITE_PAWN_X1 = 7/8;
const float WHITE_PAWN_X2 = 1;
const float WHITE_PAWN_Y1 = 0;
const float WHITE_PAWN_Y2 = 1;

int texture;

Game *game;

void create_texture(){
    texture = SOIL_load_OGL_texture("pieces.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glDisable(GL_TEXTURE_2D);
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

            /*glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);

            glBegin(GL_QUADS);
            glTexCoord2f(0.5, 0.5);
            glVertex2f(0.5, 0.5);
            glTexCoord2f(0.5, -0.5);
            glVertex2f(0.5, -0.5);
            glTexCoord2f(-0.5, -0.5);
            glVertex2f(-0.5, -0.5);
            glTexCoord2f(-0.5, 0.5);
            glVertex2f(-0.5, 0.5);*/

            float x1;
            float x2;
            float y1;
            float y2;

            if(width < height){
                x1 = -3.0 / 4.0 + (x / 4.0);
                x2 = -1.0 + (x / 4.0);
                y1 = (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width);
                y2 = (-1.0 + (y / 4.0)) / (height / (float)width);
            }else{
                x1 = (-3.0 / 4.0 + (x / 4.0)) / (width / (float)height);
                x2 = (-1.0 + (x / 4.0)) / (width / (float)height);
                y1 = -3.0 / 4.0 + (y / 4.0);
                y2 = -1.0 + (y / 4.0);
            }

            glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x1, y2);
            glVertex2f(x2, y2);
            glVertex2f(x2, y1);
            glEnd();

            
        }
    }
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    render_game(game);
    glFlush();
}

void init_window(int argc, char** argv){
    init_game(game);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    create_texture();
    glutDisplayFunc(render);

    glutMainLoop();
}