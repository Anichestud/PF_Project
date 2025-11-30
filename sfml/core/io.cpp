#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

bool loadLevelFile() {
    FILE* file = fopen("data/levels/easy_level.lvl", "r");
    if (!file) {
        cout << "Cannot open level file" << endl;
        return false;
    }

    char line[512];
    int mode = 0;
    int map_y = 0;

    initializeSimulationState();

    while (fgets(line, 512, file)) {
        if (line[0] == '\n') continue;

        if (strncmp(line, "MAP:", 4) == 0) { mode = 1; continue; }
        if (strncmp(line, "SWITCHES:", 9) == 0) { mode = 2; continue; }
        if (strncmp(line, "TRAINS:", 7) == 0) { mode = 3; continue; }

        if (mode == 1) {
            for (int c = 0; c < (int)strlen(line) && c < max_cols; c++) {  // ← FIX 1: MAX_COLS → max_cols
                grid[map_y][c] = line[c];
            }
            map_y++;
        }

        else if (mode == 2 && numSwitches < max_switches) {  // ← FIX 2: MAX_SWITCHES → max_switches
            char letter;
            char modeStr[20];
            int kVal;
            
            if (sscanf(line, "%c %s %*d %d", &letter, modeStr, &kVal) == 3) {
                switchLetter[numSwitches] = letter;
                switchKValue[numSwitches] = kVal;
                switchState[numSwitches] = 0;
                switchCounter[numSwitches] = 0;
                numSwitches++;
            }
        }

        else if (mode == 3 && numTrains < max_trains) {  // ← FIX 3: MAX_TRAINS → max_trains
            int tick, x, y, dir, col = 0;
            if (sscanf(line, "%d %d %d %d %d", &tick, &x, &y, &dir, &col) != 5) {
                sscanf(line, "%d %d %d %d", &tick, &x, &y, &dir);
            }

            trainSpawnTick[numTrains] = tick;
            startX[numTrains] = x;
            startY[numTrains] = y;
            startDir[numTrains] = dir;
            trainActive[numTrains] = 0;
            numTrains++;
        }
    }

    for (int i = 0; i < numTrains; i++) {
        trainDestX[i] = startX[i];
        trainDestY[i] = (startY[i] + 5) % max_rows;  // ← FIX 4: MAX_ROWS → max_rows
    }

    fclose(file);
    return true;
}

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

void logTrainTrace() {
    FILE* f = fopen("trace.csv", "a");
    if (!f) return;

    for (int i = 0; i < numTrains; i++) {
        if (trainActive[i] == 1) {
            fprintf(f, "%d,%d,%d,%d,%d\n", 
                    currentTick, i, trainX[i], trainY[i], trainActive[i]);
        }
    }

    fclose(f);
}

void logSwitchStates() {  // ← FIX 5: Function name was logSwitchStates, should be logSwitchState
    FILE* f = fopen("switches.csv", "a");
    if (!f) return;

    for (int i = 0; i < numSwitches; i++) {
        fprintf(f, "%d,%d,%d,%d\n", 
                currentTick, i, 0, switchState[i]);
    }

    fclose(f);
}

void logSignalState() {
    FILE* f = fopen("signals.csv", "a");
    if (!f) return;

    for (int i = 0; i < numSwitches; i++) {
        fprintf(f, "%d,%d,%d\n", currentTick, i, 0);
    }

    fclose(f);
}

void writeMetrics() {
    FILE* f = fopen("metrics.txt", "w");
    if (!f) return;

    fprintf(f, "Total Ticks: %d\n", currentTick);
    fprintf(f, "Trains Arrived: %d\n", trainsDelivered);
    fprintf(f, "Trains Crashed: %d\n", trainsCrashed);

    fclose(f);
}