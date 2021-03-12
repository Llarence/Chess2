#include <GL/glut.h>
#include <SOIL/SOIL.h>

void create_texture(){
    int texture = SOIL_load_OGL_texture("pieces.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void render_board(){
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int width = glutGet(GLUT_WINDOW_WIDTH);
            int height = glutGet(GLUT_WINDOW_HEIGHT);

            if((x + y) % 2 == 0){
                glColor3ub(0xF2, 0xB8, 0x85);
            }else{
                glColor3ub(0xB3, 0x65, 0x2E);
            }

            glBegin(GL_POLYGON);
            if(width < height){
                glVertex2f(-3.0 / 4.0 + (x / 4.0), (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width));
                glVertex2f(-3.0 / 4.0 + (x / 4.0), (-1.0 + (y / 4.0)) / (height / (float)width));
                glVertex2f(-1.0 + (x / 4.0), (-1.0 + (y / 4.0)) / (height / (float)width));
                glVertex2f(-1.0 + (x / 4.0), (-3.0 / 4.0 + (y / 4.0)) / (height / (float)width));
            }else{
                glVertex2f((-3.0 / 4.0 + (x / 4.0)) / (width / (float)height), -3.0 / 4.0 + (y / 4.0));
                glVertex2f((-3.0 / 4.0 + (x / 4.0)) / (width / (float)height), -1.0 + (y / 4.0));
                glVertex2f((-1.0 + (x / 4.0)) / (width / (float)height), -1.0 + (y / 4.0));
                glVertex2f((-1.0 + (x / 4.0)) / (width / (float)height), -3.0 / 4.0 + (y / 4.0));
            }
            glEnd();
        }
    }

    glBegin(GL_POLYGON);
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    render_board();
    glFlush();
}

void init_window(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    glutDisplayFunc(render);

    glutMainLoop();
}