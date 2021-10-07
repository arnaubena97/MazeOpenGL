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

class Walls {   
    RGB color;
    float size_x, size_y; // size squares
    Square *walls; // array of squares wall
    int num_walls; // num of squares wall
    public: 
        Walls(int num){
            color.setColor(0.5,0.5,0.5);
            num_walls = num;
            walls = new Square[num_walls];
        }
        //set positions of all walls
        void setPositions(Maze m){
            int cnt =0;
            for (int i = 0; i < m.columns ; i++) {
                for (int j = 0; j < m.rows; j++) {
                    if (m.board[i][j] =='#'){
                        //printf("x:%d   y: %d  cnt: %d\n", i , j, cnt);
                        walls[cnt].setPosition(i, j);
                        //printf("%f , %f \n", walls[cnt].x, walls[cnt].y);
                        walls[cnt].color = color;
                        walls[cnt].setSizesXY(size_x,size_y);
                        cnt++;
                    }
                }
            }
        }
        // draw all squares wall
        void draw(int scale =0){
            for(int i = 0; i< num_walls; i++){
                walls[i].draw(0);
            }
        }
        //set sizes of squares wall
        void setSizesXY(float x, float y){
            size_y = y;
            size_x = x;
            
        }
};

