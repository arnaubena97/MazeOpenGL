#include <iostream>
//#include <conio.h>
using namespace std;
class dfs{
	public:
		int M;
		int N ;
		char ** maze;
		bool ** visited;
		bool ** path;
		stack<char> directions;
		char * dir;
		int lenPath;
		dfs(int m, int n, char ** board){
			M = m;
			N = n;
			maze = board;
			//maze[M-2, N-2] = " ";
			//display(maze);
			reserve_mem();
			search(1,1,path, directions);
			//display(visited);
			//display(path);
		}

		void search( int m, int n, bool **p, stack<char> d) { //m and n are current row/col in the maze.
			if (maze[m][n] == 0) { //if current state has no path.
				return;
			}
			if (visited[m][n]) { //if current state is already visited.
				return;
			}
			else {
				visited[m][n] = true;
				p[m][n] = 1;
				if (m + 1 < M && (maze[m + 1][n] == ' ' || maze[m + 1][n] == 'E')) { //go down
					d.push('d');
					search( m + 1, n, p,d);
					d.pop();
				}
				if (n + 1 < N && (maze[m][n + 1] == ' ' || maze[m][n + 1] == 'E'))  { //go right
					d.push('r');
					search( m, n + 1, p,d);
					d.pop();
				}
				if (m - 1 >= 0 && (maze[m - 1][n] == ' ' || maze[m - 1][n] == 'E'))  { //go up
					d.push('u');
					search( m - 1, n, p,d);
					d.pop();
				}
				if (n - 1 >= 0 && (maze[m][n - 1] == ' ' || maze[m][n - 1] == 'E'))  { //go left
					d.push('l');
					search( m, n - 1, p, d);
					d.pop();
				}
				if (m == M-2 && n == N-2) {
					dir = new char[d.size()];
					int index = 0;
					while (!d.empty()) {
						dir[index] = d.top();
        				//cout << ' ' << d.top();
        				d.pop();
						index++;
   					}
					lenPath = index;
					printf("\n");
					//display(visited);
					printf("\n");
					//display(path);
					//_getch();
					return;
				}
				if (m == 0 && n == 0) {
					cout << "NO SOLUTION POSSIBLE!";
					//_getch();
					//sleep(1000);
					return;
				}
				p[m][n] = 0;
			}
		}
	private:
		void reserve_mem(){
			visited = new bool* [M];

            for (int i = 0; i < M; i++) {
                visited[i] = new bool [N];
            }
			for (int i = 0; i < N; i++) { 
				for (int j = 0; j < M; j++) {
					visited[i][j] = false;
				}
			}

			path = new bool* [M];

            for (int i = 0; i < M; i++) {
                path[i] = new bool [N];
            }
			for (int i = 0; i < N; i++) { 
				for (int j = 0; j < M; j++) {
					path[i][j] = false;
				}
			}
		}
		void display(bool ** c) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
					//printf("%d, a", c[i][j]);
                        cout << c[i][j] << " ";
                }
                cout << endl;
            }
        }   
		void display(char ** c) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                        cout << c[i][j] << " ";
                }
                cout << endl;
            }
        }   

};