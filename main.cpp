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

#define MED_COLUMNS 6
#define MED_ROWS 6
#define SIZE_SQUARE_SMALL 2 // quant mes petit
#define WIDTH 700
#define HEIGHT 700

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
void ArrowKey(int key,int x,int y);


//-----------------------------------------------
//             MAIN PROCEDURE
//-----------------------------------------------
long last_t=0;
Square square;

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
    glutSpecialFunc(ArrowKey);

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

void randomMove(){
    int v1 = rand() % 4;   
    
    if (v1 == 0){
        if (maze.canMoveLeft(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveLeft();
        else if (maze.canMoveDown(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveDown();
        else if (maze.canMoveRight(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveRight() ;
    }else if (v1 == 1){
        if (maze.canMoveDown(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveDown();
        else if (maze.canMoveRight(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveRight() ;
        else if (maze.canMoveUp(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveUp() ;
    }else if (v1 == 2){
        if (maze.canMoveRight(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveRight() ;
        else if (maze.canMoveUp(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveUp() ;
        else if (maze.canMoveLeft(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveLeft();
    }else if (v1 == 3){
        if (maze.canMoveUp(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveUp() ;
        else if (maze.canMoveLeft(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveLeft();
        else if (maze.canMoveDown(agent2.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent2.moveDown();
    }  
}

void ArrowKey(int key,int x,int y){

    switch (key){
        case GLUT_KEY_RIGHT:
            //moviment jugador
            if (maze.canMoveRight(agent1.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent1.moveRight() ;
            //moviment enemic
            randomMove();
            break;
        case GLUT_KEY_LEFT:
            if (maze.canMoveLeft(agent1.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent1.moveLeft();
            //moviment enemic
            randomMove();
            break;
        case GLUT_KEY_UP:
            if (maze.canMoveUp(agent1.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent1.moveUp();
            //moviment enemic
            randomMove();
            break;
        case GLUT_KEY_DOWN:
            if (maze.canMoveDown(agent1.position) && (agent2.position.x != agent1.position.x || agent2.position.y != agent1.position.y)) agent1.moveDown();
            //moviment enemic
           randomMove();
            break;
        case 'z':
            exit(0);
            break;
    }

    glutPostRedisplay();
};

void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 27:
        exit(0);
        break;
    }
};

//-----------------------------------------------
//-----------------------------------------------
void idle(){
  long t;

  t=glutGet(GLUT_ELAPSED_TIME); 

  if(last_t==0)
    last_t=t;
  else
    {
      square.integrate(t-last_t);
      last_t=t;
    }


  glutPostRedisplay();
}