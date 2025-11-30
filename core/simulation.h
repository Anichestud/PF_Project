#ifndef SIMULATION_H
#define SIMULATION_H

// ============================================================================
// SIMULATION.H - Simulation tick logic
// ============================================================================
extern int spawn_tick[max_spawns];
extern int spawn_dir[max_spawns]
// ----------------------------------------------------------------------------
// MAIN SIMULATION FUNCTION
// ----------------------------------------------------------------------------
// Run one simulation tick.
void simulateOneTick();

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// Initialize the simulation after loading a level.
void initializeSimulation();

// ----------------------------------------------------------------------------
// UTILITY
// ----------------------------------------------------------------------------
// True if all trains are delivered or crashed.
bool isSimulationComplete();

#endif


