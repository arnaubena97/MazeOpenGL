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

int anglealpha = 0;
int anglebeta = 0;
float zoomfactor = 1.0;
Maze maze(MED_COLUMNS, MED_ROWS);

Walls wall(maze.getNumWalls());

//Square wall;
Square start;
Square endsa;
Square agent1;
Square agent2;
Tank tankUser;
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

    //glMatrixMode(GL_PROJECTION);
    //gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

    glutMainLoop();
    return 0;
}

//-----------------------------------------------
//             CHARGE SQUARES
//-----------------------------------------------

void chargeSquares(){
    wall.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H, 25);
    wall.setPositions(maze);

    tankUser.setSizesXY(SIZE_SQUARE_W, SIZE_SQUARE_H, 50);
    tankUser.color.setColor(0.1,1,0.1);
    tankUser.setPosition(maze.getStartPoint());

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
    wall.drawFloor(HEIGHT, WIDTH);
    tankUser.draw();
    //start.draw(); //start point
    //endsa.draw(); // end point
    //agent1.draw(SIZE_SQUARE_SMALL); // agent user
    //agent2.draw(SIZE_SQUARE_SMALL); //agent pc

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
//comit
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