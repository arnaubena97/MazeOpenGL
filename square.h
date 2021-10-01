#if  __linux__ 
    #include <GL/glut.h>
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#endif
#include<tuple> 
using namespace std;

class RGB{
    public:    
        float r,g,b;
        void setColor(float x, float y, float z){
            r=x;
            g=y;
            b=z;
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
            
        void Draw(int x, int y, int scale = 0){
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
        
    //private:
       
    
};

