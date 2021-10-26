#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <tuple> 
using namespace std;

#define QUIET 2

class Maze {     
    public:       
        char **board;  // board of maze    

        int columns; // num columns
        int rows; // num rows
        char agent1, agent2; // chars of the agents
        Point startPosition, endPosition;
        Maze(int med_columns1, int med_rows1)
        {
            agent1 = 'S';
            agent2 = 'E';

            med_columns = med_columns1;
            med_rows = med_rows1;
            columns = med_columns1 * 2 + 1;
            rows = med_rows1 * 2 + 1;
            startPosition = Point(1.0, 1.0);
            endPosition= Point(columns - 2,rows -2);
            ReserveMemory();
            PutExteriorWalls();
            PutInteriorWalls();
            //noWalls();
            PutBeginEnd();
        }
        
        // function to print maze on terminal
        void display() {
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                        cout << board[i][j] << " ";
                }
                cout << endl;
            }
        }   
    
        // return the number of walls of maze
        int getNumWalls(){
            int cnt = 0;
            for (int i = 0; i < columns ; i++) {
                for (int j = 0; j < rows; j++) {
                    if (board[i][j] =='#'){
                        cnt++;
                    }  
                }
            }
            return cnt;
        }
        // functions to get the start point of agents
        Point getStartPoint(){
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    if (board[i][j] ==agent1){
                        return Point(i, j,0);
                    }
                }
            }
            return Point(0,0,0);
        }
        Point getEndPoint(){
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    if (board[i][j] ==agent2){
                        return Point(i, j, 0);
                    }
                }
            }
            return Point(0,0);
        }
        
        // functions to know if movement is possible
        bool canMoveUp(Point p, char c){
            /* s'ha de pensar que el board el tenim girat respecte al dibuix grafic*/
            return board[(int)p.x][(int)p.y+1] != '#' && board[(int)p.x][(int)p.y+1] != c;
        }
        bool canMoveDown(Point p, char c){
            return board[(int)p.x][(int)p.y-1] != '#' && board[(int)p.x][(int)p.y-1] != c;
        }
        bool canMoveLeft(Point p, char c){
            return board[(int)p.x-1][(int)p.y] != '#' && board[(int)p.x-1][(int)p.y] != c;
        }
        bool canMoveRight(Point p, char c){
            return board[(int)p.x+1][(int)p.y] != '#' && board[(int)p.x+1][(int)p.y] != c;
        }
        
        // functions to update the board of maze
        void updateUp(Point p, char c){
            //display();
            board[(int)p.x][(int)p.y] = ' ';
            board[(int)p.x][(int)p.y+1] = c;
        }
        void updateDown(Point p, char c){
            //display();
            board[(int)p.x][(int)p.y] = ' ';
            board[(int)p.x][(int)p.y-1] = c;
        }
        void updateLeft(Point p, char c){
            //display();
            board[(int)p.x][(int)p.y] = ' ';
            board[(int)p.x-1][(int)p.y] = c;
        }
        void updateRight(Point p, char c){
            //display();
            board[(int)p.x][(int)p.y] = ' ';
            board[(int)p.x+1][(int)p.y] = c;
        }

        int checkEnd(int state1, int state2){
            if(state1 == QUIET){
                if(endPosition.Equal(getStartPoint()) ) return 1;
            }
            if(state2 == QUIET){
                if(startPosition.Equal(getEndPoint()) ) return 2;
            }
            return 0;
        }
    private:
        int med_columns;
        int med_rows;
        void PutBeginEnd(){
            board[(int)startPosition.x][(int)startPosition.y] = agent1; // coloquem la sortida S
            board[(int)endPosition.x][(int)endPosition.y] = agent2; // coloquem l'entrada E
        }

        void ReserveMemory(){
            board = new char* [columns]; // Mida del laberint amb M columnes

            for (int i = 0; i < columns; i++) {
                board[i] = new char [rows]; // Mida del laberint amb N columnes
            }
        }
        
        void PutExteriorWalls(){
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    (!(i&1)||!(j&1)) ? board[i][j] = '#': board[i][j] = ' ';
                }
            }
            for (int i = 1; i < columns; i+=2) {
                for (int j = 1; j < rows; j+=2) {
                    board[i][j] = ' ';
                }
            }
        }

        void PutInteriorWalls() {

            vector < pair<int, pair<int, int> > > cell_list;
            vector <bool> visited(med_columns * med_rows, false);
            stack<pair<int, pair<int, int> > > m_stack;
            random_device rdev;
            mt19937 rng(rdev());
            uniform_int_distribution<mt19937::result_type> dist100(1, 100);

            int nVisited = 0;
            int k = 0;

            for (int i = 1; i < columns; i+=2) {
                for (int j = 1; j < rows; j+=2) {
                    cell_list.push_back(make_pair(k, make_pair(i, j)));
                    k++;
                }
            }

            int randIdx = dist100(rng) % med_columns * med_rows;
            m_stack.push(cell_list[randIdx]);
            visited[randIdx] = true;
            nVisited++;

            while(nVisited < med_columns * med_rows) {

                vector <int> neighbours;
                // North
                if (m_stack.top().second.first > 1) {
                    if (board[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                        !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
                        neighbours.push_back(0);
                    }
                }
                // East
                if (m_stack.top().second.second < rows - 2) {
                    if (board[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                        !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
                        neighbours.push_back(1);
                    }
                }
                // South
                if (m_stack.top().second.first < columns - 2) {
                    if (board[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
                        !visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]) {
                        neighbours.push_back(2);
                    }
                }
                // West
                if (m_stack.top().second.second > 1) {
                    if (board[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
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
                            board[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = ' ';
                            m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]);
                            break;
                        case 1: // East
                            board[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = ' ';
                            m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]);
                            break;
                        case 2: // South
                            board[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = ' ';
                            m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]);
                            break;
                        case 3: // West
                            board[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = ' ';
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
        
        int getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list) {
            for (int i = 0; i < cell_list.size(); i++)
            {
                if (cell_list[i].second.first == x && cell_list[i].second.second == y)
                    return cell_list[i].first;
            }
            cout << "getIdx() couldn't find the index!" << endl;
        return -1;
    }
        
        void noWalls(){
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    (i == 0 || j==0 || i == columns-1 || j == rows-1) ? board[i][j] = '#': board[i][j] = ' ';
                }
            }
        }
};