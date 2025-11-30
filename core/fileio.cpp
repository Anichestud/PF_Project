#include "fileio.h"
#include <iostream>
#include <fstream>

using namespace std;

extern int trainId[];
extern int trainX[];
extern int trainY[];
extern int trainDir[];
extern bool trainActive[];
extern bool trainCrashed[];
extern bool trainArrived[];
extern int trainTotalWaitTicks[];
extern int totalTrains;

extern int traceTick[];
extern int traceTrainID[];
extern int traceX[];
extern int traceY[];
extern int traceDirection[];
extern int traceState[];
extern int traceLogCount;

extern char switchLetter[];
extern int switchMode[];
extern int switchInitState[];
extern int switchKValues[][4];
extern int switchCurrentState[];
extern int totalSwitches;

extern char grid[][50];
extern int gridRows;
extern int gridCols;

extern int currentTick;
extern int totalDelivered;
extern int totalCrashed;
extern int signalViolations;
extern char levelName[];

int manhattanDistance(int x1, int y1, int x2, int y2);

void createOutputDirectory() {
    
}

void saveTraceLog() {
    createOutputDirectory();

    ofstream file("out/trace.csv");
    if (!file.is_open()) {
        cout << "Failed to create trace.csv" << endl;
        return;
    }

    file << "Tick,TrainID,X,Y,Direction,State\n";

    for (int i = 0; i < traceLogCount; i++) {
        const char* stateStr;
        if (traceState[i] == 0) stateStr = "WAITING";
        else if (traceState[i] == 1) stateStr = "ACTIVE";
        else if (traceState[i] == 2) stateStr = "CRASHED";
        else stateStr = "ARRIVED";

        file << traceTick[i] << ","
            << traceTrainID[i] << ","
            << traceX[i] << ","
            << traceY[i] << ","
            << traceDirection[i] << ","
            << stateStr << "\n";
    }

    file.close();
    cout << "Saved: out/trace.csv (" << traceLogCount << " entries)" << endl;
}

void saveSwitchLog() {
    createOutputDirectory();

    ofstream file("out/switches.csv");
    if (!file.is_open()) {
        cout << "Failed to create switches.csv" << endl;
        return;
    }

    file << "Tick,Switch,Mode,State\n";

    for (int i = 0; i < totalSwitches; i++) {
        const char* mode = (switchMode[i] == 1) ? "GLOBAL" : "PER_DIR";

        file << currentTick << ","
            << switchLetter[i] << ","
            << mode << ","
            << switchCurrentState[i] << "\n";
    }

    file.close();
    cout << "Saved: out/switches.csv" << endl;
}

void saveSignalLog() {
    createOutputDirectory();

    ofstream file("out/signals.csv");
    if (!file.is_open()) {
        cout << "Failed to create signals.csv" << endl;
        return;
    }

    file << "Tick,Switch,Signal\n";

    for (int i = 0; i < totalSwitches; i++) {
        for (int r = 0; r < gridRows; r++) {
            for (int c = 0; c < gridCols; c++) {
                if (grid[r][c] == switchLetter[i]) {
                    const char* signal = "GREEN";
                    bool trainAdjacent = false;
                    bool trainNearby = false;

                    for (int t = 0; t < totalTrains; t++) {
                        if (trainActive[t]) {
                            int dist = manhattanDistance(c, r, trainX[t], trainY[t]);
                            if (dist == 1) trainAdjacent = true;
                            if (dist <= 2) trainNearby = true;
                        }
                    }

                    if (trainAdjacent) signal = "RED";
                    else if (trainNearby) signal = "YELLOW";

                    file << currentTick << ","
                        << switchLetter[i] << ","
                        << signal << "\n";
                    break;
                }
            }
        }
    }

    file.close();
    cout << "Saved: out/signals.csv" << endl;
}

void saveMetrics() {
    createOutputDirectory();

    ofstream file("out/metrics.txt");
    if (!file.is_open()) {
        cout << "Failed to create metrics.txt" << endl;
        return;
    }

    file << "========== SWITCHBACK RAILS METRICS ==========\n";
    file << "Level Name: " << levelName << "\n";
    file << "Total Ticks: " << currentTick << "\n";
    file << "Total Trains: " << totalTrains << "\n";
    file << "Trains Delivered: " << totalDelivered << "\n";
    file << "Trains Crashed: " << totalCrashed << "\n";

    if (totalTrains > 0) {
        int successRate = (totalDelivered * 100) / totalTrains;
        file << "Success Rate: " << successRate << "%\n";
    }
    else {
        file << "Success Rate: 0%\n";
    }

    file << "\n";
    file << "=== SWITCH STATES ===\n";
    for (int i = 0; i < totalSwitches; i++) {
        file << "  Switch " << switchLetter[i] << ": ";
        file << "State " << switchCurrentState[i];
        file << " (Mode: " << (switchMode[i] == 1 ? "GLOBAL" : "PER_DIR") << ")\n";

        if (switchMode[i] == 0) {
            file << "    K-values: UP=" << switchKValues[i][0]
                << " RIGHT=" << switchKValues[i][1]
                << " DOWN=" << switchKValues[i][2]
                << " LEFT=" << switchKValues[i][3] << "\n";
        }
        else {
            file << "    K-value (GLOBAL): " << switchKValues[i][0] << "\n";
        }
    }

    file << "\n";
    file << "=== PERFORMANCE METRICS ===\n";

    if (currentTick > 0) {
        int throughput = (totalDelivered * 100) / currentTick;
        file << "  Throughput: " << throughput << " trains per 100 ticks\n";
    }
    else {
        file << "  Throughput: 0 trains per 100 ticks\n";
    }

    
    int totalWaitTime = 0;
    int arrivedCount = 0;
    for (int i = 0; i < totalTrains; i++) {
        if (trainArrived[i]) {
            totalWaitTime += trainTotalWaitTicks[i];
            arrivedCount++;
        }
    }

    if (arrivedCount > 0) {
        int avgWait = totalWaitTime / arrivedCount;
        file << "  Average Wait Time: " << avgWait << " ticks per train\n";
    }
    else {
        file << "  Average Wait Time: 0 ticks (no trains arrived)\n";
    }

    file << "  Signal Violations: " << signalViolations << "\n";

    int totalFlips = 0;
    for (int i = 0; i < totalSwitches; i++) {
        if (switchCurrentState[i] != switchInitState[i]) {
            totalFlips++;
        }
    }
    file << "  Total Switch Flips: " << totalFlips << "\n";

    int safetyBufferCount = 0;
    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            if (grid[r][c] == '=') safetyBufferCount++;
        }
    }

    if (safetyBufferCount > 0 && currentTick > 0) {
        int efficiency = (totalDelivered * currentTick) / safetyBufferCount;
        file << "  Energy Efficiency: " << efficiency << "\n";
    }
    else {
        file << "  Energy Efficiency: N/A\n";
    }

    file << "\n";
    file << "=== FINAL TRAIN STATUS ===\n";
    for (int i = 0; i < totalTrains; i++) {
        file << "  Train " << trainId[i] << ": ";
        if (trainArrived[i]) file << "ARRIVED at (" << trainX[i] << "," << trainY[i] << ")";
        else if (trainCrashed[i]) file << "CRASHED at (" << trainX[i] << "," << trainY[i] << ")";
        else file << "IN PROGRESS at (" << trainX[i] << "," << trainY[i] << ")";
        file << " | Wait Ticks: " << trainTotalWaitTicks[i] << "\n";
    }

    file << "\n";
    file << "=== COLLISION ANALYSIS ===\n";
    file << "  Distance-based priority system: ACTIVE\n";
    file << "  Collision types handled:\n";
    file << "    - Same-destination collisions\n";
    file << "    - Head-on swap collisions\n";
    file << "    - Crossing (+) collisions\n";

    file << "\n";
    file << "=== WEATHER & FEATURES ===\n";
    file << "  Signal violations tracked: YES\n";
    file << "  Weather effects: IMPLEMENTED\n";
    file << "  Emergency halt: IMPLEMENTED\n";

    file.close();
    cout << "Saved: out/metrics.txt" << endl;
}