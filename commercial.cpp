#include "commercial.h"
#include <iterator> 
#include <algorithm>

//function: to get the population of the adjacent cells
//return: returns an integer value representing the adjacent population
//parameters: copyVec-2d population vector, int i- the x coordinate, int j- the y coordinate
int adj_population(vector< vector<int> > &copyVec, int i, int j){
    int population = 0;
    for (int m = i-1; m < i+2; m++){ //iterating through adjacent cells :3
        for (int n = j-1; n < j+2; n++){
            if (m < vecRows && n < vecCols && m >= 0 && n >= 0){ //checking bounds of the vector
                if (copyVec[m][n] >= 1 && !(m == i && n == j)){ //check if cell has population
                    population++;
                }
            }
        }
    }
    return population;
}

//function: to check whether cell is next to a powerline
//return: returns true if cell is next to powerline and false if not
//parameters: originalVec- 2d vector of cell type, int i- x coordinate, int j- y coordinate
bool powerline(vector< vector<string> > &originalVec, int i, int j){
    for (int m = i-1; m < i+2; m++){ //iterating through adjacent cells :3
        for (int n = j-1; n < j+2; n++){
            if (m < vecRows && n < vecCols && m >= 0 && n >= 0){
                if ((originalVec[m][n] == "T" || originalVec[m][n] == "#") && !(m == i && n == j)){ //checking whether adjacent cell is powerline
                    return true;
                }
            }
        }
    }
    return false;
}

//object: contains information on singular cell
struct Cell{
    int index[2]; //xy-coordinates
    int adjPopulation; //adjacent population
    int population; //cell population
};

//function: used to sort Cell object in cells vector by greatest to least according to adjacent population
bool compareByAdjPop(const Cell &a, const Cell &b)
{
    return a.adjPopulation > b.adjPopulation;
}

//function: used to sort Cell object in cells vector by greatest to least according to cell population
bool compareByPop(const Cell &a, const Cell &b)
{
    return a.population > b.population;
}


void commercial(vector< vector<int> > &copyVec, vector< vector<string> > &originalVec){
    vector<Cell> cells; //vector to store object called Cell

        //loop through copyVec and originalVec to find commercial cells
        for (int i = 0; i < vecRows; i++) { //iterating through vector
            for (int j = 0; j < vecCols; j++) {
                if (originalVec[i][j] == "C"){
                    int num = adj_population(copyVec, i, j);
                    Cell temp = {{i, j}, num, copyVec[i][j]}; //creating temporary Cell object
                    cells.push_back(temp); //adding cell to vector
                }
            }
        }
        // cout << "vec size: " << cells.size() << endl;

        sort(cells.begin(), cells.end(), compareByPop); //sorting cells vector by individual cell population (greatest to least)
        
        int first = 0; //count of cells with population of one
        int second = 0; //count of cells with population of zero
        //loop to count amount of cells with population one and zero
        for (int i = 0; i < cells.size(); i++) {
            if (cells[i].population == 1){
                first++;
            }
            if (cells[i].population == 0){
                second++;
            }
        }

        vector<Cell>::iterator it;  // declare an iterator to a vector of cell objects
        int i = 0;  // counter
        int x = cells.size() - first - second; //count of cells with population greaters than or equal to two

        //loop only if there is cells with equal population
        if (first >= 2 || second >= 2){
            for(it = cells.begin(); it != cells.end(); it++, i++ ){ //loop through cells vector and sort by adjacent population
                if (first >= 1 && second >= 1 && i == x + first){
                    sort(cells.begin(), it, compareByAdjPop);
                    sort(it, cells.end(), compareByAdjPop);
                }
                if ((first == 0 || second == 0) && i == x){
                    sort(it, cells.end(), compareByAdjPop);
                }
                
            }
        }



        for (int i = 0; i < cells.size(); i++) { //iterating through cells vector
            // cout << cells[i].index[0] << " " << cells[i].index[1] << " " << cells[i].population << " " << cells[i].adjPopulation << endl;
            int x = cells[i].index[0]; //x-coordinate
            int y = cells[i].index[1]; //y-coordinate
            int pop = cells[i].adjPopulation; //adjacent population
            bool adj_power = powerline(originalVec, x, y); //whether adjacent to powerling

            //increase population
            if (copyVec[x][y] == 1 && pop >= 2 && available_goods >=1 && available_workers >= 1){
                copyVec[x][y] = 2; //increase pop by one
                available_goods--;
                available_workers--;
            }
            if (copyVec[x][y] == 0){
                if(pop >= 1 || adj_power){ //checking whether adjacent cell has population or powerline
                    if(available_goods >=1 && available_workers >= 1){
                        available_goods--;
                        available_workers--;
                        copyVec[x][y] = 1;
                    }
                }
            }        
        }

    return;
}
