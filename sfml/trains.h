#ifndef TRAINS_H
#define TRAINS_H

// ============================================================================
// TRAINS.H - Train logic
// ============================================================================

// ----------------------------------------------------------------------------
// TRAIN SPAWNING
// ----------------------------------------------------------------------------
// Spawn trains scheduled for the current tick.
void spawnTrainsForTick();

// ----------------------------------------------------------------------------
// TRAIN ROUTING
// ----------------------------------------------------------------------------
// Compute routes for all trains (Phase 2).
void determineAllRoutes();

// Compute next position/direction for a train.
bool determineNextPosition(int train_index);

// Get next direction on entering a tile.
// FIXED: Removed nextX and nextY because the .cpp file doesn't use them
int getNextDirection(int train_index);

// Choose best direction at a crossing.
// FIXED: Removed nextX and nextY because the .cpp file doesn't use them
int getSmartDirectionAtCrossing(int train_index);

// ----------------------------------------------------------------------------
// TRAIN MOVEMENT
// ----------------------------------------------------------------------------
// Move trains and handle collisions (Phase 5).
void moveAllTrains();

// ----------------------------------------------------------------------------
// COLLISION DETECTION
// ----------------------------------------------------------------------------
// Detect trains targeting the same tile/swap/crossing.
void detectCollisions();

// ----------------------------------------------------------------------------
// ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
void checkArrivals();

// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply emergency halt in active zone.
void applyEmergencyHalt();

// Update emergency halt timer.
void updateEmergencyHalt();

#endif