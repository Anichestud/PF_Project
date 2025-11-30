#include "simulation_state.h"
#include "simulation.h"
#include "io.h"
#include "grid.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << "=== Switchback Rails Console Mode ===" << endl;
    
    if (argc < 2) {
        cout << "Usage: ./console <level_file>" << endl;
        return 1;
    }
    
    initializeSimulationState();
    
    if (!loadLevelFile(argv[1])) {
        cout << "Failed to load level!" << endl;
        return 1;
    }
    
    cout << "Loaded: " << numTrains << " trains" << endl;
    cout << "Grid: " << gridRows << "x" << gridCols << endl;
    
    initializeSimulation();
    
    // Run 10 ticks
    for (int i = 0; i < 10; i++) {
        cout << "\n=== TICK " << currentTick << " ===" << endl;
        
        // Print grid
        for (int r = 0; r < gridRows && r < 20; r++) {
            for (int c = 0; c < gridCols && c < 40; c++) {
                bool trainHere = false;
                for (int t = 0; t < numTrains; t++) {
                    if (trainActive[t] && trainX[t] == c && trainY[t] == r) {
                        cout << 'T';
                        trainHere = true;
                        break;
                    }
                }
                if (!trainHere) cout << grid[r][c];
            }
            cout << endl;
        }
        
        simulateOneTick();
    }
    
    cout << "\nDelivered: " << trainsDelivered << endl;
    cout << "Crashed: " << trainsCrashed << endl;
    
    return 0;
}
