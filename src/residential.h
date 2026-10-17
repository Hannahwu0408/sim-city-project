#ifndef RESIDENTIAL_H
#define RESIDENTIAL_H
#include "simulation.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Residential {

    public:
    // initialize all vars
    int adjCellWorkers1 = 0;
    int adjCellWorkers2 = 0; 
    int adjCellWorkers3 = 0;
    int adjCellWorkers4 = 0; 
    int numPowerlines = 0;

    // constructor for residential class
    Residential(vector<vector<string> > &ogVec, vector<vector<int> > &copyVec); 

    // function to use in checkAdjCells in order to streamline checking the num of workers
    void addNumWorkers(int);

    // function to check if a cell is a powerline 
    void checkPowerLines(string);

    // function to check adjacent cell workers of cell at [row][column]
    void checkAdjCells(vector<vector<int> > &copyVec, vector<vector<string> > &ogVec, int i, int j);

    // function to perform residential operations
    void runResidential(vector<vector<string> > &ogVec, vector<vector<int> > &copyVec); 

};

#endif
