#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global State Definitions
// ============================================================================

// ----------------------------------------------------------------------------
// CONSTANTS (The "Magic Numbers")
// ----------------------------------------------------------------------------
// These define the size of your arrays.
const int MAX_ROWS = 200;
const int MAX_COLS = 200;
const int MAX_TRAINS = 500;   // Maximum number of trains supported
const int MAX_SWITCHES = 26;  // A through Z
const int MAX_SPAWN_POINTS = 50;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES (External Declarations)
// ----------------------------------------------------------------------------
// "extern" means: "These variables exist, but they are created in the .cpp file"

// The Map
extern char grid[MAX_ROWS][MAX_COLS];
extern int gridWidth;
extern int gridHeight;

// Simulation Time
extern int currentTick;

// Train Data
extern int numTrains;
extern int trainX[MAX_TRAINS];
extern int trainY[MAX_TRAINS];
extern int trainDir[MAX_TRAINS];
extern int trainDestX[MAX_TRAINS];
extern int trainDestY[MAX_TRAINS];

// Train Status
extern int trainActive[MAX_TRAINS];    // 1 = Active, 0 = Inactive
extern int trainCrashed[MAX_TRAINS];   // 1 = Crashed
extern int trainArrived[MAX_TRAINS];   // 1 = Arrived safely
extern int trainWait[MAX_TRAINS];      // How long it has waited

// Train Start Data (loaded from file)
extern int trainSpawnTick[MAX_TRAINS];
extern int startX[MAX_TRAINS];
extern int startY[MAX_TRAINS];
extern int startDir[MAX_TRAINS];

// Planned Movement (Calculated before moving)
extern int nextX[MAX_TRAINS];
extern int nextY[MAX_TRAINS];
extern int nextDir[MAX_TRAINS];

// Switch Data
extern int numSwitches;
extern int switchState[MAX_SWITCHES];   // 0 or 1
extern int switchCounter[MAX_SWITCHES]; // How many trains passed
extern int switchK[MAX_SWITCHES];       // The K value from file

// Metrics
extern int trainsDelivered;
extern int trainsCrashedTotal; // Renamed to avoid conflict with boolean array
extern int emergencyActive;
extern int emergencyTicks;

// ----------------------------------------------------------------------------
// FUNCTIONS
// ----------------------------------------------------------------------------
void initializeSimulationState();

#endif