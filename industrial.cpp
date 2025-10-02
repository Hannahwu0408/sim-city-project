#include "industrial.h"
#include "pollution.h"

//triggers population growth when conditions are met
void conditionMet(vector< vector<int> > &copyVec, int i, int j, vector< vector<int> > &pollutionVec) {

  copyVec[i][j]++;
  available_workers -= 2;
  spreadPollution(copyVec, pollutionVec, i, j, copyVec[i][j], vecRows, vecCols);

}

void industrial(vector< vector<int> > &copyVec, vector< vector<string> > &originalVec, vector< vector<int> > &pollutionVec){
    for (int i = 0; i < vecRows; i++) { //iterating through vector
        for (int j = 0; j < vecCols; j++) {
            if (originalVec[i][j] == "I"){
              available_goods += copyVec[i][j]; // Producing goods for every industrial cell based on its current population,
              if (i>=0 && i<vecRows && j>=0 && j<vecCols) {

                if (copyVec[i][j] == 2 && available_workers >= 2) {

                    // at least two cell with a population of at least 2

                    bool adj2 = false; 
                    int adj2num = 0; 
                    for (int m = i-1; m < i+1; m++){ //iterating through adjacent cells
                      for (int n = j-1; n < j+1; n++){
                        if (m>=0 && m < vecRows && n>=0 && n < vecCols && !(m==i && n==j)){
                          if (copyVec[i][j] >= 2) { adj2num++; }
                          if (adj2num >= 2) { adj2 = true; break; }
                        }
                      }
                    }

                    if (adj2 == true) { conditionMet(copyVec, i, j, pollutionVec); }

                  }

                if (copyVec[i][j] == 1 && available_workers >= 2) {

                  // at least two cells with a population of at least 1

                  bool adj1 = false; 
                  int adj1num = 0; 
                  for (int m = i-1; m < i+1; m++){ //iterating through adjacent cells
                    for (int n = j-1; n < j+1; n++){
                      if (m>=0 && m < vecRows && n>=0 && n < vecCols && !(m==i && n==j)){
                        if (copyVec[i][j] >= 1) { adj1num++; }
                        if (adj1num >= 2) { adj1 = true; break; }
                      }
                    }
                  }

                  if (adj1 == true) { conditionMet(copyVec, i, j, pollutionVec); }

                } 

                
                  bool adjPowerLine = false; 

                  for (int m = i-1; m < i+1; m++){ //iterating through adjacent cells
                    for (int n = j-1; n < j+1; n++){
                      if (m>=0 && m < vecRows && n>=0 && n < vecCols && !(m==i && n==j)){
                        if (originalVec[m][n] == "T" || originalVec[m][n] == "#") {
                          adjPowerLine = true; break; 
                        }
                      }
                    }
                  }

                  if (adjPowerLine == true) { 
                    
                    //cout << "0 + power line" << endl; 
                    conditionMet(copyVec, i, j, pollutionVec); 
                    
                  }
                  else { 
                  // at least one cell with a population of at least 1

                    bool adj1= false; 
                    for (int m = i-1; m < i+1; m++){ //iterating through adjacent cells
                      for (int n = j-1; n < j+1; n++){
                        if (m>=0 && m < vecRows && n>=0 && n < vecCols && !(m==i && n==j)){
                          if (copyVec[i][j] >= 1) { adj1 = true; break; }
                        }
                      }
                    }

                    if (adj1 == true) { 
                      
                      //cout << "0 + adjacent" << endl; 
                      conditionMet(copyVec, i, j, pollutionVec); 
                      
                    }

                  }

                } 

                }

              }

          }

        }

  
