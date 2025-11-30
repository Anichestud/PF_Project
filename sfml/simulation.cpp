#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------
void initializeSimulation() {
    // Seed random number generator
    srand(seed);
    
    // Initialize log files
    initializeLogFiles();
    
    // Reset tick counter
    currentTick = 0;
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------
void simulateOneTick() {
    // PHASE 1: Spawn trains for this tick
    spawnTrainsForTick();
    
    // PHASE 2: Determine all train routes
    determineAllRoutes();
    
    // PHASE 3: Update switch counters (done in moveAllTrains)
    updateSwitchCounters();
    
    // PHASE 4: Queue switches that need to flip
    queueSwitchFlips();
    
    // PHASE 5: Detect collisions
    detectCollisions();
    
    // PHASE 6: Move all trains
    moveAllTrains();
    
    // PHASE 7: Check arrivals
    checkArrivals();
    
    // PHASE 8: Apply deferred switch flips
    applyDeferredFlips();
    
    // PHASE 9: Update emergency halt
    updateEmergencyHalt();
    
    // PHASE 10: Apply emergency halt if active
    applyEmergencyHalt();
    
    // Log this tick
    logTrainTrace();
    logSwitchStates();
    
    // Increment tick
    currentTick++;
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------
bool isSimulationComplete() {
    // Check if all trains are either delivered or crashed
    int activeTrains = 0;
    
    for (int i = 0; i < numTrains; i++) {
        if (trainActive[i] == 1) {
            activeTrains++;
        }
    }
    
    // If no active trains, simulation is done
    if (activeTrains == 0) {
        return true;
    }
    
    return false;
}