/********************************************************
 *
 * Authors: Arnau Benavides, Marc Felip
 *
*********************************************************/


// Includes depends OS
#if  __linux__ 
    #include <GL/glut.h>
#elif __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#endif

#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <math.h>  
using namespace std;

//-----------------------------------------------
//              GLOBAL VARIABLES
//-----------------------------------------------

#define MED_COLUMNS 20
#define MED_ROWS 20
#define SIZE_SQUARE_SMALL 0 // quant mes petit
#define WIDTH 500
#define HEIGHT 500


int COLUMNS = MED_COLUMNS * 2 + 1;
int ROWS = MED_ROWS * 2 + 1;

float SIZE_SQUARE_W = (float)WIDTH/(float)COLUMNS;
float SIZE_SQUARE_H =(float)HEIGHT/(float)ROWS;

//int keyflag=0;

//-----------------------------------------------
//                FUNCTIONS
//-----------------------------------------------

void display();;
void keyboard(unsigned char c,int x,int y);


//-----------------------------------------------
//             MAIN PROCEDURE
//-----------------------------------------------

int main(int argc,char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(700, 400);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Maze board");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

  glutMainLoop();
  return 0;
}

//-----------------------------------------------
//           DISPLAY MAZE TERMINAL
//-----------------------------------------------

void displayMaze(int M, int N, char** maze) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
                cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

// A utility function to get the index of cell with indices x, y;
int getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list) {
    for (int i = 0; i < cell_list.size(); i++)
    {
        if (cell_list[i].second.first == x && cell_list[i].second.second == y)
            return cell_list[i].first;
    }
    cout << "getIdx() couldn't find the index!" << endl;
    return -1;
}

void createMaze(char** maze) {

    vector < pair<int, pair<int, int> > > cell_list;
    vector <bool> visited(MED_COLUMNS * MED_ROWS, false);
    stack<pair<int, pair<int, int> > > m_stack;
    random_device rdev;
    mt19937 rng(rdev());
    uniform_int_distribution<mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < COLUMNS; i+=2) {
        for (int j = 1; j < ROWS; j+=2) {
            cell_list.push_back(make_pair(k, make_pair(i, j)));
            k++;
        }
    }

    int randIdx = dist100(rng) % MED_COLUMNS * MED_ROWS;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    while(nVisited < MED_COLUMNS * MED_ROWS) {

        vector <int> neighbours;
        // North
        if (m_stack.top().second.first > 1) {
            if (maze[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < ROWS - 2) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < COLUMNS - 2) {
            if (maze[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(2);
            }
        }
        // West
        if (m_stack.top().second.second > 1) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]) {
                neighbours.push_back(3);
            }
        }
        // Neighbours available?
        if (!neighbours.empty()) {
            // Choose a random direction
            int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
            // Create a path between this cell and neighbour
            switch (next_cell_dir) {
                case 0: // North
                    maze[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 1: // East
                    maze[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]);
                    break;
                case 2: // South
                    maze[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 3: // West
                    maze[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]);
                    break;
            }

            visited[m_stack.top().first] = true;
            nVisited++;
        }
        else {
            m_stack.pop();
        }
    }
}

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

//-----------------------------------------------
    // Crear el tauler del laberint
//-----------------------------------------------
    char **maze;
    maze = new char* [COLUMNS]; // Mida del laberint amb M columnes
    
    for (int i = 0; i < COLUMNS; i++) {
        maze[i] = new char [ROWS]; // Mida del laberint amb N columnes
    }
//-----------------------------------------------
    //generar les parets del labeirnt
//-----------------------------------------------
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (!(i&1)||!(j&1)){
                maze[i][j] = '#';
            }
            else
                maze[i][j] = ' ';
        }
    }
//-----------------------------------------------
    //generar els espais buits a l'interior
//-----------------------------------------------
    for (int i = 1; i < COLUMNS; i+=2) {
        for (int j = 1; j < ROWS; j+=2) {
            maze[i][j] = ' ';
        }
    }
    
    createMaze(maze); // omple les parets del laberint
    maze[1][1] = 'S'; // coloquem la sortida S
    maze[COLUMNS-2][ROWS-2] = 'E'; // coloquem l'entrada E
    
    //displayMaze(COLUMNS, ROWS, maze); // imprimim per pantalla el laberint
//-----------------------------------------------
//           DIBUIXAR LABERINT
//-----------------------------------------------
   for (int i = 0; i < COLUMNS ; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (maze[i][j] =='#'){

                glColor3f(0.5,0.5,0.5);
                glBegin(GL_QUADS);

                glVertex2i(i*SIZE_SQUARE_W,j*SIZE_SQUARE_H); //vertex baix esquerra
                glVertex2i((i+1)*SIZE_SQUARE_W,j*SIZE_SQUARE_H); //vertex baix dreta
                glVertex2i((i+1)*SIZE_SQUARE_W,(j+1)*SIZE_SQUARE_H); //vertex dalt dreta
                glVertex2i(i*SIZE_SQUARE_W,(j+1)*SIZE_SQUARE_H);  //vertex dalt esquerra
                glEnd();     

            }  
            else if (maze[i][j] =='S'){

                glColor3f(0.8,0.3,0.5);
                glBegin(GL_QUADS);

                glVertex2i(i*SIZE_SQUARE_W+SIZE_SQUARE_SMALL,j*SIZE_SQUARE_H+SIZE_SQUARE_SMALL); //vertex baix esquerra
                glVertex2i((i+1)*SIZE_SQUARE_W-SIZE_SQUARE_SMALL,j*SIZE_SQUARE_H+SIZE_SQUARE_SMALL); //vertex baix dreta
                glVertex2i((i+1)*SIZE_SQUARE_W-SIZE_SQUARE_SMALL,(j+1)*SIZE_SQUARE_H-SIZE_SQUARE_SMALL); //vertex dalt dreta
                glVertex2i(i*SIZE_SQUARE_W+SIZE_SQUARE_SMALL,(j+1)*SIZE_SQUARE_H-SIZE_SQUARE_SMALL);  //vertex dalt esquerra
                glEnd();  

            } else if (maze[i][j] =='E'){

                glColor3f(0.8,0.2,0.8);
                glBegin(GL_QUADS);

                glVertex2i(i*SIZE_SQUARE_W+SIZE_SQUARE_SMALL,j*SIZE_SQUARE_H+SIZE_SQUARE_SMALL); //vertex baix esquerra
                glVertex2i((i+1)*SIZE_SQUARE_W-SIZE_SQUARE_SMALL,j*SIZE_SQUARE_H+SIZE_SQUARE_SMALL); //vertex baix dreta
                glVertex2i((i+1)*SIZE_SQUARE_W-SIZE_SQUARE_SMALL,(j+1)*SIZE_SQUARE_H-SIZE_SQUARE_SMALL); //vertex dalt dreta
                glVertex2i(i*SIZE_SQUARE_W+SIZE_SQUARE_SMALL,(j+1)*SIZE_SQUARE_H-SIZE_SQUARE_SMALL);  //vertex dalt esquerra
                glEnd();     
            }
        }
    }
    glutSwapBuffers();
    
}

//-----------------------------------------------
//-----------------------------------------------

void keyboard(unsigned char c,int x,int y){
    exit(0); // Qualsevol pulsacio del teclat tanca el laberint
};
