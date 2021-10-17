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

#define MED_COLUMNS 8
#define MED_ROWS 8
#define SIZE_SQUARE_SMALL 3 // quant mes petit
#define WIDTH 800
#define HEIGHT 800

int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H = (float)HEIGHT/(float)ROWS;

int anglealpha = 90;
int anglebeta = 90;
float zoomfactor = 1.0;
Maze maze(MED_COLUMNS, MED_ROWS);

Walls wall(maze.getNumWalls());


Tank agent1;
Tank agent2;
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

#define PI 3.1416
void PositionObserver(float alpha,float beta,int radi)
{
  float x,y,z;
  float upx,upy,upz;
  float modul;

  x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
  y = (float)radi*sin(beta*2*PI/360.0);
  z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

  if (beta>0)
    {
      upx=-x;
      upz=-z;
      upy=(x*x+z*z)/y;
    }
  else if(beta==0)
    {
      upx=0;
      upy=1;
      upz=0;
    }
  else
    {
      upx=x;
      upz=z;
      upy=-(x*x+z*z)/y;
    }


  modul=sqrt(upx*upx+upy*upy+upz*upz);

  upx=upx/modul;
  upy=upy/modul;
  upz=upz/modul;

  gluLookAt(x,y,z,    WIDTH/2.0,20.0, HEIGHT/2.0,    upx,upy,upz);
}

int main(int argc,char *argv[])
{
    anglealpha=90;
    anglebeta=30;
    chargeSquares();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(700, 400);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Maze board");
    glEnable(GL_DEPTH_TEST);
        
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(ArrowKey);

    glutIdleFunc(idle);


    glutMainLoop();
    return 0;
}

//-----------------------------------------------
//             CHARGE SQUARES
//-----------------------------------------------

void chargeSquares(){
    wall.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H, 15.0);
    wall.setPositions(maze);

    agent1.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H, 30.0);
    agent1.setPosition(maze.getStartPoint());

    agent2.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H, 30.0);
    agent2.setPosition(maze.getEndPoint());

    

}

//-----------------------------------------------
//             DISPLAY PROCEDURE
//-----------------------------------------------

void display() {
    glClearColor(0.8,0.8,0.8,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    PositionObserver(anglealpha,anglebeta,1000);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(WIDTH*zoomfactor),WIDTH*zoomfactor,-(HEIGHT*zoomfactor),HEIGHT*zoomfactor,100,2000);

    glMatrixMode(GL_MODELVIEW);


    wall.draw(); // walls
    wall.drawFloor(HEIGHT, WIDTH); // floor
    agent1.draw();
    //agent2.draw();

    glutSwapBuffers();
}

//-----------------------------------------------
//            KEYBOARD EVENTS
//-----------------------------------------------
void ArrowKey(int key,int x,int y){
    maze.display();
        switch (key){
            case GLUT_KEY_RIGHT:
                //moviment jugador
                if (maze.canMoveRight(agent1.position,maze.agent2) && agent1.state==QUIET){
                    if(agent1.direction!= RIGHT){
                        agent1.rotateRight();
                        agent1.direction = RIGHT;
                    }else{
                        maze.updateRight(agent1.position, maze.agent1); 
                        agent1.moveRight();
                    }
                } 
                break;
            case GLUT_KEY_LEFT:
                if (maze.canMoveLeft(agent1.position,maze.agent2) && agent1.state==QUIET){
                    if(agent1.direction!= LEFT){
                        agent1.rotateLeft();
                        agent1.direction = LEFT;
                    }else{
                    maze.updateLeft(agent1.position, maze.agent1);
                    agent1.moveLeft(); 
                    }
                }
                break;
            case GLUT_KEY_DOWN:
                if (maze.canMoveUp(agent1.position,maze.agent2) && agent1.state==QUIET){
                    if(agent1.direction!= DOWN){
                        agent1.rotateDown();
                        agent1.direction = DOWN;
                    }else{
                        maze.updateUp(agent1.position, maze.agent1);
                        agent1.moveUp();
                    }
                }
                break;
            case GLUT_KEY_UP:
                if (maze.canMoveDown(agent1.position,maze.agent2) && agent1.state==QUIET){
                    if(agent1.direction!= UP){
                        agent1.rotateUp();
                        agent1.direction = UP;
                    }else{
                    maze.updateDown(agent1.position, maze.agent1); 
                    agent1.moveDown(); 
                    }
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

    if (key=='i' && anglebeta<=(90-4))
        anglebeta=(anglebeta+3);
    else if (key=='k' && anglebeta>=(-90+4))
        anglebeta=anglebeta-3;
    else if (key=='j')
        anglealpha=(anglealpha+3)%360;
    else if (key=='l')
        anglealpha=(anglealpha-3+360)%360;
    else if (key=='o')
        zoomfactor -= 0.1;
    else if (key=='p')
        zoomfactor += 0.1;
    glutPostRedisplay();
};

//-----------------------------------------------
//            IDLE
//-----------------------------------------------

void idle(){
    long t;
    t=glutGet(GLUT_ELAPSED_TIME); 

    randomMove();
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
//comit
void randomMove(){
    bool m1,m2,m3,m4; // controlar si es queda tancat, sino -> while(true)
    int v1;
    int flag = 0;
        while(flag!=1 && !(m1&&m2&&m3&&m4)){  
            v1 = rand() % 4;
            if (v1 == 0){
                if (maze.canMoveLeft(agent2.position,maze.agent1) && agent2.state==QUIET){
                    if(agent2.direction!= LEFT){
                        agent2.rotateLeft();
                        agent2.direction = LEFT;
                    }else{
                        maze.updateLeft(agent2.position, maze.agent2);
                        agent2.moveLeft(); 
                    }
                    flag =1;
                }
                m1 = true;
            }else if (v1 == 1){
                if (maze.canMoveDown(agent2.position,maze.agent1) && agent2.state==QUIET){
                    if(agent2.direction!= UP){
                        agent2.rotateUp();
                        agent2.direction = UP;
                    }else{
                        maze.updateDown(agent2.position, maze.agent2); 
                        agent2.moveDown(); 
                    }
                    flag =1;
                }
                m2 = true;
            }else if (v1 == 2){

                if (maze.canMoveRight(agent2.position,maze.agent1) && agent2.state==QUIET){
                    if(agent2.direction!= RIGHT){
                        agent2.rotateRight();
                        agent2.direction = RIGHT;
                    }else{
                        maze.updateRight(agent2.position, maze.agent2); 
                        agent2.moveRight();
                    }
                    flag =1;
                } 
                m3 = true;
            }else if (v1 == 3){
                if (maze.canMoveUp(agent2.position,maze.agent1) && agent2.state==QUIET){
                    if(agent2.direction!= DOWN){
                        agent2.rotateDown();
                        agent2.direction = DOWN;
                    }else{
                        maze.updateUp(agent2.position, maze.agent2);
                        agent2.moveUp();
                    }
                    flag =1;
                }
                m4 = true;
            }  
        }
    
}