#include "simulation.h"
#include <fstream>
#include <iostream>
#include <sstream>

Simulation::Simulation(istream &input, vector< vector<string> >&SimVec) {
    //define string line to store the file's lines
    //temp to store values of each character from the csv file
    string line, temp;

    //vector that we can use to push into the output vector"
    vector<string> rows;

    while(getline(input, line, '\n')) {

        //break the line read into seperate character types
        istringstream breakline(line);

        while(getline(breakline, temp, ',')) {
            //store the type characters into the rows vector
            //cout << temp << " "; 
            rows.push_back(temp);
       }

        SimVec.push_back(rows);

        while (!rows.empty()) { rows.pop_back(); }

    }
  //store rows vector into output vector

}

void Simulation::runSimulation() {

    // Perform the simulation steps

}
