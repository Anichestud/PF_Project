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
int trainX[max_trains];
int trainY[max_trains];
int trainDir[max_trains];
int trainActive[max_trains];
int trainCrashed[max_trains];
int trainArrived[max_trains];
int trainWait[max_trains];
int trainDestX[max_trains];
int trainDestY[max_trains];
int trainSpawnTick[max_trains];
int numTrains = 0;

int startX[max_trains];
int startY[max_trains];
int startDir[max_trains];

int nextX[max_trains];
int nextY[max_trains];
int nextDir[max_trains];

// ----------------------------------------------------------------------------
// SWITCH ARRAYS
// ----------------------------------------------------------------------------
char switchLetter[max_switches];
int switchState[max_switches];
int switchCounter[max_switches];
int switchKValue[max_switches];
int numSwitches = 0;

// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
int spawn_X[max_spawns];
int spawn_Y[max_spawns];
int spawn_Count = 0;

int dest_X[max_dests];
int dest_Y[max_dests];
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
int emergencyActive = 0;
int emergencyTicks = 0;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
void initializeSimulationState() {

    // clear grid
    for(int y = 0; y < max_rows; y++){
        for(int x = 0; x < max_cols; x++){
            grid[y][x] = ' ';
        }
    }
    
    // reset train arrays
    numTrains = 0;
    for(int i = 0; i < max_trains; i++){
        trainX[i] = 0;
        trainY[i] = 0;
        trainDir[i] = 0;
        trainActive[i] = 0;
        trainCrashed[i] = 0;
        trainArrived[i] = 0;
        trainWait[i] = 0;
        trainDestX[i] = 0;
        trainDestY[i] = 0;
        trainSpawnTick[i] = -1;
        
        startX[i] = 0;
        startY[i] = 0;
        startDir[i] = 0;
        
        nextX[i] = 0;
        nextY[i] = 0;
        nextDir[i] = 0;
    }

    // reset switches
    numSwitches = 0;
    for(int i = 0; i < max_switches; i++){
        switchLetter[i] = '\0';
        switchState[i] = 0;
        switchCounter[i] = 0;
        switchKValue[i] = 1;
    }

    // reset spawns
    spawn_Count = 0;
    for(int i = 0; i < max_spawns; i++){
        spawn_X[i] = 0;
        spawn_Y[i] = 0;
    }

    // reset destinations
    dest_count = 0;
    for(int i = 0; i < max_dests; i++){
        dest_X[i] = 0;
        dest_Y[i] = 0;
    }

    // sim params
    currentTick = 0;
    seed = 0;

    // metrics
    trainsDelivered = 0;
    trainsCrashed = 0;

    // halt flags
    emergencyActive = 0;
    emergencyTicks = 0;
}