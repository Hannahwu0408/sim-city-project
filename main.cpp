#include "simulation.h"
#include "residential.h"
#include "commercial.h"
#include "industrial.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int regional_population(vector< vector<int> > &copyVec, vector< vector<string> > &originalVec, string district){
    int population = 0;
    for (int i = 0; i < vecRows; i++) { //iterating through vector
        for (int j = 0; j < vecCols; j++) {
            if (originalVec[i][j] == district){
                population++;
            }
        }
    }
    return population;
}

int total_pollution;
int available_workers;
int available_goods;
int vecRows; 
int vecCols;

int main() {

    fstream file; //var to save file
    string configFile, regionFile, temp; //vars to get the name + words from file
    typedef vector< vector<string> > dataVec; //define 2D vector to store the region data
    dataVec originalVec; //2d vector for region data
    typedef vector< vector<int> > intVec; //define 2D vector to store population (and pollution) data
    intVec copyVec; //2d vector for pop. data
    intVec pollutionVec; // 2d vector for pollution data 
    int copyvar; //int to initialize population data
    vector<int> tempVec; //vector to use to push_back into 2D vector
    vector<int> pollutionTempVec; //vector to initialize the pollution 2d vector 

    // Variables for simulation. 
    total_pollution = 0;
    available_workers = 0;
    available_goods = 0;
    // Change these to 0 before testing the whole thing. 
    vecRows = 0; 
    vecCols = 0;
    int refresh_rate = 0;
    int time_step = 0;

    //prompt for configuration file name
    cout << "Please enter the file name for the configuration file: ";
    cin >> configFile;

    file.open(configFile);
    while (!file.is_open()){ //loop until file is valid
        cout<< "Invalid file name. Please try again: ";
        cin >> configFile;
        file.open(configFile);
    }

    while(file >> temp){
        stringstream ss;
        if (temp.find("Layout:") != string::npos) {
            temp.erase(temp.find("Layout:"), 7);
            regionFile = temp;
            cout << temp << endl;
        }
        if (temp.find("Limit:") != string::npos) {
            temp.erase(temp.find("Limit:"), 6);
            ss << temp;
            ss >> time_step;
        }
        if (temp.find("Rate:") != string::npos) {
            temp.erase(temp.find("Rate:"), 5);
            ss << temp;
            ss >> refresh_rate;
            break;
        }
    }

    file.close();
    file.open(regionFile);

    //save the simulation data
    Simulation simulation(file, originalVec);
    file.close();

    vecRows = originalVec.size(); 
    vecCols = -1; 

    //Function that records the maximum number of entries per row (columns). Occasionally doubles as print function.  
    for (int i = 0; i < originalVec.size(); i++) {
        
        int size = originalVec[i].size(); 
        if (size >= vecCols) { vecCols = originalVec[i].size();}
        
        for (int j = 0; j < originalVec[i].size(); j++) {
            
            cout << originalVec[i][j] << " "; 
        }
        
        cout << endl;
    }

    //iterate through dataVec and copy over to copyVec, change all values to 0,-1
    for (int i = 0; i < vecRows; i++) {

        for (int j = 0; j < vecCols; j++) {

            if (originalVec[i][j] == "R" || originalVec[i][j] == "I" || originalVec[i][j] == "C" 
            || originalVec[i][j] == " R" || originalVec[i][j] == " I" || originalVec[i][j] == " C") {

                copyvar = 0;
            }

            else {

                copyvar = -1;

            }

            tempVec.push_back(copyvar);   
            pollutionTempVec.push_back(0);

        }

        copyVec.push_back(tempVec);
        pollutionVec.push_back(pollutionTempVec);
        while (!tempVec.empty()) { tempVec.pop_back(); } 
        while (!pollutionTempVec.empty()) { pollutionTempVec.pop_back(); }  
    }

    for (int i = 0; i < time_step; i++) {

        if (i%refresh_rate == 0) { // If at refresh rate, print. 

            cout << "TIME STEP : " << i + 1 << endl;

            for (int i = 0; i < originalVec.size(); i++) {
            
                for (int j = 0; j < originalVec[i].size(); j++) {
                    
                    if (copyVec[i][j] != 0 && (originalVec[i][j] == "R" || originalVec[i][j] == "I" || originalVec[i][j] == "C" 
                    || originalVec[i][j] == " R" || originalVec[i][j] == " I" || originalVec[i][j] == " C")) {
                        
                        cout << copyVec[i][j] << " "; 

                    } 

                    else { cout << originalVec[i][j] << " "; }

                }
            
                cout << endl;
            
            }

            cout << "Available Goods: " << available_goods << endl;
            cout << "Available Workers: " << available_workers << endl;

        }

        Residential residential = Residential(originalVec, copyVec);
        residential.runResidential(originalVec, copyVec);

        industrial(copyVec, originalVec, pollutionVec);
        commercial(copyVec, originalVec);

    }

    // Update pollution vector with appropriate pollution one last time.
        // Because commercial triggers later than industrial, and it's only industrial pollution that spreads,
        // updating in one final time is necessary. 
    for (int i = 0; i < vecRows; i++) {

        for (int j = 0; j < vecCols; j++) {

            if (pollutionVec[i][j] < copyVec[i][j]) { pollutionVec[i][j] = copyVec[i][j]; }

        }

    }

    // Initialize variables to track final population + pollution. 

    int rTotal = 0, iTotal = 0, cTotal = 0, populationTotal = 0; 
    int rPollutionTotal = 0, pollutionTotal = 0; 

    // Output final region state + track final population & pollutions across residential, industrial, and commercial zones. 

    cout << endl; 
    cout << "FINAL REGION STATE  - - - - " << endl; 

    for (int i = 0; i < originalVec.size(); i++) { 
            
        for (int j = 0; j < originalVec[i].size(); j++) {

            pollutionTotal += pollutionVec[i][j]; 
                    
            if (copyVec[i][j] != 0 && (originalVec[i][j] == "R" || originalVec[i][j] == "I" || originalVec[i][j] == "C" 
                || originalVec[i][j] == " R" || originalVec[i][j] == " I" || originalVec[i][j] == " C")) {
                        
                cout << copyVec[i][j] << " "; 

                if (originalVec[i][j] == "R" || originalVec[i][j] == " R") { rTotal += copyVec[i][j]; rPollutionTotal += pollutionVec[i][j]; }
                else if (originalVec[i][j] == "I" || originalVec[i][j] == " I") { iTotal += copyVec[i][j]; }
                else if (originalVec[i][j] == "C" || originalVec[i][j] == " C") { cTotal += copyVec[i][j]; }

                populationTotal += copyVec[i][j]; 

            } 

            else { cout << originalVec[i][j] << " "; }

        }
            
        cout << endl;

    } 

    // Output total residential, industrial, commercial zone population. 
    cout << "Total Population : " << populationTotal << endl; 
    cout << " > Total Residential Zone Popultion: " << rTotal << endl;
    cout << " > Total Industrial Zone Popultion: " << iTotal << endl;
    cout << " > Total Commercial Zone Popultion: " << cTotal << endl;

    cout << "Total Pollution State: " << pollutionTotal << endl; 
    cout << " > Regional Pollution State: " << rPollutionTotal << endl; 

    int xstart = -1, xend = -1;
    int ystart = -1, yend = -1;

    cout << endl; 

    cout << "ZOOM-IN FEATURE - - - - " << endl;  

    char quit = 'Y';

    while(quit == 'Y') {

        cout << "Your map is (0, 0) to (" << (vecRows-1) << ", " << (vecCols-1) << ") big." << endl;  
        cout << " > Enter the starting x-coordinate of the region you'd like to zoom-in on : "; 
        cin >> xstart;
        cout << " > Enter the ending x-coordinate of the region you'd like to zoom-in on : "; 
        cin >> xend;
        cout << " > Enter the starting y-coordinate of the region you'd like to zoom-in on : "; 
        cin >> ystart; 
        cout << " > Enter the ending y-coordinate of the region you'd like to zoom-in on : "; 
        cin >> yend; 

        while (xstart < 0 || ystart < 0 || xend >= vecRows || yend >= vecCols) {

            cout << "The coordinates you have entered are out-of-bounds. Please enter them again." << endl; 
            cout << " > Enter the starting x-coordinate of the region you'd like to zoom-in on : "; 
            cin >> xstart;
            cout << " > Enter the ending x-coordinate of the region you'd like to zoom-in on : "; 
            cin >> xend;
            cout << " > Enter the starting y-coordinate of the region you'd like to zoom-in on : "; 
            cin >> ystart; 
            cout << " > Enter the ending y-coordinate of the region you'd like to zoom-in on : "; 
            cin >> yend; 

        }

        cout << "Checking coordinates (" << xstart << ", " << ystart << ") to (" << xend << ", " << yend << ")..." << endl; 
        pollutionTotal = 0;
        populationTotal = 0;

        for (int i = xstart; i <= xend; i++) { 
                
            for (int j = ystart; j <= yend; j++) {

                pollutionTotal += pollutionVec[i][j]; 
                        
                if (copyVec[i][j] != 0 && (originalVec[i][j] == "R" || originalVec[i][j] == "I" || originalVec[i][j] == "C" 
                    || originalVec[i][j] == " R" || originalVec[i][j] == " I" || originalVec[i][j] == " C")) {
                            
                    populationTotal += copyVec[i][j];

                } 

            }
                
        } 

        cout << " > Total Population of region (" << xstart << ", " << ystart << ") to (" << xend << ", " << yend << ") : " << populationTotal << endl; 
        cout << " > Total Pollution of region (" << xstart << ", " << ystart << ") to (" << xend << ", " << yend << ") : " << pollutionTotal << endl; 
 
        cout << "Would you like to Zoom-In again? (Y/N) : "; 
        cin >> quit; 

    }

    return 0;
}
