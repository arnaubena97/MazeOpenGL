#if  __linux__ 
    #include <GL/glut.h>
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#endif
#include <tuple> 
using namespace std;


#define MOVE 1
#define QUIET 2



class Square {     
    public:       
        Point position; // Current position
        float vx,vy; // Velocity vector
        int state; 
        long time_remaining, time_mov;
        RGB color; 
        float size_x, size_y; // size of square
        Square(){
            state = QUIET;
            time_mov= 300;
        }
        
        void draw(int scale =0){
            //printf("x:%d y:%d sizex:%f sizey:%f \n", x, y , size_x, size_y);
            glColor3f(color.r,color.g,color.b);
            glBegin(GL_QUADS);
            glVertex2i(position.x * size_x + scale ,position.y*size_y + scale);//vertex baix esquerra
            glVertex2i((position.x+1)* size_x -scale,position.y*size_y+scale);//vertex baix dreta
            glVertex2i((position.x+1) * size_x -scale ,(position.y+1)*size_y-scale);//vertex dalt dreta
            glVertex2i(position.x * size_x +scale,(position.y+1)*size_y-scale);//vertex dalt esquerra
            glEnd();
        }
        void setSizesXY(float x, float y){
            size_y = y;
            size_x = x;
        }
        
        void init_movement(int destination_x,int destination_y,int duration){
            vx = (destination_x - position.x)/duration;
            vy = (destination_y - position.y)/duration;
            state=MOVE;
            time_remaining=duration;
        }
        void integrate(long t){
            if(state==MOVE && t<time_remaining)
                {
                position.x = position.x + vx*t;
                position.y = position.y + vy*t;
                time_remaining-=t;
                }
            else if(state==MOVE && t>=time_remaining)
                {
                position.x = roundf(position.x + vx*time_remaining);
                position.y = roundf(position.y + vy*time_remaining);
                state=QUIET;
                }
        }
        
        //functions to set the position of square
        void setPosition(float x, float y){
            position = Point(x,y);
        }
        void setPosition(Point pos){
            position = pos;
        }

        //functions to move square 1 position
        void moveUp(){
            init_movement(position.x,position.y +1,time_mov);
        }
        void moveDown(){
            init_movement(position.x,position.y -1,time_mov);
        }
        void moveLeft(){
            init_movement(position.x-1,position.y,time_mov);
        }
        void moveRight(){
            init_movement(position.x+1,position.y,time_mov);
        } 
};

class SquareWall {     
    public:       
        Point position; // Current position
        RGB color; 
        float size_x, size_y, size_z; // size of square
        SquareWall(){
        }

        void setSizesXY(float x, float y, float z = 0){
            size_y = y;
            size_x = x;
            size_z = z;
        }
        //functions to set the position of square
        void setPosition(float x, float y, float z = 0){
            position = Point(x,y,z);
        }
        void setPosition(Point pos){
            position = pos;
        }
        void draw(){
            GLfloat x = position.x * size_x;
            GLfloat x1 = (position.x + 1) * size_x;
            GLfloat z = position.y * size_y;
            GLfloat z1 =(position.y + 1) * size_y;
            GLfloat y = position.z * size_z;
            GLfloat y1 = (position.z + 1) * size_z;
            
            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_BACK,GL_LINE);


            glColor3f(1,0,1);
            glBegin(GL_QUADS); // BACK
            glVertex3i(x1,y,z);
            glVertex3i(x,y,z);
            glVertex3i(x,y1,z);
            glVertex3i(x1,y1,z);
            glEnd();

            glColor3f(1,0,0); // FRONT
            glBegin(GL_QUADS);
            glVertex3i(x1,y1,z1);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y,z1);
            glVertex3i(x1,y,z1);
            glEnd();

            glColor3f(0,1,0); //RIGHT
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y1,z);
            glVertex3i(x,y,z);
            glVertex3i(x,y,z1);
            glEnd();

            glColor3f(0,0,1); // LEFT
            glBegin(GL_QUADS);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x1,y1,z1);
            glEnd();

            glColor3f(0.5,0.5,0.5); // TOP
            glBegin(GL_QUADS);
            glVertex3i(x,y,z1);
            glVertex3i(x,y,z);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y,z1);
            glEnd();

            glColor3f(0.3,0.6,0.7); // BOTTOM
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z);
            glVertex3i(x,y1,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y1,z);
            glEnd();
        }
};

class Walls {   
    RGB color;
    float size_x, size_y, size_z; // size of square // size squares
    SquareWall *walls; // array of squares wall
    int num_walls; // num of squares wall
    public: 
        Walls(int num){
            color.setColor(0.5,0.5,0.5);
            num_walls = num;
            walls = new SquareWall[num_walls];
            
        }
        //set positions of all walls
        void setPositions(Maze m){
            int cnt =0;
            for (int i = 0; i < m.columns ; i++) {
                for (int j = 0; j < m.rows; j++) {
                    if (m.board[i][j] =='#'){
                        walls[cnt].setPosition(i, j);
                        walls[cnt].color = color;
                        walls[cnt].setSizesXY(size_x,size_y, size_z);
                        cnt++;
                    }
                }
            }
        }
        // draw all squares wall
        void draw(int scale =0){
            for(int i = 0; i< num_walls; i++){
                walls[i].draw();
            }
        }
        //set sizes of squares wall
        void setSizesXY(float x, float y, float z = 0){
            size_y = y;
            size_x = x;
            size_z = z;
        }
        void drawFloor(float x, float y){
            glColor3f(1.0,1.0,1.0);
            glBegin(GL_QUADS);
            glVertex3i(0,0,0);//vertex baix esquerra
            glVertex3i(0,0,y);//vertex baix dreta
            glVertex3i(x,0,y);//vertex dalt dreta
            glVertex3i(x,0,0);//vertex dalt esquerra
            glEnd();
        }
};


class Tank{     
    public:       
        Point position; // Current position
        RGB color, colorWheels, colorTank, colorCanon; 
        float size_x, size_y, size_z; // size of square
        Tank(){
        }

        void setSizesXY(float x, float y, float z = 0){
            size_y = y;
            size_x = x;
            size_z = z;
        }
        //functions to set the position of square
        void setPosition(float x, float y, float z = 0){
            position = Point(x,y,z);
        }
        void setPosition(Point pos){
            position = pos;
        }

        void draw(){

            GLfloat x0 = position.x * size_x;
            GLfloat x1 = (position.x + 0.125)* size_x;
            GLfloat x2 = (position.x + 0.25) * size_x;
            GLfloat x3 = (position.x + 0.375) * size_x;
            GLfloat x4 = (position.x + 0.5) * size_x;
            GLfloat x5 = (position.x + 0.625) * size_x;
            GLfloat x6 = (position.x + 0.75)* size_x;
            GLfloat x7= (position.x + 0.875)* size_x;
            GLfloat x8 = (position.x + 1) * size_x;
            
            GLfloat z0 = position.y * size_y;
            GLfloat z1 = (position.y + 0.25) * size_y;
            GLfloat z2 = (position.y + 0.5) * size_y;
            GLfloat z3 = (position.y + 0.75)* size_y;
            GLfloat z4 = (position.y + 1) * size_y;

            GLfloat y0 = position.z * size_z;
            GLfloat y1 = (position.z + (1.0/6.0)) * size_z;
            GLfloat y2 = (position.z + (1.0/3.0)) * size_z;
            GLfloat y3 = (position.z + (5.0/6.0)) * size_z;
            GLfloat y4 = (position.z + 1) * size_z;

            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_BACK,GL_LINE);
            drawWheelsL(x0, x1, x2, x4, x6, x7, x8, y0, y1, y2, z0, z1);
            drawWheelsR(x0, x1, x2, x4, x6, x7, x8, y0, y1, y2, z3, z4);
            drawBody(x1,x7,y1,y3,z1,z3);
            drawCanon(x4,x0,z0,z1,z2,y1,y3,y4);
        }

        void drawWheelsL(GLfloat x0, GLfloat x1, GLfloat x2,GLfloat x4,
                        GLfloat x6, GLfloat x7, GLfloat x8, GLfloat y0,
                        GLfloat y1, GLfloat y2, GLfloat z0,GLfloat z1){

            glColor3f(1,0,0);
            glBegin(GL_QUADS); // xapa1 fora
            glVertex3i(x0,y2,z0);
            glVertex3i(x1,y0,z0);
            glVertex3i(x1,y0,z1);
            glVertex3i(x0,y2,z1);
            glEnd();

            glColor3f(0,1,0);
            glBegin(GL_QUADS); // xapa2 fora
            glVertex3i(x8,y2,z0);
            glVertex3i(x0,y2,z0);
            glVertex3i(x0,y2,z1);
            glVertex3i(x8,y2,z1);
            glEnd();

            glColor3f(0,0,1);
            glBegin(GL_QUADS); // xapa3 fora
            glVertex3i(x1,y0,z0);
            glVertex3i(x7,y0,z0);
            glVertex3i(x7,y0,z1);
            glVertex3i(x1,y0,z1);
            glEnd();
            
            glColor3f(1,1,0);
            glBegin(GL_QUADS); // xapa4 fora
            glVertex3i(x7,y0,z0);
            glVertex3i(x8,y2,z0);
            glVertex3i(x8,y2,z1);
            glVertex3i(x7,y0,z1);
            glEnd();

            
            glColor3f(0.4,0.2,0.8);
            glBegin(GL_QUADS); // xapa5 fora
            glVertex3i(x0,y2,z1);
            glVertex3i(x1,y0,z1);
            glVertex3i(x7,y0,z1);
            glVertex3i(x8,y2,z1);
            glEnd();


            glColor3f(1,0,1);
            glBegin(GL_QUADS); // xapa6 exterior
            glVertex3i(x1,y0,z0*1.03);
            glVertex3i(x0,y2,z0*1.03);
            glVertex3i(x8,y2,z0*1.03);
            glVertex3i(x7,y0,z0*1.03);
            glEnd();

            GLUquadricObj *p = gluNewQuadric();
            //gluQuadricDrawStyle(GLU_LINE);
            glTranslatef(x2, y1, z0);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2, 100, 100);
            glTranslatef(-x2, 0, 0);
            glTranslatef(x4, 0, 0);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2, 100, 100);
            glTranslatef(-x4, 0, 0);
            glTranslatef(x6, 0, 0);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2, 100, 100);
            glTranslatef(-x6, -y1, -z0);
            glEnd();


        }
        
        void drawWheelsR(GLfloat x0, GLfloat x1, GLfloat x2,GLfloat x4,
                        GLfloat x6, GLfloat x7, GLfloat x8, GLfloat y0,
                        GLfloat y1, GLfloat y2, GLfloat z3,GLfloat z4){

            glColor3f(1,0,0);
            glBegin(GL_QUADS); // xapa1 fora
            glVertex3i(x0,y2,z3);
            glVertex3i(x1,y0,z3);
            glVertex3i(x1,y0,z4);
            glVertex3i(x0,y2,z4);
            glEnd();

            glColor3f(0,1,0);
            glBegin(GL_QUADS); // xapa2 fora
            glVertex3i(x8,y2,z3);
            glVertex3i(x0,y2,z3);
            glVertex3i(x0,y2,z4);
            glVertex3i(x8,y2,z4);
            glEnd();

            glColor3f(0,0,1);
            glBegin(GL_QUADS); // xapa3 fora
            glVertex3i(x1,y0,z3);
            glVertex3i(x7,y0,z3);
            glVertex3i(x7,y0,z4);
            glVertex3i(x1,y0,z4);
            glEnd();
            
            glColor3f(1,1,0);
            glBegin(GL_QUADS); // xapa4 fora
            glVertex3i(x7,y0,z3);
            glVertex3i(x8,y2,z3);
            glVertex3i(x8,y2,z4);
            glVertex3i(x7,y0,z4);
            glEnd();

            
            glColor3f(0.4,0.2,0.8);
            glBegin(GL_QUADS); // xapa5 fora
            glVertex3i(x1,y0,z3);
            glVertex3i(x0,y2,z3);
            glVertex3i(x8,y2,z3);
            glVertex3i(x7,y0,z3);
            glEnd();


            glColor3f(1,0,1);
            glBegin(GL_QUADS); // xapa6 exterior
            glVertex3i(x0,y2,z4*0.97);
            glVertex3i(x1,y0,z4*0.97);
            glVertex3i(x7,y0,z4*0.97);
            glVertex3i(x8,y2,z4*0.97);
            glEnd();

            GLUquadricObj *p = gluNewQuadric();
            glTranslatef(x2, y1, z4);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2, 100, 100);
            glTranslatef(-x2, 0, 0);
            glTranslatef(x4, 0, 0);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2,100, 100);
            glTranslatef(-x4, 0, 0);
            glTranslatef(x6, 0, 0);
            glColor3f(0,0.2,0);
            gluDisk(p, 0,y1/2, 100, 100);
            glTranslatef(-x6, -y1, -z4);
            glEnd();
        }
        
        void drawBody(GLfloat x1, GLfloat x7, GLfloat y1,
                    GLfloat y3, GLfloat z1, GLfloat z3){

            glColor3f(1,0,1);
            glBegin(GL_QUADS); // xapa1 sota
            glVertex3i(x1,y1,z1);
            glVertex3i(x7,y1,z1);
            glVertex3i(x7,y1,z3);
            glVertex3i(x1,y1,z3);
            glEnd();


            glColor3f(0.1,0,1);
            glBegin(GL_QUADS); // xapa1 costat esq
            glVertex3i(x7,y1,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y3,z1);
            glVertex3i(x7,y3,z1);
            glEnd();    

            glColor3f(0.1,0.4,0.5);
            glBegin(GL_QUADS); // xapa1 costat dret
            glVertex3i(x1,y1,z3);
            glVertex3i(x7,y1,z3);
            glVertex3i(x7,y3,z3);
            glVertex3i(x1,y3,z3);
            glEnd();   

            glColor3f(0.1,0.2,0.5);
            glBegin(GL_QUADS); // xapa1 costat davat
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y1,z3);
            glVertex3i(x1,y3,z3);
            glVertex3i(x1,y3,z1);
            glEnd();  

            glColor3f(0.5,0.2,0.2);
            glBegin(GL_QUADS); // xapa1 costat davat
            glVertex3i(x7,y1,z3);
            glVertex3i(x7,y1,z1);
            glVertex3i(x7,y3,z1);
            glVertex3i(x7,y3,z3);
            glEnd();  

            glColor3f(0.5,0.7,0.1);
            glBegin(GL_QUADS); // xapa1 costat davat
            glVertex3i(x1,y3,z1);
            glVertex3i(x1,y3,z3);
            glVertex3i(x7,y3,z3);
            glVertex3i(x7,y3,z1);
            
            glEnd(); 

        }

        void drawCanon(GLfloat x4,GLfloat x0, GLfloat z0, GLfloat z1,
                     GLfloat z2, GLfloat y1,GLfloat y3,GLfloat y4){
            GLfloat zr = z1-z0;
            GLUquadricObj *p = gluNewQuadric();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glTranslatef(x4, z2, -y4);
            glColor3f(1,0.8,0);
            gluCylinder(p, zr, zr, y1, 500, 500);
            //glTranslatef(-x4, -z2, +y4);
            //glRotatef(90.0, -1.0, 0.0, 0.0);
            glColor3f(0.2,0.5,1);
            gluDisk(p, 0,zr, 100, 100);
            glRotatef(90.0, -1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glColor3f(0.1,0.5,0.1);
            glTranslatef(0, -(y4-y3)/2.0, 0);
            gluCylinder(p, (y1/2.0)*0.8, (y1/2.0)*0.8, x4-x0, 500, 500);
            glTranslatef(0, (y4-y3)/2.0, 0);
            glRotatef(90.0, 0.0, -1.0, 0.0);
            glTranslatef(-x4, -z2, y4);
            glEnd();

        }
};

