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
#define ROTATE 3

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4


class SquareWall {     
    public:       
        Point position; // Current position
        //RGB color; 
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

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0);
            glBegin(GL_QUADS); // BACK
            glTexCoord2f(3,0); glVertex3i(x1,y,z);
            glTexCoord2f(0,0); glVertex3i(x,y,z);
            glTexCoord2f(0,1); glVertex3i(x,y1,z);
            glTexCoord2f(3,1); glVertex3i(x1,y1,z);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glColor3f(1,0,0); // FRONT
            glBegin(GL_QUADS);
            glVertex3i(x1,y1,z1);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y,z1);
            glVertex3i(x1,y,z1);
            glEnd();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,2); //RIGHT To texture
            glBegin(GL_QUADS);
            glTexCoord2f(3,0);glVertex3i(x,y1,z1);
            glTexCoord2f(0,0);glVertex3i(x,y1,z);
            glTexCoord2f(0,1);glVertex3i(x,y,z);
            glTexCoord2f(3,1);glVertex3i(x,y,z1);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glColor3f(0,0,1); // LEFT
            glBegin(GL_QUADS);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x1,y1,z1);
            glEnd();

             // BOTTOM
            glColor3f(0.3,0.6,0.7);
            glBegin(GL_QUADS);
            glVertex3i(x,y,z1);
            glVertex3i(x,y,z);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y,z1);
            glEnd();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,1); // TOP
            glBegin(GL_QUADS);
            glTexCoord2f(2,0);glVertex3i(x,y1,z);
            glTexCoord2f(0,0);glVertex3i(x,y1,z1);
            glTexCoord2f(0,1);glVertex3i(x1,y1,z1);
            glTexCoord2f(2,1);glVertex3i(x1,y1,z);
            glEnd();
            glDisable(GL_TEXTURE_2D);
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
                        //walls[cnt].color = color;
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


            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,3); 
            glBegin(GL_QUADS);
            glTexCoord2f(30,0);glVertex3i(0,0,0);//vertex baix esquerra
            glTexCoord2f(0,0);glVertex3i(0,0,y);//vertex baix dreta
            glTexCoord2f(0,30);glVertex3i(x,0,y);//vertex dalt dreta
            glTexCoord2f(30,30);glVertex3i(x,0,0);//vertex dalt esquerra
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
};


class Tank{     
    public:       
        Point position; // Current position
        float size_x, size_y, size_z; // size of square
        float vx,vy, vz, valpha; // Velocity vector
        int state; 
        int direction; //1NORTH 2SOUTH 3WEST 4EAST
        long time_remaining, time_mov;
        int angle, teoric_angle;

        Tank(){
            state = QUIET;
            time_mov= 300;
            direction = RIGHT;
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
            GLfloat x1 = (position.x + 0.25) * size_x;
            GLfloat x2 = (position.x + 0.5) * size_x;
            GLfloat x3 = (position.x + 0.75)* size_x;
            GLfloat x4 = (position.x + 1) * size_x;
            GLfloat x5 = (position.x + 0.375) * size_x;
            GLfloat x6 = (position.x + 0.625) * size_x;

            GLfloat xpw1 = (position.x + 0.4385) * size_x;
            GLfloat xpw2 = (position.x + 0.5625) * size_x;

            
            GLfloat z0 = position.y * size_y;
            GLfloat z1 = (position.y + 0.25) * size_y;
            GLfloat z2 = (position.y + 0.5) * size_y;
            GLfloat z3 = (position.y + 0.75)* size_y;
            GLfloat z4 = (position.y + 1) * size_y;
            GLfloat z5 = (position.y + 0.375) * size_y;
            GLfloat z6 = (position.y + 0.625) * size_y;

            GLfloat y0 = position.z * size_z ;
            GLfloat y1 = (position.z + (1.0/6.0)) * size_z;
            GLfloat y2 = (position.z + (1.0/3.0)) * size_z;
            GLfloat y3 = (position.z + (4.0/6.0)) * size_z;
            GLfloat y4 = (position.z + 1) * size_z;

            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_BACK,GL_LINE);
            glPushMatrix();
            glTranslatef(x2, y0, z2);
            //glTranslatef((x0 + size_x/2), y0, (z0 + size_z/2));
            glRotatef(angle, 0,1,0);
            glTranslatef(-x2, y0, -z2);
            //glTranslatef(-(x0 + size_x/2), -y0, -(z0 + size_z/2));

            drawWheels(x0, x1, x3, x5, x6, y0, y1, y2, z1, z5, xpw1, xpw2, -0.3, 0);
            drawWheels(x0, x1, x3, x5, x6, y0, y1, y2, z6, z3, xpw1, xpw2, -0.5, 1);
            drawBody(x5, x6, y1, y3, z5, z6);
            drawCanon(x1, x2, y1, y4, z2, z5, z6);
            glRotatef(-angle, 0,-1,0);
            glPopMatrix();
            
        }

        void integrate(long t){
            if(state==MOVE && t<time_remaining){
                position.x = position.x + vx*t;
                position.y = position.y + vy*t;
                position.z = 0;
                time_remaining-=t;
                }
            else if(state==MOVE && t>=time_remaining){
                position.x = roundf(position.x + vx*time_remaining);
                position.y = roundf(position.y + vy*time_remaining);
                position.z = 0;
                state=QUIET;
            }
            else if(state==ROTATE && t<time_remaining){
                //printf("state: %d, angle: %d , time: %d\n", state, angle, t);
                angle = angle + valpha*t;
                time_remaining-=t;
            }
            else if(state==ROTATE && t>=time_remaining){
                angle = teoric_angle;
                state=QUIET;
            }

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

        

        void rotateLeft(){
            if(direction == UP){
                init_movement_angle(angle + 90, time_mov);
                teoric_angle += 90;
            }
            else if(direction == DOWN){
                init_movement_angle(angle - 90, time_mov);
                teoric_angle -= 90;
            }
            else{
                init_movement_angle(angle + 180, time_mov);
                teoric_angle += 180;
            }
        }
        void rotateRight(){
            if(direction == UP){
                init_movement_angle(angle - 90, time_mov);
                teoric_angle -= 90;
            }
            else if(direction == DOWN){
                init_movement_angle(angle + 90, time_mov);
                teoric_angle += 90;
            }
            else{
                init_movement_angle(angle - 180, time_mov);
                teoric_angle -= 180;
            }
        }
        void rotateUp(){
            if(direction == RIGHT){
                init_movement_angle(angle + 90, time_mov);
                teoric_angle += 90;
            }
            else if(direction == LEFT){
                init_movement_angle(angle - 90, time_mov);
                teoric_angle -= 90;
            }
            else{
                init_movement_angle(angle + 180, time_mov);
                teoric_angle += 180;
            }
        }
        void rotateDown(){
            if(direction == RIGHT){
                init_movement_angle(angle - 90, time_mov);
                teoric_angle -= 90;
            }
            else if(direction == LEFT){
                init_movement_angle(angle + 90, time_mov);
                teoric_angle += 90;
            }
            else{
                init_movement_angle(angle - 180, time_mov);
                teoric_angle -= 180;
            }
        }

    private:
        void init_movement(int destination_x,int destination_y,int duration){
            vx = (destination_x - position.x)/duration;
            vy = (destination_y - position.y)/duration;
            state=MOVE;
            time_remaining=duration;
        }
        
        void init_movement_angle(int destination, int duration){
            valpha = (float)(destination - angle)/duration;
            state=ROTATE;
            time_remaining=duration;
            //printf("time: %ld, valpa: %f \n", time_remaining ,valpha);
        }


        void drawWheels( GLfloat x0,  GLfloat x1,  GLfloat x3,  GLfloat x5,  GLfloat x6,  GLfloat y0, GLfloat y1, GLfloat y2,  GLfloat z1, GLfloat z5, GLfloat xpw1, GLfloat xpw2, float wheel, bool side){

            glColor3f(1,0,0);
            glBegin(GL_QUADS); // xapa1 fora
            glVertex3i(x1,y2,z1);
            glVertex3i(x5,y0,z1);
            glVertex3i(x5,y0,z5);
            glVertex3i(x1,y2,z5);
            glEnd();

            glColor3f(0,1,0);
            glBegin(GL_QUADS); // xapa2 fora
            glVertex3i(x3,y2,z1);
            glVertex3i(x1,y2,z1);
            glVertex3i(x1,y2,z5);
            glVertex3i(x3,y2,z5);
            glEnd();

            glColor3f(0,0,1);
            glBegin(GL_QUADS); // xapa3 fora
            glVertex3i(x5,y0,z1);
            glVertex3i(x6,y0,z1);
            glVertex3i(x6,y0,z5);
            glVertex3i(x5,y0,z5);
            glEnd();
            
            glColor3f(1,1,0);
            glBegin(GL_QUADS); // xapa4 fora
            glVertex3i(x6,y0,z1);
            glVertex3i(x3,y2,z1);
            glVertex3i(x3,y2,z5);
            glVertex3i(x6,y0,z5);
            glEnd();

            
            glColor3f(0.4,0.2,0.8);
            glBegin(GL_QUADS); // xapa5 fora
            glVertex3i(x1,y2,z5);
            glVertex3i(x5,y0,z5);
            glVertex3i(x6,y0,z5);
            glVertex3i(x3,y2,z5);
            glEnd();


            glColor3f(1,0,1);
            glBegin(GL_QUADS); // xapa6 exterior
            glVertex3i(x5,y0,z1);
            glVertex3i(x1,y2,z1);
            glVertex3i(x3,y2,z1);
            glVertex3i(x6,y0,z1);
            glEnd();
            if(side == 0){
            GLUquadricObj *p = gluNewQuadric();
                glTranslatef(xpw1, y1, z1 + wheel);
                glColor3f(0,0.2,0);
                gluDisk(p, 0,y1/2, 100, 100);
                glTranslatef(-xpw1, 0, 0);
                glTranslatef(xpw2, 0, 0);
                glColor3f(0,0.2,0);
                gluDisk(p, 0,y1/2, 100, 100);
                glTranslatef(-xpw2, -y1, -z1 + wheel);
                glEnd();
            }else{
                GLUquadricObj *p = gluNewQuadric();
                glTranslatef(xpw1, y1, z5 + wheel);
                glColor3f(0,0.2,0);
                gluDisk(p, 0,y1/2, 100, 100);
                glTranslatef(-xpw1, 0, 0);
                glTranslatef(xpw2, 0, 0);
                glColor3f(0,0.2,0);
                gluDisk(p, 0,y1/2, 100, 100);
                glTranslatef(-xpw2, -y1, -(z5 + wheel));
                glEnd();
            }

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

        void drawCanon(GLfloat x1,GLfloat x2, GLfloat y1, GLfloat y4,
                     GLfloat z2, GLfloat z5, GLfloat z6){
            GLfloat zr = (z6-z5)/3.0;
            GLUquadricObj *p = gluNewQuadric();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glTranslatef(x2, z2, -y4);
            glColor3f(1,0.8,0);
            gluCylinder(p, zr, zr, 2*y1, 500, 500);
            glColor3f(0.2,0.5,1);
            gluDisk(p, 0,zr, 100, 100);
            glRotatef(90.0, -1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glColor3f(0.1,0.5,0.1);
            glTranslatef(0, -(y1)/2.0, 0);
            gluCylinder(p, (y1/2.0), (y1/2.0), x2-x1, 500, 500);
            glTranslatef(0, (y1)/2.0, 0);
            glRotatef(90.0, 0.0, -1.0, 0.0);
            glTranslatef(-x2, -z2, y4);
            glEnd();

        }

};

