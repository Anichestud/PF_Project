#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

const int MAX_ROWS = 50;
const int MAX_COLS = 50;
const int MAX_TRAINS = 100;
const int MAX_SWITCHES = 26;
const int MAX_SPAWN_POINTS = 20;
const int MAX_DESTINATIONS = 20;

struct Train {
    int id;
    int x, y;
    int direction;
    int destX, destY;
    bool isActive;
    bool hasCrashed;
    bool hasArrived;
    int waitTicks;
};

struct Switch {
    char letter;
    int currentState;
    int counter;
    int kValue;
};

extern char grid[MAX_ROWS][MAX_COLS];
extern int gridRows;  // ← FIX: gridRows not current_rows
extern int gridCols;  // ← FIX: gridCols not current_cols

extern Train trains[MAX_TRAINS];
extern int numTrains;

extern Switch switches[MAX_SWITCHES];
extern int numSwitches;

extern int spawnPointsX[MAX_SPAWN_POINTS];
extern int spawnPointsY[MAX_SPAWN_POINTS];
extern int numSpawnPoints;

extern int destinationsX[MAX_DESTINATIONS];
extern int destinationsY[MAX_DESTINATIONS];
extern int numDestinations;

extern int currentTick;
extern int seed;
extern bool isRunning;
extern bool isPaused;

extern int trainsDelivered;
extern int trainsCrashed;

void initializeSimulationState();

#endif