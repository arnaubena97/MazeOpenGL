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
#include "extras.h"
#include "maze.h"
#include "square.h"

//-----------------------------------------------
//              GLOBAL VARIABLES
//-----------------------------------------------

#define MED_COLUMNS 6
#define MED_ROWS 6
#define SIZE_SQUARE_SMALL 3 // quant mes petit
#define WIDTH 700
#define HEIGHT 700

int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H = (float)HEIGHT/(float)ROWS;

Maze maze(MED_COLUMNS, MED_ROWS);

Walls wall(maze.getNumWalls());

//Square wall;
Square start;
Square endsa;
Square agent1;
Square agent2;

//-----------------------------------------------
//                FUNCTIONS
//-----------------------------------------------
void randomMove();
void display();
void chargeSquares();
void keyboard(unsigned char c,int x,int y);
void ArrowKey(int key,int x,int y);
void idle();

//-----------------------------------------------
//             MAIN PROCEDURE
//-----------------------------------------------
long last_t=0;
Square square;

int main(int argc,char *argv[])
{

    chargeSquares();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(700, 400);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Maze board");
        
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(ArrowKey);

    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

    glutMainLoop();
    return 0;
}

//-----------------------------------------------
//             CHARGE SQUARES
//-----------------------------------------------

void chargeSquares(){
    wall.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);
    wall.setPositions(maze);
    
    start.setPosition(maze.getStartPoint());
    start.color.setColor(0.9,0.1,0.1);
    start.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);

    endsa.setPosition(maze.getEndPoint());
    endsa.color.setColor(0.1,0.9,0.1);
    endsa.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);


    agent1.setPosition(maze.getStartPoint());
    agent1.color.setColor(1,0.7,0);
    agent1.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);

    agent2.setPosition(maze.getEndPoint());
    agent2.color.setColor(0.3,0.2,0.6);
    agent2.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H);

}

//-----------------------------------------------
//             DISPLAY PROCEDURE
//-----------------------------------------------

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT); 
    wall.draw(); // walls
    start.draw(); //start point
    endsa.draw(); // end point
    agent1.draw(SIZE_SQUARE_SMALL); // agent user
    agent2.draw(SIZE_SQUARE_SMALL); //agent pc

    glutSwapBuffers();
}

//-----------------------------------------------
//            KEYBOARD EVENTS
//-----------------------------------------------
void ArrowKey(int key,int x,int y){

        switch (key){
            case GLUT_KEY_RIGHT:
                //moviment jugador
                if (maze.canMoveRight(agent1.position,maze.agent2) && agent1.state==QUIET){
                    maze.updateRight(agent1.position, maze.agent1); 
                    agent1.moveRight();
                } 
                break;
            case GLUT_KEY_LEFT:
                if (maze.canMoveLeft(agent1.position,maze.agent2) && agent1.state==QUIET){
                    maze.updateLeft(agent1.position, maze.agent1);
                    agent1.moveLeft(); 
                }
                break;
            case GLUT_KEY_UP:
                if (maze.canMoveUp(agent1.position,maze.agent2) && agent1.state==QUIET){
                    maze.updateUp(agent1.position, maze.agent1);
                    agent1.moveUp();
                }
                break;
            case GLUT_KEY_DOWN:
                if (maze.canMoveDown(agent1.position,maze.agent2) && agent1.state==QUIET){
                    maze.updateDown(agent1.position, maze.agent1); 
                    agent1.moveDown(); 
                }
                break;
        }
    
    glutPostRedisplay();
};

void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 27: // exit with ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
};

//-----------------------------------------------
//            IDLE
//-----------------------------------------------

void idle(){
    long t;
    randomMove();
    t=glutGet(GLUT_ELAPSED_TIME); 

    if(last_t==0)
        last_t=t;
    else{
        agent1.integrate(t-last_t);
        agent2.integrate(t-last_t);
        last_t=t;
    }

    glutPostRedisplay();
}

//-----------------------------------------------
//            RANDOM MOVE
//-----------------------------------------------

void randomMove(){
    bool m1,m2,m3,m4; // controlar si es queda tancat, sino -> while(true)
    int v1;
    int flag = 0;
        while(flag!=1 && !(m1&&m2&&m3&&m4)){  
            v1 = rand() % 4;
            if (v1 == 0){
                if (maze.canMoveLeft(agent2.position, maze.agent1) && agent2.state==QUIET){
                    maze.updateLeft(agent2.position, maze.agent2);
                    agent2.moveLeft();
                    flag =1;
                }
                m1 = true;
            }else if (v1 == 1){
                if (maze.canMoveDown(agent2.position, maze.agent1) && agent2.state==QUIET){
                    maze.updateDown(agent2.position, maze.agent2); 
                    agent2.moveDown();
                    flag =1;
                } 
                m2 = true;
            }else if (v1 == 2){
                if (maze.canMoveRight(agent2.position, maze.agent1) && agent2.state==QUIET){
                    maze.updateRight(agent2.position, maze.agent2);
                    agent2.moveRight();
                    flag =1;
                } 
                m3 = true;
            }else if (v1 == 3){
                if (maze.canMoveUp(agent2.position, maze.agent1) && agent2.state==QUIET){
                    maze.updateUp(agent2.position, maze.agent2);
                    agent2.moveUp();
                    flag =1;
                } 
                m4 = true;
            }  
        }
    
}