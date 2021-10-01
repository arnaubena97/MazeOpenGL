/********************************************************
 *
 * Authors: Arnau Benavides, Marc Felip
 *
*********************************************************/

//-----------------------------------------------
//           INCLUDES DEPENDS OS
//-----------------------------------------------

#if  __linux__ 
    #include <GL/glut.h>
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#endif

#include "maze.h"
#include "square.h"

//-----------------------------------------------
//              GLOBAL VARIABLES
//-----------------------------------------------

#define MED_COLUMNS 4
#define MED_ROWS 4
#define SIZE_SQUARE_SMALL 2 // quant mes petit
#define WIDTH 500
#define HEIGHT 500

int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H = (float)HEIGHT/(float)ROWS;

Maze maze(MED_COLUMNS, MED_ROWS);
Walls wall(maze.getNumWalls());
//Square wall;
Square agent1;
Square agent2;

//-----------------------------------------------
//                FUNCTIONS
//-----------------------------------------------

void display();
void chargeSquares();
void keyboard(unsigned char c,int x,int y);


//-----------------------------------------------
//             MAIN PROCEDURE
//-----------------------------------------------

int main(int argc,char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(700, 400);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Maze board");
        

    chargeSquares();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

    glutMainLoop();
    return 0;
}

//-----------------------------------------------
//             CHARGE SQUARES
//-----------------------------------------------

void chargeSquares(){
    //wall.reserveMemory(maze.getNumWalls());
    wall.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);
    wall.setPositions(maze);
    
    agent1.setPosition(maze.getStartPoint());
    agent1.color.setColor(0.8,0.2,0.8);
    agent1.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);

    agent2.setPosition(maze.getEndPoint());
    agent2.color.setColor(0.3,0.2,0.8);
    agent2.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);


}

//-----------------------------------------------
//             DISPLAY PROCEDURE
//-----------------------------------------------

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT); 
    wall.draw();
    agent1.draw(SIZE_SQUARE_SMALL);
    agent2.draw(SIZE_SQUARE_SMALL);
    glutSwapBuffers();
}

//-----------------------------------------------
//            KEYBOARD EVENTS
//-----------------------------------------------

void keyboard(unsigned char c,int x,int y){

    switch (c) {
        case 'w':
        cout << "Move UP";
        agent1.moveUp();
        break;
        case 's':
        cout << "Move DOWN";
        agent1.moveUp();
        break;
        case 'a':
        cout << "Move LEFT";
        agent1.moveUp();
        break;
        case 'd':
        cout << "Move RIGHT";
        agent1.moveUp();
        break;
        case 'z':
        cout << "EXIT";
        exit(0);
        break;
    }
};
