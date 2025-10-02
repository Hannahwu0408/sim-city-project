#include <vector>
#include <algorithm>
#include "pollution.h"
#include "simulation.h"

using namespace std;

void spreadPollution(vector<vector<int> > &copyVec, vector<vector<int> > &pollutionLevels, int i, int j, int population, int vecRows, int vecCols) {

  for (int m = 0; m < vecRows; m++) {

    for (int n = 0; n < vecCols; n++) {

      if (pollutionLevels[m][n] < copyVec[m][n]) { pollutionLevels[m][n] = copyVec[m][n]; }

    }

  }

  //cout << i << " " << j << endl;

  int newPollutionLevel = population; 

  //cout << " pollution starts at " << newPollutionLevel << endl; 

  while (newPollutionLevel > 0) { 

    newPollutionLevel--; 

    /*
    cout << " current pollution level " << newPollutionLevel << endl;
    cout << " distance from original block " << (population - newPollutionLevel) << endl; 

    cout << " m starts : " << i - (population - newPollutionLevel) << endl; 
    cout << "  m ends : " << i + (population - newPollutionLevel) << endl; 

    cout << " n starts : " << j-(population - newPollutionLevel) << endl; 
    cout << "  n ends : " << j + (population - newPollutionLevel) << endl; 
    */

    for (int m = i-(population - newPollutionLevel); m <= i+(population - newPollutionLevel); m++) {

      // Fixed error: m bounds was set to m <= m+(population - newPollutionLevel).

      for (int n = j-(population - newPollutionLevel); n <= j+(population - newPollutionLevel); n++){

        //cout << " " << m << " " << n << endl; 

        if ((m == i && n == j) || m < 0 || n < 0 || m >= vecRows || n >= vecCols) { //skip the current cell
          continue;
        }

        if (pollutionLevels[m][n] < newPollutionLevel) { pollutionLevels[m][n] = newPollutionLevel; }

      }

    }

  }
  
}
