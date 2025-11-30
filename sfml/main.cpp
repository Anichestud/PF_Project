#include "app.h"
#include "simulation_state.h"
#include "simulation.h"
#include "io.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << "Switchback Rails - Loading..." << endl;

    // Require level file argument
    if (argc < 2) {
        cout << "Usage: ./switchback_rails <level file>" << endl;
        return 1;
    }

    const char* levelFile = argv[1];

    initializeSimulationState();

    // Load level using provided filename
    if (!loadLevelFile(levelFile)) {
        cout << "Failed to load level file: " << levelFile << endl;
        return 1;
    }

    cout << "Level loaded successfully!" << endl;
    cout << "Trains: " << numTrains << endl;

    // Start the app / rendering loop
    runApp();

    return 0;
}
