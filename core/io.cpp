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

    init_simulation(); // reset all counts and arrays

    while (fgets(line, 512, file)) {
        if (line[0] == '\n') continue;

        if (strncmp(line, "MAP:", 4) == 0) { mode = 1; continue; }
        if (strncmp(line, "SWITCHES:", 9) == 0) { mode = 2; continue; }
        if (strncmp(line, "TRAINS:", 7) == 0) { mode = 3; continue; }

        if (mode == 1) {  // MAP
            for (int c = 0; c < (int)strlen(line) && c < COLS; c++) {
                grid[map_y][c] = line[c];
            }
            map_y++;
        }

        else if (mode == 2 && s_count < MAX_SWITCHES) {  // SWITCHES
            s_x[s_count] = 0;
            s_y[s_count] = 0;
            s_mode[s_count] = 1;
            for (int k = 0; k < 4; k++) s_k[s_count][k] = 3;
            s_turned[s_count] = 0;
            s_count++;
        }

        else if (mode == 3 && t_count < MAX_TRAINS) {  // TRAINS
            int tick, x, y, dir, col;
            if (sscanf(line, "%d %d %d %d %d", &tick, &x, &y, &dir, &col) != 5)
                sscanf(line, "%d %d %d %d", &tick, &x, &y, &dir), col = 0;

            t_spawn_tick[t_count] = tick;
            t_x[t_count] = x;
            t_y[t_count] = y;
            t_dir[t_count] = dir;
            t_color[t_count] = col;
            t_state[t_count] = 0; // WAIT
            t_id[t_count] = t_count;
            t_count++;
        }
    }

    // Assign simple destination for each train
    for (int i = 0; i < t_count; i++) {
        t_target_x[i] = t_x[i];
        t_target_y[i] = (t_y[i] + 5) % ROWS; // just a simple target
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

    for (int i = 0; i < t_count; i++) {
        if (t_state[i] == 1) {
            fprintf(f, "%d,%d,%d,%d,%d\n", current_tick, i, t_x[i], t_y[i], t_state[i]);
        }
    }

    fclose(f);
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState() {
    FILE* f = fopen("switches.csv", "a");
    if (!f) return;

    for (int i = 0; i < s_count; i++) {
        fprintf(f, "%d,%d,%d,%d\n", current_tick, i, s_mode[i], s_turned[i]);
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

    for (int i = 0; i < s_count; i++) {
        int signal = 0; // just green
        fprintf(f, "%d,%d,%d\n", current_tick, i, signal);
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

    fprintf(f, "Total Ticks: %d\n", current_tick);
    fprintf(f, "Trains Arrived: %d\n", trains_arrived);
    fprintf(f, "Trains Crashed: %d\n", trains_crashed);

    fclose(f);
}
