#include <iostream>
#include <stack>
#include <vector>
#include <random>
using namespace std;

class Maze {     
    public:       
        char **board;      

        int columns;
        int rows;

        Maze(int med_columns1, int med_rows1)
        {
            med_columns = med_columns1;
            med_rows = med_rows1;
            columns = med_rows1 * 2 + 1;
            rows = med_rows1 * 2 + 1;
            ReserveMemory();
            PutExteriorWalls();
            PutInteriorWalls();
            PutBeginEnd();
        }
        
        void Display() {
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                        cout << board[i][j] << " ";
                }
                cout << endl;
            }
        }   
    
        int getNumWalls(){
            int cnt = 0;
            for (int i = 0; i < columns ; i++) {
                for (int j = 0; j < rows; j++) {
                    if (board[i][j] =='#'){
                        cnt++;
                    }  
                }
            }
            printf("NUMWALLS %d\n", cnt);
            return cnt;
        }
    private:
        int med_columns;
        int med_rows;
        void PutBeginEnd(){
            board[1][1] = 'S'; // coloquem la sortida S
            board[columns-2][rows-2] = 'E'; // coloquem l'entrada E
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
};