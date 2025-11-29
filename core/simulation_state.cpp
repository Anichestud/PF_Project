#include "simulation_state.h"
#include <cstring>

// ============================================================================
// CREATE THE VARIABLES (no extern here!)
// ============================================================================

// Grid
char grid[MAX_ROWS][MAX_COLS];
int gridRows = 0;
int gridCols = 0;

// Trains
Train trains[MAX_TRAINS];
int numTrains = 0;

// Switches
Switch switches[MAX_SWITCHES];
int numSwitches = 0;

// Spawn points
int spawnPointsX[MAX_SPAWN_POINTS];
int spawnPointsY[MAX_SPAWN_POINTS];
int numSpawnPoints = 0;

// Destinations
int destinationsX[MAX_DESTINATIONS];
int destinationsY[MAX_DESTINATIONS];
int numDestinations = 0;

// Simulation
int currentTick = 0;
int seed = 0;
bool isRunning = false;
bool isPaused = true;

// Metrics
int trainsDelivered = 0;
int trainsCrashed = 0;

// ============================================================================
// INITIALIZATION FUNCTION
// ============================================================================

void initializeSimulationState() {
    // Clear grid
    memset(grid, '.', sizeof(grid));
    gridRows = 0;
    gridCols = 0;
    
    // Clear trains
    numTrains = 0;
    for (int i = 0; i < MAX_TRAINS; i++) {
        trains[i].id = -1;
        trains[i].x = -1;
        trains[i].y = -1;
        trains[i].direction = 0;
        trains[i].destX = -1;
        trains[i].destY = -1;
        trains[i].isActive = false;
        trains[i].hasCrashed = false;
        trains[i].hasArrived = false;
        trains[i].waitTicks = 0;
    }
    
    // Clear switches
    numSwitches = 0;
    for (int i = 0; i < MAX_SWITCHES; i++) {
        switches[i].letter = '\0';
        switches[i].currentState = 0;
        switches[i].counter = 0;
        switches[i].kValue = 1;
    }
    
    // Clear spawn points
    numSpawnPoints = 0;
    for (int i = 0; i < MAX_SPAWN_POINTS; i++) {
        spawnPointsX[i] = -1;
        spawnPointsY[i] = -1;
    }
    
    // Clear destinations
    numDestinations = 0;
    for (int i = 0; i < MAX_DESTINATIONS; i++) {
        destinationsX[i] = -1;
        destinationsY[i] = -1;
    }
    
    // Reset simulation
    currentTick = 0;
    seed = 0;
    isRunning = false;
    isPaused = true;
    
    // Reset metrics
    trainsDelivered = 0;
    trainsCrashed = 0;
}