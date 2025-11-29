#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
extern char grid[max_rows][max_cols];
const int max_rows=200;
cost int max_cols=200;


// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
extern int train_count;

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
extern bool switch_states[26];
// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
extern int spawn_X[max_spawn];
extern int spawn_Y[max_spawn];
extern int spawn_count;
extern int dest_X[max-dest];
extern int dest_Y[max<_dest];
// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------

int tickSpeed=1;
bool paused=false;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int trains_arrived=0;
int trains_crashed=0;
// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
bool emergencyHalt=false;
// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() {

    // clear grid
    for(int y=0;y<max_rows;y++){
        for(int x=0;x<max_cols;x++){
            grid[y][x]=' ';
        }
    }

    // reset trains
    trainCount=0;
    for(int i=0;i<max_trains;i++){
        trains[i].active=false;
    }

    // reset switches
    for(int i=0;i<26;i++){
        switch_states[i]=false;
    }

    // reset spawns
    spawn_count=0;

    // reset destinations
    destCount=0;

    // sim params
    tickSpeed=1;
    paused=false;

    // metrics
    trainsArrived=0;
    trainsCrashed=0;

    // halt flag
    emergencyHalt=false;
}

