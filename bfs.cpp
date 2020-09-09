#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
#include <math.h>

using namespace std;

bool checkRow(int val, int row, vector< vector<int>> &grid){
    for(size_t i = 0; i < grid.size(); ++i){
        if(grid[row][i] == val){
            return false;
        }
    }
    return true;
}

bool checkCol(int val, int col, vector< vector<int>> &grid){
    for(size_t i = 0; i < grid.size(); ++i){
        if(grid[i][col] == val){
            return false;
        }
    }
    return true;
}

bool checkBlock(int val, int row, int col, vector< vector<int>> &grid){
    int blockSize = sqrt(grid.size());
    int firstR = (row/blockSize)*blockSize;
    int firstC = (col/blockSize)*blockSize;

    for(int i = firstR; i < firstR+blockSize; ++i){
        for(int j = firstC; j < firstC+blockSize; ++j){
            if(val == grid[i][j]){
                return false;
            }
        }
    }

    return true;
}

bool isValid(int val, int row, int col, vector< vector<int>> &grid){
    return checkRow(val, row, grid) && checkCol(val, col, grid) && checkBlock(val, row, col, grid);
}

void fill(vector< vector<int>> &grid){
    string line;
    string letters = "ABCDEFGHIJKLMNOP";
    int row = 0;
    while(getline(cin, line)){
        if(line == ""){
            break;
        }
        grid.push_back(vector<int>());
        for(size_t i = 0; i < line.size(); i = i+2){
            string chr = string(1, line[i]);
            if(letters.find(chr) != string::npos){
                grid[row].push_back(stoi(chr, 0, 17));
            }else{
                grid[row].push_back(line[i]-48);
            }
        } 
        row++;
    }
}


void print(vector< vector<int>> &grid){
    for(int i = 0; i < grid.size(); ++i){
        if(grid[i][0] > 9){
            cout << (char)(grid[i][0]+55);
        }else{
            cout << grid[i][0];
        }        
        for(int j = 1; j < grid.size(); ++j){
            if(grid[i][j] > 9){
                cout << " " << (char)(grid[i][j]+55);
            }else{
                cout << " " << grid[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool isComplete(vector< vector<int>> &grid){
    for(auto row: grid){
        for(auto element: row){
            if(element == 0){
                return false;
            }
        }
    }
    return true;
}

int numChoices(vector< vector<int>> &grid, int i, int j){
    vector<bool> canPick(grid.size());
    fill(canPick.begin(), canPick.end(), true);

    //checks row
    for(int k = 0; k < grid.size(); ++k){
        canPick[grid[i][k]] = false;
    }

    //checks collumn
    for(int k = 0; k < grid.size(); ++k){
        canPick[grid[k][j]] = false;
    }

    //checks block
    int blockSize = sqrt(grid.size());
    int r = i/blockSize;
    int c = j/blockSize;

    for(int row = r*blockSize; row < r*blockSize+blockSize; ++row){
        for(int col = c*blockSize; col < c*blockSize+blockSize; ++col){
            canPick[grid[row][col]] =false;
        }
    }

    int count = 0;
    for(int k = 1; k <= grid.size(); ++k){
        if(canPick[k]){
            count ++;
        }
    }
    return count;
}


void findNext(int &row, int &col, vector< vector<int>> &grid, int &bestChoice){
    int bestRow = -1, bestCol = -1;
    bestChoice = 100;

    for(int i = 0; i < grid.size(); ++i){
        for(int j = 0; j < grid.size(); ++j){
            if(grid[i][j] == 0){
                int choices = numChoices(grid, i, j);
                if(bestChoice > choices){
                    bestChoice = choices;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    row = bestRow;
    col = bestCol;
}

bool solve(vector< vector<int>> &grid){
    stack <pair<int,int>> s;

    int i = 0;
    int j = 0;
    int bestChoice;
    findNext(i, j, grid, bestChoice);
    s.push({i,j});
    

    while(bestChoice != 100){

        int val = grid[i][j] + 1;
        
        while(isValid(val, i, j, grid) == false && val <= grid.size()){
            val++;
        }

        if(val > grid.size()){
            grid[i][j] = 0;
            s.pop();
            if(s.empty() == true){
                return false;
            }
            i = s.top().first;
            j = s.top().second;
        }else{
            grid[i][j] = val;
            findNext(i, j, grid, bestChoice);
            s.push({i,j});
        }
    }
    return true;
}

int main(){
    vector< vector<int>> grid;

    fill(grid);

    time_t start,end;
    time(&start);

    bool sol = solve(grid);
    if(sol == true){
        print(grid);
    }else{
        cout << "No Solution" << endl;
    }

    time(&end);

    double time_taken = double(end - start); 
    cout << "Time taken by program is : " << fixed 
         << time_taken; 
    cout << " sec " << endl; 


    return 0;
}