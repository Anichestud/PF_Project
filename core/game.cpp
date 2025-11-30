#include "game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int trainId[MAX_TRAINS];
int trainX[MAX_TRAINS];
int trainY[MAX_TRAINS];
int trainDir[MAX_TRAINS];
int trainColor[MAX_TRAINS];
int trainSpawnTick[MAX_TRAINS];
int trainDestX[MAX_TRAINS];
int trainDestY[MAX_TRAINS];
bool trainActive[MAX_TRAINS];
bool trainCrashed[MAX_TRAINS];
bool trainArrived[MAX_TRAINS];
int trainWaitTicks[MAX_TRAINS];
int trainTotalWaitTicks[MAX_TRAINS];
int trainMovesSinceRain[MAX_TRAINS];
int totalTrains = 0;

int trainPrevX[MAX_TRAINS];
int trainPrevY[MAX_TRAINS];

int traceTick[MAX_TRACE_ENTRIES];
int traceTrainID[MAX_TRACE_ENTRIES];
int traceX[MAX_TRACE_ENTRIES];
int traceY[MAX_TRACE_ENTRIES];
int traceDirection[MAX_TRACE_ENTRIES];
int traceState[MAX_TRACE_ENTRIES];
int traceLogCount = 0;

char switchLetter[MAX_SWITCHES];
int switchMode[MAX_SWITCHES];
int switchInitState[MAX_SWITCHES];
int switchKValues[MAX_SWITCHES][4];
int switchCurrentState[MAX_SWITCHES];
int switchCounters[MAX_SWITCHES][4];
int switchGlobalCounter[MAX_SWITCHES];
bool switchFlipQueued[MAX_SWITCHES];
int switchEmergencyHaltTicks[MAX_SWITCHES];
int totalSwitches = 0;

char grid[MAX_ROWS][MAX_COLS];
int gridRows = 0;
int gridCols = 0;

int currentTick = 0;
int totalDelivered = 0;
int totalCrashed = 0;
int seed = 0;
bool isPaused = false;
int weatherMode = WEATHER_NORMAL;
int signalViolations = 0;
char levelName[100];

void addTraceEntry(int trainIdx) {
    if (traceLogCount < MAX_TRACE_ENTRIES) {
        traceTick[traceLogCount] = currentTick;
        traceTrainID[traceLogCount] = trainId[trainIdx];
        traceX[traceLogCount] = trainX[trainIdx];
        traceY[traceLogCount] = trainY[trainIdx];
        traceDirection[traceLogCount] = trainDir[trainIdx];

        if (trainCrashed[trainIdx]) traceState[traceLogCount] = 2;
        else if (trainArrived[trainIdx]) traceState[traceLogCount] = 3;
        else if (!trainActive[trainIdx]) traceState[traceLogCount] = 0;
        else traceState[traceLogCount] = 1;

        traceLogCount++;
    }
}

void initGame() {
    for (int i = 0; i < MAX_TRAINS; i++) {
        trainId[i] = 0;
        trainX[i] = 0;
        trainY[i] = 0;
        trainPrevX[i] = 0;
        trainPrevY[i] = 0;
        trainDir[i] = 0;
        trainColor[i] = 0;
        trainSpawnTick[i] = 0;
        trainDestX[i] = -1;
        trainDestY[i] = -1;
        trainActive[i] = false;
        trainCrashed[i] = false;
        trainArrived[i] = false;
        trainWaitTicks[i] = 0;
        trainTotalWaitTicks[i] = 0;
        trainMovesSinceRain[i] = 0;
    }

    for (int i = 0; i < MAX_SWITCHES; i++) {
        switchLetter[i] = ' ';
        switchMode[i] = 0;
        switchInitState[i] = 0;
        switchCurrentState[i] = 0;
        switchGlobalCounter[i] = 0;
        switchFlipQueued[i] = false;
        switchEmergencyHaltTicks[i] = 0;
        for (int j = 0; j < 4; j++) {
            switchKValues[i][j] = 0;
            switchCounters[i][j] = 0;
        }
    }

    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COLS; c++) {
            grid[r][c] = ' ';
        }
    }

    totalTrains = 0;
    totalSwitches = 0;
    currentTick = 0;
    totalDelivered = 0;
    totalCrashed = 0;
    traceLogCount = 0;
    weatherMode = WEATHER_NORMAL;
    signalViolations = 0;
    levelName[0] = '\0';
}

void loadLevel(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open level file!" << endl;
        return;
    }

    char buffer[256];

    file >> buffer;
    file.getline(levelName, 100);

    int nameStart = 0;
    while (levelName[nameStart] == ' ') nameStart++;
    if (nameStart > 0) {
        int i = 0;
        while (levelName[nameStart + i] != '\0') {
            levelName[i] = levelName[nameStart + i];
            i++;
        }
        levelName[i] = '\0';
    }

    file >> buffer >> gridRows;
    file >> buffer >> gridCols;
    file >> buffer >> seed;
    file.getline(buffer, 256);

    file >> buffer;
    file >> buffer;
    if (buffer[0] == 'R' || buffer[0] == 'r') {
        weatherMode = WEATHER_RAIN;
    }
    else if (buffer[0] == 'F' || buffer[0] == 'f') {
        weatherMode = WEATHER_FOG;
    }
    else {
        weatherMode = WEATHER_NORMAL;
    }

    file.getline(buffer, 256);
    file.getline(buffer, 256);

    for (int r = 0; r < gridRows; r++) {
        file.getline(buffer, 256);
        for (int c = 0; c < gridCols && c < 256; c++) {
            grid[r][c] = buffer[c];
        }
    }

    int destIndex = 0;
    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            if (grid[r][c] == 'D') {
                if (destIndex < MAX_TRAINS) {
                    trainDestX[destIndex] = c;
                    trainDestY[destIndex] = r;
                    destIndex++;
                }
            }
        }
    }

    file.getline(buffer, 256);

    while (file.peek() != 'T' && !file.eof()) {
        char letter;
        char modeStr[20];
        int init, k0, k1, k2, k3;
        char label0[20], label1[20];

        file >> letter >> modeStr >> init >> k0 >> k1 >> k2 >> k3 >> label0 >> label1;
        if (file.fail()) break;

        switchLetter[totalSwitches] = letter;
        switchMode[totalSwitches] = (modeStr[0] == 'G') ? 1 : 0;
        switchInitState[totalSwitches] = init;
        switchCurrentState[totalSwitches] = init;
        switchKValues[totalSwitches][0] = k0;
        switchKValues[totalSwitches][1] = k1;
        switchKValues[totalSwitches][2] = k2;
        switchKValues[totalSwitches][3] = k3;
        switchEmergencyHaltTicks[totalSwitches] = 0;

        totalSwitches++;
        file.getline(buffer, 256);
    }

    file.getline(buffer, 256);

    while (!file.eof() && totalTrains < MAX_TRAINS) {
        int tick, x, y, dir, color;
        file >> tick >> x >> y >> dir >> color;
        if (file.fail()) break;

        trainId[totalTrains] = totalTrains;
        trainSpawnTick[totalTrains] = tick;
        trainX[totalTrains] = x;
        trainY[totalTrains] = y;
        trainDir[totalTrains] = dir;
        trainColor[totalTrains] = color;
        trainDestX[totalTrains] = trainDestX[totalTrains % destIndex];
        trainDestY[totalTrains] = trainDestY[totalTrains % destIndex];
        totalTrains++;
    }

    file.close();
    cout << "Level loaded: " << levelName << " (" << gridRows << "x" << gridCols << ")" << endl;
    cout << "Weather: " << (weatherMode == WEATHER_RAIN ? "RAIN" :
        weatherMode == WEATHER_FOG ? "FOG" : "NORMAL") << endl;
    cout << "Trains: " << totalTrains << " Switches: " << totalSwitches << endl;
}

void spawnTrains() {
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i] && !trainCrashed[i] && !trainArrived[i]) {
            if (trainSpawnTick[i] == currentTick) {
                bool occupied = false;
                for (int j = 0; j < totalTrains; j++) {
                    if (i != j && trainActive[j]) {
                        if (trainX[j] == trainX[i] && trainY[j] == trainY[i]) {
                            occupied = true;
                            break;
                        }
                    }
                }

                if (!occupied) {
                    trainActive[i] = true;
                    trainPrevX[i] = trainX[i];
                    trainPrevY[i] = trainY[i];
                    addTraceEntry(i);
                    cout << "Train " << trainId[i] << " spawned at ("
                        << trainX[i] << "," << trainY[i] << ")" << endl;
                }
                else {
                    trainSpawnTick[i]++;
                }
            }
        }
    }
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

int getNextX(int x, int dir) {
    if (dir == DIR_LEFT) return x - 1;
    if (dir == DIR_RIGHT) return x + 1;
    return x;
}

int getNextY(int y, int dir) {
    if (dir == DIR_UP) return y - 1;
    if (dir == DIR_DOWN) return y + 1;
    return y;
}

bool isValidTile(int x, int y) {
    if (x < 0 || x >= gridCols || y < 0 || y >= gridRows) return false;
    char tile = grid[y][x];
    return tile != ' ' && tile != '.';
}

int findSwitchIndex(char letter) {
    for (int i = 0; i < totalSwitches; i++) {
        if (switchLetter[i] == letter) return i;
    }
    return -1;
}

int getOppositeDirection(int dir) {
    if (dir == DIR_UP) return DIR_DOWN;
    if (dir == DIR_DOWN) return DIR_UP;
    if (dir == DIR_LEFT) return DIR_RIGHT;
    if (dir == DIR_RIGHT) return DIR_LEFT;
    return dir;
}

int checkSignalAtSwitch(int switchIdx) {
    char letter = switchLetter[switchIdx];

    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            if (grid[r][c] == letter) {
                int signalColor = 0;

                for (int t = 0; t < totalTrains; t++) {
                    if (trainActive[t]) {
                        int dist = manhattanDistance(c, r, trainX[t], trainY[t]);
                        if (dist == 1) {
                            return 2;
                        }
                        else if (dist <= 2) {
                            signalColor = 1;
                        }
                    }
                }

                return signalColor;
            }
        }
    }

    return 0;
}

void determineRoutes() {
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i]) continue;
        if (trainWaitTicks[i] > 0) continue;

        char currentTile = grid[trainY[i]][trainX[i]];
        int currentDir = trainDir[i];

        if (currentTile == '/') {
            if (currentDir == DIR_RIGHT) trainDir[i] = DIR_UP;
            else if (currentDir == DIR_DOWN) trainDir[i] = DIR_LEFT;
            else if (currentDir == DIR_LEFT) trainDir[i] = DIR_DOWN;
            else if (currentDir == DIR_UP) trainDir[i] = DIR_RIGHT;
        }
        else if (currentTile == '\\') {
            if (currentDir == DIR_RIGHT) trainDir[i] = DIR_DOWN;
            else if (currentDir == DIR_UP) trainDir[i] = DIR_LEFT;
            else if (currentDir == DIR_LEFT) trainDir[i] = DIR_UP;
            else if (currentDir == DIR_DOWN) trainDir[i] = DIR_RIGHT;
        }
        else if (currentTile >= 'A' && currentTile <= 'Z') {
            int swIdx = findSwitchIndex(currentTile);
            if (swIdx != -1) {
                int signal = checkSignalAtSwitch(swIdx);
                if (signal == 2) {
                    signalViolations++;
                }

                int state = switchCurrentState[swIdx];
                if (state == 1) {
                    if (currentDir == DIR_RIGHT) trainDir[i] = DIR_DOWN;
                    else if (currentDir == DIR_LEFT) trainDir[i] = DIR_UP;
                    else if (currentDir == DIR_UP) trainDir[i] = DIR_RIGHT;
                    else if (currentDir == DIR_DOWN) trainDir[i] = DIR_LEFT;
                }
            }
        }
    }
}

void updateCounters() {
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i]) continue;

        char tile = grid[trainY[i]][trainX[i]];

        if (tile >= 'A' && tile <= 'Z') {
            int swIdx = findSwitchIndex(tile);
            if (swIdx != -1) {
                if (switchMode[swIdx] == 1) {
                    switchGlobalCounter[swIdx]++;
                }
                else {
                    switchCounters[swIdx][trainDir[i]]++;
                }
            }
        }
    }
}

void queueFlips() {
    for (int i = 0; i < totalSwitches; i++) {
        bool shouldFlip = false;

        if (switchMode[i] == 1) {
            if (switchGlobalCounter[i] >= switchKValues[i][0]) {
                shouldFlip = true;
            }
        }
        else {
            for (int d = 0; d < 4; d++) {
                if (switchCounters[i][d] >= switchKValues[i][d] && switchKValues[i][d] > 0) {
                    shouldFlip = true;
                    break;
                }
            }
        }

        if (shouldFlip) {
            switchFlipQueued[i] = true;
        }
    }
}

void applyEmergencyHalts() {
    for (int i = 0; i < totalSwitches; i++) {
        if (switchEmergencyHaltTicks[i] > 0) {
            for (int r = 0; r < gridRows; r++) {
                for (int c = 0; c < gridCols; c++) {
                    if (grid[r][c] == switchLetter[i]) {
                        for (int t = 0; t < totalTrains; t++) {
                            if (trainActive[t]) {
                                int dx = trainX[t] - c;
                                int dy = trainY[t] - r;
                                if (dx < 0) dx = -dx;
                                if (dy < 0) dy = -dy;

                                if (dx <= 1 && dy <= 1) {
                                    trainWaitTicks[t] = 1;
                                }
                            }
                        }
                    }
                }
            }

            switchEmergencyHaltTicks[i]--;
        }
    }
}

void applyWeatherEffects() {
    if (weatherMode == WEATHER_RAIN) {
        for (int i = 0; i < totalTrains; i++) {
            if (trainActive[i] && !trainCrashed[i]) {
                trainMovesSinceRain[i]++;

                if (trainMovesSinceRain[i] >= 5) {
                    trainWaitTicks[i] = 1;
                    trainMovesSinceRain[i] = 0;
                }
            }
        }
    }
    
}

void resolveCollisions() {
    
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i] || trainCrashed[i] || trainWaitTicks[i] > 0) continue;

        for (int j = i + 1; j < totalTrains; j++) {
            if (!trainActive[j] || trainCrashed[j] || trainWaitTicks[j] > 0) continue;

            if (trainX[i] == trainX[j] && trainY[i] == trainY[j]) {
                int distI = manhattanDistance(trainX[i], trainY[i], trainDestX[i], trainDestY[i]);
                int distJ = manhattanDistance(trainX[j], trainY[j], trainDestX[j], trainDestY[j]);

                if (distI == distJ) {
                    trainCrashed[i] = true;
                    trainCrashed[j] = true;
                    trainActive[i] = false;
                    trainActive[j] = false;
                    totalCrashed += 2;
                    cout << "CRASH: Trains " << trainId[i] << " and " << trainId[j]
                        << " collided (equal distance)!" << endl;
                }
                else if (distI > distJ) {
                    trainX[j] = trainPrevX[j];
                    trainY[j] = trainPrevY[j];
                    trainWaitTicks[j] = 1;
                }
                else {
                    trainX[i] = trainPrevX[i];
                    trainY[i] = trainPrevY[i];
                    trainWaitTicks[i] = 1;
                }
            }
        }
    }

    
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i] || trainCrashed[i] || trainWaitTicks[i] > 0) continue;

        for (int j = i + 1; j < totalTrains; j++) {
            if (!trainActive[j] || trainCrashed[j] || trainWaitTicks[j] > 0) continue;

            if (trainX[i] == trainPrevX[j] && trainY[i] == trainPrevY[j] &&
                trainX[j] == trainPrevX[i] && trainY[j] == trainPrevY[i]) {

                int distI = manhattanDistance(trainX[i], trainY[i], trainDestX[i], trainDestY[i]);
                int distJ = manhattanDistance(trainX[j], trainY[j], trainDestX[j], trainDestY[j]);

                if (distI == distJ) {
                    trainCrashed[i] = true;
                    trainCrashed[j] = true;
                    trainActive[i] = false;
                    trainActive[j] = false;
                    totalCrashed += 2;
                    cout << "CRASH: Trains " << trainId[i] << " and " << trainId[j]
                        << " head-on collision!" << endl;
                }
                else if (distI > distJ) {
                    trainX[j] = trainPrevX[j];
                    trainY[j] = trainPrevY[j];
                    trainWaitTicks[j] = 1;
                }
                else {
                    trainX[i] = trainPrevX[i];
                    trainY[i] = trainPrevY[i];
                    trainWaitTicks[i] = 1;
                }
            }
        }
    }

    
    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            if (grid[r][c] == '+') {
                int trainsAtCrossing[MAX_TRAINS];
                int crossingCount = 0;

                for (int i = 0; i < totalTrains; i++) {
                    if (trainActive[i] && !trainCrashed[i] &&
                        trainX[i] == c && trainY[i] == r && trainWaitTicks[i] == 0) {
                        trainsAtCrossing[crossingCount++] = i;
                    }
                }

                if (crossingCount > 1) {
                    int maxDist = -1;
                    int maxIdx = -1;
                    bool equalMax = false;

                    for (int k = 0; k < crossingCount; k++) {
                        int idx = trainsAtCrossing[k];
                        int dist = manhattanDistance(trainX[idx], trainY[idx],
                            trainDestX[idx], trainDestY[idx]);
                        if (dist > maxDist) {
                            maxDist = dist;
                            maxIdx = idx;
                            equalMax = false;
                        }
                        else if (dist == maxDist) {
                            equalMax = true;
                        }
                    }

                    if (equalMax) {
                        for (int k = 0; k < crossingCount; k++) {
                            int idx = trainsAtCrossing[k];
                            trainCrashed[idx] = true;
                            trainActive[idx] = false;
                        }
                        totalCrashed += crossingCount;
                        cout << "CRASH: Multiple trains at crossing!" << endl;
                    }
                    else {
                        for (int k = 0; k < crossingCount; k++) {
                            int idx = trainsAtCrossing[k];
                            if (idx != maxIdx) {
                                trainX[idx] = trainPrevX[idx];
                                trainY[idx] = trainPrevY[idx];
                                trainWaitTicks[idx] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void moveTrains() {
    for (int i = 0; i < totalTrains; i++) {
        trainPrevX[i] = trainX[i];
        trainPrevY[i] = trainY[i];
    }

    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i] || trainCrashed[i]) continue;

        if (trainWaitTicks[i] > 0) {
            trainWaitTicks[i]--;
            trainTotalWaitTicks[i]++;
            addTraceEntry(i);
            continue;
        }

        char currentTile = grid[trainY[i]][trainX[i]];
        if (currentTile == '=') {
            trainWaitTicks[i] = 1;
            trainTotalWaitTicks[i]++;
            addTraceEntry(i);
            continue;
        }

        int nextX = getNextX(trainX[i], trainDir[i]);
        int nextY = getNextY(trainY[i], trainDir[i]);

        if (!isValidTile(nextX, nextY)) {
            trainCrashed[i] = true;
            trainActive[i] = false;
            totalCrashed++;
            addTraceEntry(i);
            cout << "Train " << trainId[i] << " crashed (invalid tile)!" << endl;
            continue;
        }

        trainX[i] = nextX;
        trainY[i] = nextY;
        addTraceEntry(i);
    }

    resolveCollisions();

    for (int i = 0; i < totalSwitches; i++) {
        if (switchFlipQueued[i]) {
            switchCurrentState[i] = 1 - switchCurrentState[i];
            switchGlobalCounter[i] = 0;
            for (int d = 0; d < 4; d++) {
                switchCounters[i][d] = 0;
            }
            switchFlipQueued[i] = false;
            cout << "Switch " << switchLetter[i] << " flipped to state "
                << switchCurrentState[i] << endl;
        }
    }
}

void checkArrivals() {
    for (int i = 0; i < totalTrains; i++) {
        if (!trainActive[i] || trainArrived[i] || trainCrashed[i]) continue;

        if (grid[trainY[i]][trainX[i]] == 'D') {
            trainArrived[i] = true;
            trainActive[i] = false;
            totalDelivered++;
            addTraceEntry(i);
            cout << "Train " << trainId[i] << " arrived at destination!" << endl;
        }
    }
}

void printGridToTerminal() {
    cout << "\n========== TICK " << currentTick << " ==========" << endl;

    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            bool trainHere = false;

            for (int t = 0; t < totalTrains; t++) {
                if (trainActive[t] && trainX[t] == c && trainY[t] == r) {
                    cout << (char)('0' + (trainId[t] % 10));
                    trainHere = true;
                    break;
                }
            }

            if (!trainHere) {
                cout << grid[r][c];
            }
        }
        cout << endl;
    }

    cout << "Active: ";
    for (int t = 0; t < totalTrains; t++) {
        if (trainActive[t]) cout << trainId[t] << " ";
    }
    cout << "| Delivered: " << totalDelivered << " | Crashed: " << totalCrashed;
    cout << " | Signal Violations: " << signalViolations << endl;
}

void updateTick() {
    if (isPaused) return;

    spawnTrains();
    determineRoutes();
    updateCounters();
    queueFlips();
    applyWeatherEffects();
    applyEmergencyHalts();
    moveTrains();
    checkArrivals();
    printGridToTerminal();

    currentTick++;
}