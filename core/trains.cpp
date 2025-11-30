#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    // Check every train to see if it is time to start
    for (int i = 0; i < numTrains; i++) {
        
        // If the train is not active yet AND the time matches
        if (trainActive[i] == 0 && trainSpawnTick[i] == currentTick) {
            
            // Turn the train on
            trainActive[i] = 1;
            
            // Reset its status
            trainCrashed[i] = 0;
            trainArrived[i] = 0;
            trainWait[i] = 0;

            // Set position to the start coordinates
            trainX[i] = startX[i];
            trainY[i] = startY[i];
            trainDir[i] = startDir[i];
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int i) {
    
    // If train is crashed or arrived, do nothing
    if (trainActive[i] == 0) {
        return false;
    }

    char tile = grid[trainY[i]][trainX[i]];
    int currentD = trainDir[i];

    // Figure out change in X and Y based on current direction
    // 0 = Up, 1 = Right, 2 = Down, 3 = Left
    int dx = 0;
    int dy = 0;

    if (currentD == 0) dy = -1;
    if (currentD == 1) dx = 1;
    if (currentD == 2) dy = 1;
    if (currentD == 3) dx = -1;

    // Calculate the tentative new position
    int possibleX = trainX[i] + dx;
    int possibleY = trainY[i] + dy;

    // RULE 1: Standard tracks
    if (tile == '-' || tile == '|') {
        nextX[i] = possibleX;
        nextY[i] = possibleY;
        nextDir[i] = currentD; // Keep same direction
        return true;
    }

    // RULE 2: Crossings (+)
    if (tile == '+') {
        // Ask the helper function which way to go
        int newDirection = getSmartDirectionAtCrossing(i);
        nextDir[i] = newDirection;

        // Recalculate movement based on new direction
        int dx2 = 0;
        int dy2 = 0;
        if (newDirection == 0) dy2 = -1;
        if (newDirection == 1) dx2 = 1;
        if (newDirection == 2) dy2 = 1;
        if (newDirection == 3) dx2 = -1;

        nextX[i] = trainX[i] + dx2;
        nextY[i] = trainY[i] + dy2;
        return true;
    }

    // RULE 3: Switches (A-Z)
    if (tile >= 'A' && tile <= 'Z') {
        // Ask helper function for switch direction
        int newDirection = getNextDirection(i);
        nextDir[i] = newDirection;

        // Recalculate movement
        int dx2 = 0;
        int dy2 = 0;
        if (newDirection == 0) dy2 = -1;
        if (newDirection == 1) dx2 = 1;
        if (newDirection == 2) dy2 = 1;
        if (newDirection == 3) dx2 = -1;

        nextX[i] = trainX[i] + dx2;
        nextY[i] = trainY[i] + dy2;
        return true;
    }

    // Default: just move forward if nothing else matched
    nextX[i] = possibleX;
    nextY[i] = possibleY;
    nextDir[i] = currentD;
    return true;
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int i) {
    char tile = grid[trainY[i]][trainX[i]];
    
    // Find which switch this is (0 to 25)
    int switchIndex = getSwitchIndex(tile);

    // If something went wrong and it's not a switch, keep going straight
    if (switchIndex < 0) {
        return trainDir[i];
    }

    int count = switchCounter[switchIndex];
    int currentD = trainDir[i];

    // If the count is even (0, 2, 4...), turn RIGHT
    if ((count % 2) == 0) {
        // Math trick to turn right: (dir + 1) % 4
        return (currentD + 1) % 4;
    }
    
    // If the count is odd (1, 3, 5...), turn LEFT
    // Math trick to turn left: (dir + 3) % 4
    return (currentD + 3) % 4;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int i) {
    
    // Calculate raw distance to target
    int dx = trainDestX[i] - trainX[i];
    int dy = trainDestY[i] - trainY[i];

    // Get absolute values manually (no abs() function allowed)
    int abs_dx = dx;
    if (abs_dx < 0) {
        abs_dx = -abs_dx; // Make it positive
    }

    int abs_dy = dy;
    if (abs_dy < 0) {
        abs_dy = -abs_dy; // Make it positive
    }

    // Logic: Move in the direction of the biggest gap
    if (abs_dx > abs_dy) {
        // We need to move horizontally
        if (dx > 0) {
            return 1; // Target is to the Right
        } else {
            return 3; // Target is to the Left
        }
    } else {
        // We need to move vertically
        if (dy > 0) {
            return 2; // Target is Down
        } else {
            return 0; // Target is Up
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    // Just loop through every train and figure out where it wants to go
    for (int i = 0; i < numTrains; i++) {
        if (trainActive[i] == 1) {
            determineNextPosition(i);
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    for (int i = 0; i < numTrains; i++) {
        
        // Skip inactive trains
        if (trainActive[i] == 0) continue;

        // Apply the move
        trainX[i] = nextX[i];
        trainY[i] = nextY[i];
        trainDir[i] = nextDir[i];

        // Check if we just landed on a switch
        char tile = grid[trainY[i]][trainX[i]];
        if (tile >= 'A' && tile <= 'Z') {
            int idx = getSwitchIndex(tile);
            
            // If it is a valid switch, increase the counter so it flips for the next guy
            if (idx >= 0) {
                switchCounter[idx] = switchCounter[idx] + 1;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    
    // Loop through every train (Train A)
    for (int i = 0; i < numTrains; i++) {
        
        // Loop through every other train (Train B)
        for (int j = i + 1; j < numTrains; j++) {

            // We only care if BOTH trains are currently active
            if (trainActive[i] == 1 && trainActive[j] == 1) {

                // Check if both trains are trying to move to the exact same square
                if (nextX[i] == nextX[j] && nextY[i] == nextY[j]) {

                    // ---------------------------------------------
                    // STEP 1: Calculate distance for Train i
                    // ---------------------------------------------
                    
                    int xDist_i;
                    if (trainX[i] > trainDestX[i]) {
                        xDist_i = trainX[i] - trainDestX[i];
                    } else {
                        xDist_i = trainDestX[i] - trainX[i];
                    }

                    int yDist_i;
                    if (trainY[i] > trainDestY[i]) {
                        yDist_i = trainY[i] - trainDestY[i];
                    } else {
                        yDist_i = trainDestY[i] - trainY[i];
                    }

                    int totalDist_i = xDist_i + yDist_i;

                    // ---------------------------------------------
                    // STEP 2: Calculate distance for Train j
                    // ---------------------------------------------
                    
                    int xDist_j;
                    if (trainX[j] > trainDestX[j]) {
                        xDist_j = trainX[j] - trainDestX[j];
                    } else {
                        xDist_j = trainDestX[j] - trainX[j];
                    }

                    int yDist_j;
                    if (trainY[j] > trainDestY[j]) {
                        yDist_j = trainY[j] - trainDestY[j];
                    } else {
                        yDist_j = trainDestY[j] - trainY[j];
                    }

                    int totalDist_j = xDist_j + yDist_j;

                    // ---------------------------------------------
                    // STEP 3: Compare distances to decide priority
                    // ---------------------------------------------

                    // If Train i is further away, it should wait
                    if (totalDist_i > totalDist_j) {
                        nextX[i] = trainX[i]; // Reset position to current (don't move)
                        nextY[i] = trainY[i];
                        trainWait[i]++;       // Mark that it is waiting
                    }
                    // If Train j is further away, it should wait
                    else if (totalDist_j > totalDist_i) {
                        nextX[j] = trainX[j]; 
                        nextY[j] = trainY[j];
                        trainWait[j]++;
                    }
                    // If they are the same distance, they crash
                    else {
                        trainCrashed[i] = 1;
                        trainCrashed[j] = 1;
                        trainActive[i] = 0; // Turn them off
                        trainActive[j] = 0;
                        trainsCrashed = trainsCrashed + 2; 
                    }
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for (int i = 0; i < numTrains; i++) {

        if (trainActive[i] == 0) continue;

        // Check if coords match destination coords
        if (trainX[i] == trainDestX[i] && trainY[i] == trainDestY[i]) {
            
            // Mark as arrived
            trainArrived[i] = 1;
            
            // Remove from map
            trainActive[i] = 0;
            
            // Increase score
            trainsDelivered = trainsDelivered + 1;
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
    // If emergency mode is not on, do nothing
    if (emergencyActive == 0) return;

    // Loop through all trains and stop them
    for (int i = 0; i < numTrains; i++) {
        if (trainActive[i] == 1) {
            // Overwrite the planned move with current position
            nextX[i] = trainX[i];
            nextY[i] = trainY[i];
            
            // Mark as waiting
            trainWait[i]++;
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
    if (emergencyActive == 1) {
        emergencyTicks = emergencyTicks - 1;
        
        // If time runs out, turn off emergency
        if (emergencyTicks <= 0) {
            emergencyActive = 0;
        }
    }
}