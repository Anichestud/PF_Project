#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Switchback Rails - Loading..." << endl;
    
    initializeSimulationState();
    
    if (!loadLevelFile()) {
        cout << "Failed to load level file!" << endl;
        return 1;
    }
    
    cout << "Level loaded successfully!" << endl;
    cout << "Trains: " << numTrains << endl;
    cout << "Switches: " << numSwitches << endl;
    
    initializeSimulation();
    
    if (!initializeApp()) {
        cout << "Failed to initialize app!" << endl;
        return 1;
    }
    
    cout << "Controls: SPACE=pause, ESC=exit" << endl;
    
    runApp();
    
    writeMetrics();
    cleanupApp();
    
    cout << "Simulation complete!" << endl;
    cout << "Delivered: " << trainsDelivered << endl;
   // Count crashed trains manually
int crashedCount = 0;
for (int i = 0; i < numTrains; i++) {
    if (trainCrashed[i] == 1) {
        crashedCount++;
    }
}
cout << "Crashed: " << crashedCount << endl;
    
    return 0;
}