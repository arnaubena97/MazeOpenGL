/********************************************************
 *
 * Author: Akshat Malviya (@akshat157)
 * Date: Sunday, June 07
 * Desc: A random maze generator using backtracking!
 *
*********************************************************/

#include <iostream>
#include <stack>
#include <vector>
#include <random>
using namespace std;

// Default values
int m = 4, n = 4;

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

void createMaze(int M, int N, char** maze) {

    vector < pair<int, pair<int, int> > > cell_list;
    vector <bool> visited(m*n, false);
    stack<pair<int, pair<int, int> > > m_stack;
    random_device rdev;
    mt19937 rng(rdev());
    uniform_int_distribution<mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < M; i+=2) {
        for (int j = 1; j < N; j+=2) {
            cell_list.push_back(make_pair(k, make_pair(i, j)));
            k++;
        }
    }

    int randIdx = dist100(rng) % m*n;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    // Algo
    while(nVisited < m*n) {

        vector <int> neighbours;
        // North
        if (m_stack.top().second.first > 1) {
            if (maze[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < N - 2) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < M - 2) {
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

int main(int argc, char const *argv[]) {
    m = 9;
    n = 9;
    int M = 2*m+1;
    int N = 2*n+1;
    //###############################################
    // Crear el tauler del laberint
    //###############################################
    char **maze;
    maze = new char* [M]; // Mida del laberint amb M columnes
    
    for (int i = 0; i < M; i++) {
        maze[i] = new char [N]; // Mida del laberint amb N columnes
    }
    //###############################################
    //generar les parets del labeirnt
    //###############################################
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (!(i&1)||!(j&1))
                maze[i][j] = '#';
            else
                maze[i][j] = ' ';

        }
    }
    //###############################################
    //generar els espais buits a l'interior
    //###############################################
    for (int i = 1; i < M; i+=2) {
        for (int j = 1; j < N; j+=2) {
            maze[i][j] = ' ';
        }

    }
    
    createMaze(M, N, maze); // omple les parets del laberint
    maze[1][1] = 'S'; // coloquem la sortida S
    maze[2*m-1][2*n-1] = 'E'; // coloquem l'entrada E
    displayMaze(M, N, maze); // imprimim per pantalla el laberint
    
    return 0;
}
