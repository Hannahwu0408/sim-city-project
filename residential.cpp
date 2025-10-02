#include "residential.h"
#include "simulation.h"
#include <iostream>
#include <vector>

Residential::Residential(vector<vector<string> > &ogVec, vector<vector<int> > &copyVec){}

void Residential::addNumWorkers(int i) {
    switch(i) { // check population of cell, add to proper adjCellWorkers
        case 0:
            break;
        case 1:
            adjCellWorkers1++;
            break;
        case 2:
            adjCellWorkers2++;
            break;
        case 3:
            adjCellWorkers3++;
            break;
        default:
            break;
    }

    if (i >= 4) {
        adjCellWorkers4++;
    }
}

void Residential::checkPowerLines(string i) {
    if (i == "P") {
        numPowerlines++;
    }
}

void Residential::checkAdjCells(vector<vector<int> > &vec, vector<vector<string> > &ogVec, int i, int j) {

    for (int m = i-1; m < i+2; m++) { //iterating through adjacent cells
        for (int n = j-1; n < j+2; n++) {
            if (m < vecRows && n < vecCols && m >= 0 && n >= 0) { // making sure to not check nonexistent cells (negative)
                addNumWorkers(vec[m][n]);
                checkPowerLines(ogVec[m][n]);
            }
        }
    }
}

void Residential::runResidential(vector<vector<string> > &ogVec, vector<vector<int> > &copyVec) {
    
    // all changes need to first happen in the tempvec and then be added in the copyvec at the end of runResidential
    vector<vector<int> > tempVec = copyVec; // create a temp vec that is originally equal to the copyVec
    int row_i = 0;
    int col_i = 0;
    
    for (int i = 0; i < vecRows; i++) {
        for (int j = 0; j < vecCols; j++) {
            
            if (ogVec[i][j] == "R") { // checks if cell is residential
                switch (tempVec[row_i][col_i]) { 
                    case 0: 
                        checkAdjCells(copyVec, ogVec, row_i, col_i); // check cells of copyVec
                        if (numPowerlines >= 1) { // increase population and available_workers accordingly
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        } else if (adjCellWorkers1 >= 1) {
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        }
                        break;
                    case 1:
                        checkAdjCells(copyVec, ogVec, row_i, col_i);
                        if (adjCellWorkers1 >= 1 ) {
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        }
                        break;
                    case 2:
                        checkAdjCells(copyVec, ogVec, row_i, col_i);
                        if (adjCellWorkers2 >= 4 ) {
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        }
                        break;
                    case 3:
                        checkAdjCells(copyVec, ogVec, row_i, col_i);
                        if (adjCellWorkers3 >= 6 ) {
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        }
                        break;
                    case 4:
                        checkAdjCells(copyVec, ogVec, row_i, col_i);
                        if (adjCellWorkers4 >= 8 ) {
                            tempVec[row_i][col_i]++;
                            available_workers++;
                        }
                        break;
                    default:
                        break;
                }
            }
            col_i++; // increment column counter
        }
        row_i++; // increment row counter
    }

    copyVec = tempVec; // set copyVec equal to tempVec after all changed are made
}
    
