#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global simulation state
// ============================================================================

// -----------------------------
// LIMITS / MAX VALUES
// -----------------------------
const int max_rows = 200;
const int max_cols = 200;
const int max_trains = 50;
const int max_switches = 50;
const int max_spawns = 50;
const int max_dests = 50;

// -----------------------------
// GRID
// -----------------------------
extern char grid[max_rows][max_cols];

// -----------------------------
// TRAINS
// -----------------------------
extern int trainX[max_trains];
extern int trainY[max_trains];
extern int trainDir[max_trains];
extern int trainActive[max_trains];
extern int trainCrashed[max_trains];
extern int trainArrived[max_trains];
extern int trainWait[max_trains];
extern int trainDestX[max_trains];
extern int trainDestY[max_trains];
extern int trainSpawnTick[max_trains];

extern int startX[max_trains];
extern int startY[max_trains];
extern int startDir[max_trains];

extern int nextX[max_trains];
extern int nextY[max_trains];
extern int nextDir[max_trains];

extern int numTrains;

// -----------------------------
// SWITCHES
// -----------------------------
extern char switchLetter[max_switches];
extern int switchStates[max_switches];  // fixed name
extern int switchCounter[max_switches];
extern int switchKValue[max_switches];
extern int numSwitches;

// -----------------------------
// SPAWN & DESTINATION POINTS
// -----------------------------
extern int spawn_X[max_spawns];
extern int spawn_Y[max_spawns];
extern int spawn_Count;

extern int dest_X[max_dests];
extern int dest_Y[max_dests];
extern int dest_count;

// -----------------------------
// SIMULATION PARAMS
// -----------------------------
extern int currentTick;
extern int seed;

// -----------------------------
// METRICS
// -----------------------------
extern int trainsDelivered;
extern int trainsCrashed;

// -----------------------------
// EMERGENCY HALT
// -----------------------------
extern bool emergencyActive;
extern int emergencyTicks;

// -----------------------------
// FUNCTIONS
// -----------------------------
void initializeSimulationState();

#endif
