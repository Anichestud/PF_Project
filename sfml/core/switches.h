#ifndef SWITCHES_H
#define SWITCHES_H

void updateSwitchCounters();
void queueSwitchFlips();
void applyDeferredFlips();
void logSwitchStates();  // ← FIXED: was logSwitchState
void updateSignalLights();
void toggleSwitchState(int switchIndex);
int getSwitchStateForDirection(int switchIndex, int currentDir);

#endif