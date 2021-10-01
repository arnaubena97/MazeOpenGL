#if  __linux__ 
    #include <GL/glut.h>
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#endif
#include<tuple> 
using namespace std;


#define MOVE 1
#define QUIET 2

class RGB{
    public:    
        float r,g,b;
        void setColor(float x, float y, float z){
            r=x;
            g=y;
            b=z;
        }
        void setColor(RGB color){
            r=color.r;
            g=color.g;
            b=color.b;
        }
};

class Square {     
    public:       
        /*EXEMPLE*/
        float x,y;   //-- Current position
        float vx,vy; //-- Velocity vector
        int state;
        long time_remaining;

        /*UTILS*/
        RGB color;
        float size_x, size_y;
        Square(){
            state = QUIET;
        }
         /*   
        void draw(int x, int y, int scale = 0){
            this -> x = x * size_x;
            this -> y = y * size_y;
            glColor3f(color.r,color.g,color.b);
            glBegin(GL_QUADS);
            glVertex2i(x * size_x + scale ,y*size_y + scale);//vertex baix esquerra
            glVertex2i((x+1)* size_x -scale,y*size_y+scale);//vertex baix dreta
            glVertex2i((x+1) * size_x -scale ,(y+1)*size_y-scale);//vertex dalt dreta
            glVertex2i(x * size_x +scale,(y+1)*size_y-scale);//vertex dalt esquerra
            glEnd();
        }*/
        void draw(int scale =0){
            //printf("x:%d y:%d sizex:%f sizey:%f \n", x, y , size_x, size_y);
            glColor3f(color.r,color.g,color.b);
            glBegin(GL_QUADS);
            glVertex2i(x * size_x + scale ,y*size_y + scale);//vertex baix esquerra
            glVertex2i((x+1)* size_x -scale,y*size_y+scale);//vertex baix dreta
            glVertex2i((x+1) * size_x -scale ,(y+1)*size_y-scale);//vertex dalt dreta
            glVertex2i(x * size_x +scale,(y+1)*size_y-scale);//vertex dalt esquerra
            glEnd();
        }
        void setSizesXY(float x, float y){
            size_y = y;
            size_x = x;
        }
        void changeState(){
            state==QUIET ? state = MOVE : state= QUIET;
        }
        void init_movement(int destination_x,int destination_y,int duration){
            vx = (destination_x - x)/duration;
            vy = (destination_y - y)/duration;

            changeState();
            time_remaining=duration;
        }
        void integrate(long t){
            if(state==MOVE && t<time_remaining)
                {
                x = x + vx*t;
                y = y + vy*t;
                time_remaining-=t;
                }
            else if(state==MOVE && t>=time_remaining)
                {
                x = x + vx*time_remaining;
                y = y + vy*time_remaining;
                state=QUIET;
                }
        }
        
        void setPosition(float x, float y){
            this -> x = x;
            this -> y = y;
        }
        void moveUp(){

        }
        void moveDown(){}
        void moveLeft(){}
        void moveRight(){}
        void printSizes(){
            printf("x:%f y:%f sx:%f sy:%f\n", x, y, size_x, size_y);
        }
    //private:
       
};

class SquaresWall {   
    float x,y;   //-- Current position  
    RGB color;
    float size_x, size_y;
    Square *walls; 
    int num_walls;
    public: 
        SquaresWall(int num){
            color.setColor(0.5,0.5,0.5);
            num_walls = num;
            walls = new Square[num_walls];
        }
        void reserveMemory(int number){
            walls = new Square[number];
            color.setColor(0.5,0.5,0.5);
        }
        void setPositions(Maze m){
            int cnt =0;
            for (int i = 0; i < m.columns ; i++) {
                for (int j = 0; j < m.rows; j++) {
                    if (m.board[i][j] =='#'){
                        //printf("x:%d   y: %d  cnt: %d\n", i , j, cnt);
                        walls[cnt].setPosition(i, j);
                        //printf("%f , %f \n", walls[cnt].x, walls[cnt].y);
                        walls[cnt].color = color;
                        printf("%f   %f   ", size_x, size_y);
                        walls[cnt].setSizesXY(size_x,size_y);
                        walls[cnt].printSizes();
                        cnt++;
                    }
                }
            }
        }
        void draw(int scale =0){
            for(int i = 0; i< num_walls; i++){
                walls[i].draw(0);
            }
        }
        void setSizesXY(float x, float y){
            size_y = y;
            size_x = x;
            printf("%f   %f   ", size_x, size_y);
        }
};
