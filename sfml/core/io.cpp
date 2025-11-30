#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile() {
    FILE* file = fopen("data/levels/easy_level.lvl", "r");
    if (!file) {
        cout << "Cannot open level file" << endl;
        return false;
    }

    char line[512];
    int mode = 0;
    int map_y = 0;

    initializeSimulationState(); // ← FIXED: was init_simulation

    while (fgets(line, 512, file)) {
        if (line[0] == '\n') continue;

        if (strncmp(line, "MAP:", 4) == 0) { mode = 1; continue; }
        if (strncmp(line, "SWITCHES:", 9) == 0) { mode = 2; continue; }
        if (strncmp(line, "TRAINS:", 7) == 0) { mode = 3; continue; }

        if (mode == 1) {  // MAP
            for (int c = 0; c < (int)strlen(line) && c < max_cols; c++) {  // ← FIXED: COLS → max_cols
                grid[map_y][c] = line[c];
            }
            map_y++;
        }

        else if (mode == 2 && numSwitches < max_switches) {  // ← FIXED: s_count → numSwitches, MAX_SWITCHES → max_switches
            // SWITCHES - just parse the letter and k-value
            char letter;
            char modeStr[20];
            int kVal;
            
            if (sscanf(line, "%c %s %*d %d", &letter, modeStr, &kVal) == 3) {
                switchLetter[numSwitches] = letter;
                switchKValue[numSwitches] = kVal;
                switchStates[numSwitches] = 0;
                switchCounter[numSwitches] = 0;
                numSwitches++;
            }
        }

        else if (mode == 3 && numTrains < max_trains) {  // ← FIXED: t_count → numTrains, MAX_TRAINS → max_trains
            int tick, x, y, dir, col;
            if (sscanf(line, "%d %d %d %d %d", &tick, &x, &y, &dir, &col) != 5)
                sscanf(line, "%d %d %d %d", &tick, &x, &y, &dir), col = 0;

            trainSpawnTick[numTrains] = tick;  // ← FIXED: t_spawn_tick → trainSpawnTick
            startX[numTrains] = x;              // ← FIXED: t_x → startX
            startY[numTrains] = y;              // ← FIXED: t_y → startY
            startDir[numTrains] = dir;          // ← FIXED: t_dir → startDir
            trainActive[numTrains] = 0;         // ← FIXED: t_state → trainActive (0 = not spawned yet)
            numTrains++;
        }
    }

    // Assign simple destination for each train
    for (int i = 0; i < numTrains; i++) {
        trainDestX[i] = startX[i];                           // ← FIXED: t_target_x → trainDestX
        trainDestY[i] = (startY[i] + 5) % max_rows;        // ← FIXED: ROWS → max_rows
    }

    fclose(file);
    return true;
}

// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {
    FILE* f = fopen("trace.csv", "w");
    if (f) { fprintf(f, "Tick,TrainID,X,Y,State\n"); fclose(f); }

    f = fopen("switches.csv", "w");
    if (f) { fprintf(f, "Tick,SwitchID,Mode,State\n"); fclose(f); }

    f = fopen("signals.csv", "w");
    if (f) { fprintf(f, "Tick,SwitchID,Signal\n"); fclose(f); }

    f = fopen("metrics.txt", "w");
    if (f) { fprintf(f, "Simulation Metrics\n"); fclose(f); }
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace() {
    FILE* f = fopen("trace.csv", "a");
    if (!f) return;

    for (int i = 0; i < numTrains; i++) {              // ← FIXED: t_count → numTrains
        if (trainActive[i] == 1) {                     // ← FIXED: t_state → trainActive
            fprintf(f, "%d,%d,%d,%d,%d\n", 
                    currentTick,                        // ← FIXED: current_tick → currentTick
                    i, 
                    trainX[i],                          // ← FIXED: t_x → trainX
                    trainY[i],                          // ← FIXED: t_y → trainY
                    trainActive[i]);                    // ← FIXED: t_state → trainActive
        }
    }

    fclose(f);
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchStates() {
    FILE* f = fopen("switches.csv", "a");
    if (!f) return;

    for (int i = 0; i < numSwitches; i++) {            // ← FIXED: s_count → numSwitches
        fprintf(f, "%d,%d,%d,%d\n", 
                currentTick,                            // ← FIXED: current_tick → currentTick
                i, 
                0,                                      // ← FIXED: s_mode (placeholder)
                switchStates[i]);                        // ← FIXED: s_turned → switchState
    }

    fclose(f);
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState() {
    FILE* f = fopen("signals.csv", "a");
    if (!f) return;

    for (int i = 0; i < numSwitches; i++) {            // ← FIXED: s_count → numSwitches
        int signal = 0; // just green
        fprintf(f, "%d,%d,%d\n", 
                currentTick,                            // ← FIXED: current_tick → currentTick
                i, 
                signal);
    }

    fclose(f);
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
    FILE* f = fopen("metrics.txt", "w");
    if (!f) return;

    fprintf(f, "Total Ticks: %d\n", currentTick);      // ← FIXED: current_tick → currentTick
    fprintf(f, "Trains Arrived: %d\n", trainsDelivered); // ← FIXED: trains_arrived → trainsDelivered
    fprintf(f, "Trains Crashed: %d\n", trainsCrashed);   // ← FIXED: trains_crashed → trainsCrashed

    fclose(f);
}