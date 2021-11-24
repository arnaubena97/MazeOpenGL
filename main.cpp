/********************************************************
 *
 * Authors: Arnau Benavides, Marc Felip
 *
*********************************************************/

//-----------------------------------------------
//           INCLUDES DEPENDS OS
//-----------------------------------------------
#include "extras.h"
#include "maze.h"
#include "square.h"
#include "dfs.h"
#include <string.h>
#include <string> 
#include <iostream>
#include <stdio.h>
#include <vector>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#if  __linux__ 
    #include <GL/glut.h>
    #include "jpeglib.h"
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include "jpeg-9d3/jpeglib.h"
#endif




//-----------------------------------------------
//              GLOBAL VARIABLES
//-----------------------------------------------
#define ARDUINO true
#define MED_COLUMNS 6 // Tamany del tauler
#define MED_ROWS 6
#define SIZE_SQUARE_SMALL 3 // quant mes petit
#define WIDTH 800  //tamany de la finestra
#define HEIGHT 800


int TIME = 60; // Temps de joc
int TIME_PLAYER_OFF = 2; // temps que el jugador desapareix
int time_show = TIME;  // auxiliar pel temps de joc
int time_show_shooted = TIME_PLAYER_OFF; // auxiliar per un jugador disparat

int flagExit = 0; // flag per marxar del joc
int flagShooted = 0; // flag per saber si han disparat
int endGame =0; // flag marxar del joc

int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H = (float)HEIGHT/(float)ROWS;

int anglealpha = 90;
int anglebeta = 90;
float zoomfactor = 1.0;
long last_t=0;
#define PI 3.1416
int index_path = 0; // index del cami pel dfs agafar el proxim moviment

Maze maze(MED_COLUMNS, MED_ROWS);// crar el laberint
Walls wall(maze.getNumWalls()); // afegir les parets
Tank agent1(maze.agent1); // agent1 es l'usuari
Tank agent2(maze.agent2); // agent2 te IA

dfs intell(COLUMNS, ROWS, maze.board); // fem el DFS per trobar el cami

FILE *serialPort;
int code = 0;
//-----------------------------------------------
//                FUNCTIONS
//-----------------------------------------------
void randomMove();
void dfsMove();
void display();
void chargeSquares();
void keyboard(unsigned char c,int x,int y);
void ArrowKey(int key,int x,int y);
void idle();
void ReadJPEG(char *filename,unsigned char **image,int *width, int *height);
void LoadTexture(char *filename,int dim);
void readSerialPort();
void moveSerialPort();

//-----------------------------------------------
//             MAIN PROCEDURE
//-----------------------------------------------
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
    
    //readSerialPort();
    anglealpha=90;
    anglebeta=30;
    chargeSquares();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(700, 400);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Maze board");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
        
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    if(!ARDUINO){
        glutSpecialFunc(ArrowKey);
    }else{
        
        system( "MODE /dev/cu.usbserial-0001 : BAUD=9600 PARITY=n DATA=8 STOP=1" );
        serialPort = fopen("/dev/cu.usbserial-0001", "r" );

        if (serialPort == NULL) {
            printf ("Error: unable to open serial port COM4\n");
            exit (1);
        }
    }

    glutIdleFunc(idle);

    glBindTexture(GL_TEXTURE_2D,0);
    LoadTexture("textures/pared.jpg",64);
    
    glBindTexture(GL_TEXTURE_2D,1);
    LoadTexture("textures/grava.jpg",64);
    
    
    glBindTexture(GL_TEXTURE_2D,2);
    LoadTexture("textures/wood.jpg",64);
    
    
    glBindTexture(GL_TEXTURE_2D,3);
    LoadTexture("textures/grass.jpg",64);
    
    


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
//             DISPLAY TEXT
//-----------------------------------------------
void display_text(string s, int color=0){
    float size = 0.3;
    if(color ==0)(time_show<= 10) ? glColor3f(1.0, 0.0, 0.0): glColor3f(0.0, 0.0, 0.0);
    if(color==1)(glColor3f(0.0, 1.0, 0.0));
    if(color==2)(glColor3f(1.0, 0.0, 0.0));
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(WIDTH*0.04, HEIGHT *0.91, 1);
    glScalef(size, size, 1);
    for( char* p = &s[0] ; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
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
    if(flagShooted !=1){
        agent1.draw();
    }
    if(flagShooted !=2){
        agent2.draw();
    }
    if (time_show <=0 && endGame==0){
        display_text("TIME OUT");
        if (time_show <=-2) exit(0);
    }else if(endGame==1){
        display_text("PLAYER 1 WIN", 1);
        if (time_show <=-2) exit(0);
    }else if(endGame==2){
        display_text("PLAYER 2 WIN", 2);
        if (time_show <=-2) exit(0);
    }else{
        display_text("Time left: " + std::to_string(time_show) + "s");
    }
    
    glutSwapBuffers();
}

//-----------------------------------------------
//            KEYBOARD EVENTS
//-----------------------------------------------
void ArrowKey(int key,int x,int y){
    //printf(" %d \n",key);
    if(endGame == 0 && flagShooted!=1){
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
    }
    //maze.display();
    glutPostRedisplay();
};

void keyboard(unsigned char key, int x, int y){
    printf("%c",key);
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
    else if (key==' '){
        if(agent1.state == QUIET){// sino es produeixen bugs de mig moviments
            int len = maze.getLenShoot(agent1.symbol, agent1.direction);
            agent1.shoot(len);
        }
    }
    glutPostRedisplay();
};

//-----------------------------------------------
//            IDLE
//-----------------------------------------------

void idle(){
    if(ARDUINO){
    //readSerialPort();
    moveSerialPort();
    }
    long t;
    t=glutGet(GLUT_ELAPSED_TIME); 
    if(endGame != 0 && flagExit != endGame){
        time_show = 1;
        flagExit=endGame;
        TIME = 1 + (int)((int)t/1000);
    } else if(endGame ==0){
        
        //printf("shot x: %f y:%f\n", agent1.position_shoot.x, agent1.position_shoot.y);
        //printf("pos x: %f y:%f\n", agent2.position.x, agent2.position.y);
        if(agent1.position_shoot.Equal(agent2.position) && flagShooted==0){
            //printf("AGENT2 DEAD\n");
            maze.resetAgent(maze.agent2);
            flagShooted = 2;
            time_show_shooted = (int)((int)t/1000);
        }
        if(agent2.position_shoot.Equal(agent1.position) && flagShooted==0){
            //printf("AGENT1 DEAD\n");
            maze.resetAgent(maze.agent1);
            flagShooted = 1;
            time_show_shooted = (int)((int)t/1000);
        }
        if((int)((int)t/1000) - time_show_shooted > TIME_PLAYER_OFF && flagShooted!=0){
            if(flagShooted == 2){
                maze.putPlayer(maze.agent2, maze.endPosition);
                agent2.reset(maze.endPosition);
                agent2.position_shoot = agent2.position;
                flagShooted = 0;
                index_path = 0;
            }else if(flagShooted ==1){
                maze.putPlayer(maze.agent1, maze.startPosition);
                agent1.reset(maze.startPosition);
                agent1.position_shoot = agent1.position;
                flagShooted = 0;
            }
        }
        if(flagShooted != 2){
            //randomMove();
            dfsMove();
        }
        
        endGame = maze.checkEnd(agent1.state, agent2.state);
    }
    time_show = TIME - (int)((int)t/1000);
    if(last_t==0){
        last_t=t;
    }
    else{
        agent1.integrate(t-last_t);
        agent2.integrate(t-last_t);
        last_t=t;
    }

    glutPostRedisplay();
}

//-----------------------------------------------
//            RANDOM MOVE --> ADD DFS
//-----------------------------------------------
void randomMove(){
    bool m1,m2,m3,m4, m5; // controlar si es queda tancat, sino -> while(true)
    int v1;
    int flag = 0;
    while(flag!=1 && !(m1&&m2&&m3&&m4)){  
        v1 = rand() % 9;
        if (v1 == 8 || v1 == 1){
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
        }else if (v1 == 2 || v1 == 3){
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
        }else if (v1 == 4 || v1 == 5){
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
        }else if (v1 == 6 || v1 == 7){
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
        else if((v1 == 9 ||v1==0) && agent2.state==QUIET){
            int len = maze.getLenShoot(agent2.symbol, agent2.direction);
            agent2.shoot(len);
            flag =1;
            m5 = true;
        }
    }
}
char revert(char c){
    char ret;
    if(c=='d') ret = 'l';
    else if(c=='u') ret = 'r';
    else if(c=='l') ret = 'd';
    else if(c=='r') ret = 'u';
    return ret;
}

void dfsMove(){
    int v1 = rand() % 3; // Triem aleatoriament si ens movem o disparem
    if(v1 == 0 || v1 == 1){//MOVE
        char move = revert(intell.dir[index_path]);
        //printf("%d, %d, %c\n",intell.lenPath,index_path,move);
        if(move == 'd'){
            if (maze.canMoveUp(agent2.position,maze.agent1) && agent2.state==QUIET){
                if(agent2.direction!= DOWN){
                    agent2.rotateDown();
                    agent2.direction = DOWN;
                }else{
                    maze.updateUp(agent2.position, maze.agent2);
                    agent2.moveUp();
                    index_path ++;
                }
            }
        }else if(move == 'u'){
            if (maze.canMoveDown(agent2.position,maze.agent1) && agent2.state==QUIET){
                if(agent2.direction!= UP){
                    agent2.rotateUp();
                    agent2.direction = UP;
                }else{
                    maze.updateDown(agent2.position, maze.agent2); 
                    agent2.moveDown(); 
                    index_path ++;
                }
            }
        }else if(move == 'l'){
            if (maze.canMoveLeft(agent2.position,maze.agent1) && agent2.state==QUIET){
                if(agent2.direction!= LEFT){
                    agent2.rotateLeft();
                    agent2.direction = LEFT;
                }else{
                    maze.updateLeft(agent2.position, maze.agent2);
                    agent2.moveLeft();
                    index_path ++;
                }
            }
        }else if(move == 'r'){
            if (maze.canMoveRight(agent2.position,maze.agent1) && agent2.state==QUIET){
                if(agent2.direction!= RIGHT){
                    agent2.rotateRight();
                    agent2.direction = RIGHT;
                }else{
                    maze.updateRight(agent2.position, maze.agent2); 
                    agent2.moveRight();
                    index_path ++;
                } 
            } 
        }
    }else if( v1==2 && agent2.state==QUIET){//SHOOT
        int len = maze.getLenShoot(agent2.symbol, agent2.direction);
        agent2.shoot(len);
    }
}
/*--------------------------------------------------------*/
/*--------------------------------------------------------*/
void ReadJPEG(char *filename,unsigned char **image,int *width, int *height)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  unsigned char **buffer;
  int i,j;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);


  if ((infile = fopen(filename, "rb")) == NULL) {
    printf("Unable to open file %s\n",filename);
    exit(1);
  }

  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_calc_output_dimensions(&cinfo);
  jpeg_start_decompress(&cinfo);

  *width = cinfo.output_width;
  *height  = cinfo.output_height;


  *image=(unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

  buffer=(unsigned char **)malloc(1*sizeof(unsigned char **));
  buffer[0]=(unsigned char *)malloc(cinfo.output_width*cinfo.output_components);


  i=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    for(j=0;j<cinfo.output_width*cinfo.output_components;j++)
      {
	(*image)[i]=buffer[0][j];
	i++;
      }   

    }

  free(buffer);
  jpeg_finish_decompress(&cinfo);
} 

/*--------------------------------------------------------*/
/*--------------------------------------------------------*/
void LoadTexture(char *filename,int dim)
{
  unsigned char *buffer;
  unsigned char *buffer2;
  int width,height;
  long i,j;
  long k,h;

  ReadJPEG(filename,&buffer,&width,&height);

  buffer2=(unsigned char*)malloc(dim*dim*3);

  //-- The texture pattern is subsampled so that its dimensions become dim x dim --
  for(i=0;i<dim;i++)
    for(j=0;j<dim;j++)
      {
	k=i*height/dim;
	h=j*width/dim;
	
	buffer2[3*(i*dim+j)]=buffer[3*(k*width +h)];
	buffer2[3*(i*dim+j)+1]=buffer[3*(k*width +h)+1];
	buffer2[3*(i*dim+j)+2]=buffer[3*(k*width +h)+2];

      }

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim,dim,0,GL_RGB,GL_UNSIGNED_BYTE,buffer2);

  free(buffer);
  free(buffer2);
}

void moveSerialPort(){
    readSerialPort();
    if (code != 0){
        ArrowKey(code, 0, 0);
    }
}

void readSerialPort(){
    //int code = 0;
    char t[2];
    char * b;
    //fwrite (buffer , sizeof(char), sizeof(buffer), serialPort);
    size_t n;
    //cout << b;
    //char * t = fgetln(serialPort,&n);
    printf("helrrrrrrrrrrrrlo\n");
    b=fgets (t,2, serialPort);
    printf("11111111111hello   %c\n", t);
     //fclose (pFile);
    //cout << t;
    if (t == "1"){
        code = 101;
    }else if (t == "2"){
        code = 103;
    }else if (t == "3"){
        code = 102;
    }else if (t == "4"){
        code = 100;
    }
}