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

class Point{
    public:
    float x, y, z;
    Point(){
        this ->x = 0;
        this ->y = 0;
        this ->z = 0;
    }
    Point(float x , float y , float z = 0){
        this ->x = x;
        this ->y = y;
        this ->z = z;
    }
};