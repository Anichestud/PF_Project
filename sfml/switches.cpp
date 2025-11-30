#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
    // Already done in moveAllTrains() in trains.cpp
    // This function can be empty or just a placeholder
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    for (int i = 0; i < numSwitches; i++) {
        if (switchCounter[i] >= switchKValue[i]) {
            // Mark this switch to flip
            switchCounter[i] = 0;  // Reset counter
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
    for (int i = 0; i < numSwitches; i++) {
        if (switchCounter[i] >= switchKValue[i]) {
            // Flip the state
            if (switchState[i] == 0) {
                switchState[i] = 1;
            } else {
                switchState[i] = 0;
            }
            switchCounter[i] = 0;
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
    // Simple placeholder - you can implement signal colors later
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int switchIndex) {
    if (switchIndex < 0 || switchIndex >= numSwitches) return;
    
    if (switchState[switchIndex] == 0) {
        switchState[switchIndex] = 1;
    } else {
        switchState[switchIndex] = 0;
    }
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(int switchIndex, int currentDir) {
    if (switchIndex < 0 || switchIndex >= numSwitches) return currentDir;
    
    // Simplified: if state is 0, keep direction
    // If state is 1, turn (simplified logic)
    if (switchState[switchIndex] == 0) {
        return currentDir;  // Straight
    } else {
        // Turn right
        if (currentDir == 0) return 1;  // UP -> RIGHT
        if (currentDir == 1) return 2;  // RIGHT -> DOWN
        if (currentDir == 2) return 3;  // DOWN -> LEFT
        if (currentDir == 3) return 0;  // LEFT -> UP
    }
    
    return currentDir;
}