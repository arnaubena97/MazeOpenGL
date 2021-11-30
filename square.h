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
#define SHOOTING 4


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define PI 3.1416 


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

            GLfloat material[4] = {1, 1, 1, 1};
            

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
  
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0);
            glBegin(GL_QUADS); // BACK toxos darrere
            glNormal3f(0,0,-1);
            glTexCoord2f(3,0); glVertex3i(x1,y,z);
            glTexCoord2f(0,0); glVertex3i(x,y,z);
            glTexCoord2f(0,1); glVertex3i(x,y1,z);
            glTexCoord2f(3,1); glVertex3i(x1,y1,z);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
  
             glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0); // FRONT toxos davant
            glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glTexCoord2f(3,0);glVertex3i(x1,y1,z1);
             glTexCoord2f(0,0);glVertex3i(x,y1,z1);
            glTexCoord2f(0,1);glVertex3i(x,y,z1);
            glTexCoord2f(3,1);glVertex3i(x1,y,z1);
            glEnd();
            glDisable(GL_TEXTURE_2D);

glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
  
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,2); //quadrat costat esquerra sense moure parquet To texture parket
            glBegin(GL_QUADS);
            glNormal3f(-1,0,0);
            glTexCoord2f(3,0);glVertex3i(x,y1,z1);
            glTexCoord2f(0,0);glVertex3i(x,y1,z);
            glTexCoord2f(0,1);glVertex3i(x,y,z);
            glTexCoord2f(3,1);glVertex3i(x,y,z1);
            glEnd();
            glDisable(GL_TEXTURE_2D);

glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
             glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,2); // LEFT //quadrat costat dret sense moure parquet To texture parket
            glBegin(GL_QUADS);
            glNormal3f(1,0,0);
            glTexCoord2f(3,0);glVertex3i(x1,y,z1);
            glTexCoord2f(0,0);glVertex3i(x1,y,z);
            glTexCoord2f(0,1);glVertex3i(x1,y1,z);
            glTexCoord2f(3,1);glVertex3i(x1,y1,z1);
            glEnd();
            glDisable(GL_TEXTURE_2D);
             // BOTTOM
            glColor3f(0.3,0.6,0.7);
            glBegin(GL_QUADS);
            glVertex3i(x,y,z1);
            glVertex3i(x,y,z);
            glVertex3i(x1,y,z);
            glVertex3i(x1,y,z1);
            glEnd();

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,1); // TOP
            glBegin(GL_QUADS);
            glNormal3f(0,1,0);
            glTexCoord2f(2,0);glVertex3i(x,y1,z);
            glTexCoord2f(0,0);glVertex3i(x,y1,z1);
            glTexCoord2f(0,1);glVertex3i(x1,y1,z1);
            glTexCoord2f(2,1);glVertex3i(x1,y1,z);
            // (x1, y1, z1 )- (x, y1, z) X (x1, y1, z) - (x, y1, z1)
            //((x1-x) , 0, (z1 -z)) X (x1 - x, 0 , (z-z1))
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
};

class Walls {   
    RGB color;
    float size_x, size_y, size_z; // size of square // size squares
    SquareWall *walls; // array of squares wall
    int num_walls; // num of squares wall
    int col, row;
    public: 
        Walls(int num){
            color.setColor(0.5,0.5,0.5);
            num_walls = num;
            walls = new SquareWall[num_walls];
            
        }
        //set positions of all walls
        void setPositions(Maze m){
            col = m.columns;
            row = m.rows;
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
            /*for(int i = 0; i< col; i++){
                for(int j = 0; j< row; j++){
                    GLfloat material[4] = {1, 1, 1, 1};
                    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
        
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D,3); 
                    glBegin(GL_QUADS);
                    //glNormal3f(0,1,0);
                    glTexCoord2f(30,0);glVertex3i(0,0,0);//vertex baix esquerra

                    //glNormal3f(0,1,0);
                    glTexCoord2f(0,0);glVertex3i(0,0,j*size_y);//vertex baix dreta

                    //glNormal3f(0,1,0);
                    glTexCoord2f(0,30);glVertex3i(i*size_x,0,j*size_y);//vertex dalt dreta

                    //glNormal3f(0,1,0);
                    glTexCoord2f(30,30);glVertex3i(i*size_x,0,0);//vertex dalt esquerra
                    glEnd();
                    glDisable(GL_TEXTURE_2D);
                }
            }*/
            
            GLfloat material[4] = {1, 1, 1, 1};
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
  
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
        Point position_shoot;
        float size_x, size_y, size_z; // size of square
        float vx,vy, vz, valpha; // Velocity vector
        float vsx, vsy;
        int state; 
        int direction; //1NORTH 2SOUTH 3WEST 4EAST
        long time_remaining, time_mov;
        int angle, teoric_angle;
        char symbol;
        bool flag = false;
        bool isDead = false;
        Tank(char sym){
            state = QUIET;
            time_mov= 300;
            direction = RIGHT;
            symbol = sym;
        }
        
        void setSizesXY(float x, float y, float z = 0){
            size_y = y;
            size_x = x;
            size_z = z;
        }
        //functions to set the position of square
        void setPosition(float x, float y, float z = 0){
            position = Point(x,y,z);
            position_shoot = Point(x,y,z);
        }
        void setPosition(Point pos){
            position = pos;
            position_shoot = pos;
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
            
            lightrotate();
            glEnable(GL_COLOR_MATERIAL);
            drawWheels(x0, x1, x3, x5, x6, y0, y1, y2, z1, z5, xpw1, xpw2, -0.3, 0);
            drawWheels(x0, x1, x3, x5, x6, y0, y1, y2, z6, z3, xpw1, xpw2, -0.5, 1);
            drawBody(x5, x6, y1, y3, z5, z6);
            drawCanon(x1, x2, y1, y4, z2, z5, z6);
            glDisable(GL_COLOR_MATERIAL);
            glRotatef(-angle, 0,-1,0);

            glPopMatrix();
            glPushMatrix();
            x2 = (position_shoot.x + 0.5)* size_x;
            y1 = (position_shoot.z + (1.0/6.0)) * size_z;
            y4 = (position_shoot.z + 1) * size_z;
            z2 = (position_shoot.y + 0.5) * size_y;
            drawShoot(x2, y1, y4, z2);
            glPopMatrix();


            //lighting();
        }

        void integrate(long t){
            if(state==MOVE && t<time_remaining){
                position.x = position.x + vx*t;
                position.y = position.y + vy*t;
                position.z = 0;
                position_shoot = position;
                time_remaining-=t;
                }
            else if(state==MOVE && t>=time_remaining){
                position.x = roundf(position.x + vx*time_remaining);
                position.y = roundf(position.y + vy*time_remaining);
                position.z = 0;
                position_shoot = position;
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
            else if(state==SHOOTING && t<time_remaining){
                position_shoot.x = position_shoot.x + vsx*t;
                position_shoot.y = position_shoot.y + vsy*t;
                position_shoot.z = 0;
                time_remaining-=t;
            }
            else if(state==SHOOTING && t>=time_remaining && !flag){
                position_shoot.x = roundf(position_shoot.x + vsx*time_remaining);
                position_shoot.y = roundf(position_shoot.y + vsy*time_remaining);
                position_shoot.z = 0;
                flag = true;
                

            }else if (state ==SHOOTING && t>=time_remaining && flag){
                position_shoot.x = position.x;
                position_shoot.y = position.y;
                position_shoot.z = 0;
                state=QUIET;
                flag = false;
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


        void shoot(int len){
            
            len > 3 ? len = 3 : len=len;// maxim distancia disparo 3
            //com girem el pla, nomes hem de augmentar x
            if(direction == LEFT){
                init_movement_shoot(position_shoot.x-len,position_shoot.y,time_mov);
            }
            else if(direction == RIGHT){
                init_movement_shoot(position_shoot.x+len,position_shoot.y,time_mov);
            }
            else if(direction == UP){
                init_movement_shoot(position_shoot.x,position_shoot.y-len,time_mov);
            }
            else if(direction == DOWN){
                init_movement_shoot(position_shoot.x,position_shoot.y+len,time_mov);
            }
        }
        void reset(Point p){
            position = p;
        }

        
        void lightingDirections(){

            //GLfloat direction_light[] = {position.x, position.y, -1};

            
            GLfloat direction_light[3];
            if(direction == UP){
           
                direction_light[0] = 0;
                direction_light[1] = 0;
                direction_light[2] = -1;
                

            }else if(direction == DOWN){
    
                direction_light[0] = 0;
                direction_light[1] = 0;
                direction_light[2] = 1;
                

            }else if(direction == RIGHT){
                
                direction_light[0] = 1;
                direction_light[1] = 0;
                direction_light[2] = 0;
                

            }else if(direction == LEFT){
                direction_light[0] = -1;
                direction_light[1] = 0;
                direction_light[2] = 0;
                
            }  
            GLenum light = GL_LIGHT1;  
            if(symbol == 'S'){
                light = GL_LIGHT1;
            }else {
                light = GL_LIGHT2;
            }

            GLfloat color_light[] = {1, 1, 1, 1};
            //GLfloat position_light[] = {position.x* size_x, position.z* size_z,position.y* size_y, 1};
            GLfloat position_light[] = {position.x*size_x+size_x/2 , size_z+1*2,size_y*position.y + size_y/2, 1};
            //GLfloat dir[] = {1,0,0};
            
            glLightfv(light, GL_POSITION, position_light);
            glLightfv(light,GL_DIFFUSE,color_light);
            glLightfv(light, GL_SPOT_DIRECTION, direction_light);
            glLightf(light,GL_CONSTANT_ATTENUATION,0.3);
            glLightf(light, GL_SPOT_EXPONENT, 10);
            glLightf(light, GL_SPOT_CUTOFF, 20.0);
            
            glEnable(light);
            cout<<symbol;
            cout<<"Tank Position: X = "<<position.x<<" Y = "<<position.y<<" Z = "<<position.z<<endl;
            cout<<"Light Position: X = "<<position_light[0]<<" Y = "<<position_light[1]<<" Z = "<<direction_light[2]<<endl; 
            cout<<"Light Direction: X = "<<direction_light[0]<<" Y = "<<direction_light[1]<<" Z = "<<direction_light[2]<<endl; 
            cout<<"----------------------------------------------------------------"<<endl;
            
        }
        void lightrotate(){

            GLenum light = GL_LIGHT1;  
            if(symbol == 'S'){
                light = GL_LIGHT1;
            }else {
                light = GL_LIGHT2;
            }

            GLfloat color_light[] = {1, 1, 1, 1};
            //GLfloat position_light[] = {position.x* size_x, position.z* size_z,position.y* size_y, 1};
            GLfloat position_light[] = {position.x*size_x+size_x/2 , size_z+1*2,size_y*position.y + size_y/2, 1};
            GLfloat dir[] = {1,0,0};
            
            glLightfv(light, GL_POSITION, position_light);
            glLightfv(light,GL_DIFFUSE,color_light);
            glLightfv(light, GL_SPOT_DIRECTION, dir);
            glLightf(light,GL_CONSTANT_ATTENUATION,0.3);
            glLightf(light, GL_SPOT_EXPONENT, 10);
            glLightf(light, GL_SPOT_CUTOFF, 23.0);
            glLightf(light, GL_LINEAR_ATTENUATION, 0.001f);
            glEnable(light);

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

        void init_movement_shoot(int destinations_x,int destinations_y,int duration){
            vsx = (destinations_x - position_shoot.x)/duration;
            vsy = (destinations_y - position_shoot.y)/duration;
            state=SHOOTING;
            time_remaining=duration;
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

        void drawShoot(GLfloat x2,GLfloat y1, GLfloat y4, GLfloat z2){
            GLUquadricObj *p = gluNewQuadric();
            glTranslatef(+x2, +y4 - (y1/2)*1.05, +z2);
            glColor3f(1,1,1);
            gluSphere(p, (y1/2)*0.95, 500, 500);
            glTranslatef(-x2, -y4+ (y1/2)*1.05,  -z2);
            glEnd();
        }
};

