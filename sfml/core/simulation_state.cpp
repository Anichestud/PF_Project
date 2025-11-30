#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
char grid[max_rows][max_cols];

// ----------------------------------------------------------------------------
// TRAIN ARRAYS
// ----------------------------------------------------------------------------
int trainX[max_trains] = {0};
int trainY[max_trains] = {0};
int trainDir[max_trains] = {0};
int trainActive[max_trains] = {0};
int trainCrashed[max_trains] = {0};
int trainArrived[max_trains] = {0};
int trainWait[max_trains] = {0};
int trainDestX[max_trains] = {0};
int trainDestY[max_trains] = {0};
int trainSpawnTick[max_trains] = {0};
int numTrains = 0;

int startX[max_trains] = {0};
int startY[max_trains] = {0};
int startDir[max_trains] = {0};

int nextX[max_trains] = {0};
int nextY[max_trains] = {0};
int nextDir[max_trains] = {0};

// ----------------------------------------------------------------------------
// SWITCH ARRAYS
// ----------------------------------------------------------------------------
char switchLetter[max_switches] = {0};
int switchStates[max_switches] = {0};
int switchCounter[max_switches] = {0};
int switchKValue[max_switches] = {0};
int numSwitches = 0;

// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
int spawn_X[max_spawns] = {0};
int spawn_Y[max_spawns] = {0};
int spawn_Count = 0;

int dest_X[max_dests] = {0};
int dest_Y[max_dests] = {0};
int dest_count = 0;

// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int currentTick = 0;
int seed = 0;

// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int trainsDelivered = 0;
int trainsCrashed = 0;

// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
bool emergencyActive = false;
int emergencyTicks = 0;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
void initializeSimulationState() {

    // Clear grid
    for (int y = 0; y < max_rows; y++) {
        for (int x = 0; x < max_cols; x++) {
            grid[y][x] = ' ';
        }
    }

    // Reset train arrays
    numTrains = 0;
    for (int i = 0; i < max_trains; i++) {
        trainX[i] = trainY[i] = trainDir[i] = trainActive[i] = 0;
        trainCrashed[i] = trainArrived[i] = trainWait[i] = 0;
        trainDestX[i] = trainDestY[i] = trainSpawnTick[i] = -1;

        startX[i] = startY[i] = startDir[i] = 0;
        nextX[i] = nextY[i] = nextDir[i] = 0;
    }

    // Reset switches
    numSwitches = 0;
    for (int i = 0; i < max_switches; i++) {
        switchLetter[i] = '\0';
        switchStates[i] = 0;
        switchCounter[i] = 0;
        switchKValue[i] = 1;
    }

    // Reset spawns
    spawn_Count = 0;
    for (int i = 0; i < max_spawns; i++) {
        spawn_X[i] = spawn_Y[i] = 0;
    }

    // Reset destinations
    dest_count = 0;
    for (int i = 0; i < max_dests; i++) {
        dest_X[i] = dest_Y[i] = 0;
    }

    // Simulation parameters
    currentTick = 0;
    seed = 0;

    // Metrics
    trainsDelivered = 0;
    trainsCrashed = 0;

    // Emergency halt
    emergencyActive = false;
    emergencyTicks = 0;
}
