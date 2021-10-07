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
            GLfloat y = position.y * size_y;
            GLfloat y1 =(position.y + 1) * size_y;
            GLfloat z = position.z * size_z;
            GLfloat z1 = (position.z + 1) * size_z;
            
            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_BACK,GL_LINE);


            glColor3f(color.r,color.g,color.b);
            glBegin(GL_QUADS); // BACK
            glVertex3i(x,y,z);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x,y1,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // FRONT
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y,z1);
            glVertex3i(x,y,z1);
            glEnd();

            glColor3f(color.r,color.g,color.b); //RIGHT
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y,z1);
            glVertex3i(x,y,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // LEFT
            glBegin(GL_QUADS);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y1,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // TOP
            glBegin(GL_QUADS);
            glVertex3i(x,y,z);
            glVertex3i(x,y,z1);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // BOTTOM
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y1,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x1,y1,z1);
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
            GLfloat x = position.x * size_x;
            GLfloat x1 = (position.x + 1) * size_x;
            GLfloat y = position.y * size_y;
            GLfloat y1 =(position.y + 1) * size_y;
            GLfloat z = position.z * size_z;
            GLfloat z1 = (position.z + 1) * size_z;
            
            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_BACK,GL_LINE);


            glColor3f(color.r,color.g,color.b);
            glBegin(GL_QUADS); // BACK
            glVertex3i(x,y,z);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x,y1,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // FRONT
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y,z1);
            glVertex3i(x,y,z1);
            glEnd();

            glColor3f(color.r,color.g,color.b); //RIGHT
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y,z1);
            glVertex3i(x,y,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // LEFT
            glBegin(GL_QUADS);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y1,z1);
            glVertex3i(x1,y1,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // TOP
            glBegin(GL_QUADS);
            glVertex3i(x,y,z);
            glVertex3i(x,y,z1);
            glVertex3i(x1,y,z1);
            glVertex3i(x1,y,z);
            glEnd();

            glColor3f(color.r,color.g,color.b); // BOTTOM
            glBegin(GL_QUADS);
            glVertex3i(x,y1,z1);
            glVertex3i(x,y1,z);
            glVertex3i(x1,y1,z);
            glVertex3i(x1,y1,z1);
            glEnd();
        }
};

