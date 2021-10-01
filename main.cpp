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

#define MED_COLUMNS 10
#define MED_ROWS 10
#define SIZE_SQUARE_SMALL 2 // quant mes petit
#define WIDTH 500
#define HEIGHT 500

int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H = (float)HEIGHT/(float)ROWS;

Maze maze(MED_COLUMNS, MED_ROWS);

Square wall;
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
    wall.color.setColor(0.5,0.5,0.5);
    agent1.color.setColor(0.8,0.2,0.8);
    agent2.color.setColor(0.3,0.2,0.8);
    wall.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);
    agent1.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);
    agent2.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);
}

//-----------------------------------------------
//             DISPLAY PROCEDURE
//-----------------------------------------------

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT); 

   for (int i = 0; i < maze.columns ; i++) {
        for (int j = 0; j < maze.rows; j++) {
            if (maze.board[i][j] =='#'){
                wall.Draw(i,j);
            }  
            else if (maze.board[i][j] =='S'){
                agent1.Draw(i,j,SIZE_SQUARE_SMALL); 

            } else if (maze.board[i][j] =='E'){
                agent2.Draw(i,j,SIZE_SQUARE_SMALL);
            }
        }
    }
    glutSwapBuffers();
}

//-----------------------------------------------
//            KEYBOARD EVENTS
//-----------------------------------------------

void keyboard(unsigned char c,int x,int y){
    exit(0); // Qualsevol pulsacio del teclat tanca el laberint
};
