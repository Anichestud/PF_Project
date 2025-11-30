#ifndef GAME_H
#define GAME_H

const int MAX_ROWS = 50;
const int MAX_COLS = 50;
const int MAX_TRAINS = 50;
const int MAX_SWITCHES = 26;
const int MAX_TRACE_ENTRIES = 5000;

const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;

const int WEATHER_NORMAL = 0;
const int WEATHER_RAIN = 1;
const int WEATHER_FOG = 2;

extern int trainId[MAX_TRAINS];
extern int trainX[MAX_TRAINS];
extern int trainY[MAX_TRAINS];
extern int trainDir[MAX_TRAINS];
extern int trainColor[MAX_TRAINS];
extern int trainSpawnTick[MAX_TRAINS];
extern int trainDestX[MAX_TRAINS];
extern int trainDestY[MAX_TRAINS];
extern bool trainActive[MAX_TRAINS];
extern bool trainCrashed[MAX_TRAINS];
extern bool trainArrived[MAX_TRAINS];
extern int trainWaitTicks[MAX_TRAINS];
extern int trainTotalWaitTicks[MAX_TRAINS];
extern int trainMovesSinceRain[MAX_TRAINS];
extern int totalTrains;

extern int trainPrevX[MAX_TRAINS];
extern int trainPrevY[MAX_TRAINS];

extern int traceTick[MAX_TRACE_ENTRIES];
extern int traceTrainID[MAX_TRACE_ENTRIES];
extern int traceX[MAX_TRACE_ENTRIES];
extern int traceY[MAX_TRACE_ENTRIES];
extern int traceDirection[MAX_TRACE_ENTRIES];
extern int traceState[MAX_TRACE_ENTRIES];
extern int traceLogCount;

extern char switchLetter[MAX_SWITCHES];
extern int switchMode[MAX_SWITCHES];
extern int switchInitState[MAX_SWITCHES];
extern int switchKValues[MAX_SWITCHES][4];
extern int switchCurrentState[MAX_SWITCHES];
extern int switchCounters[MAX_SWITCHES][4];
extern int switchGlobalCounter[MAX_SWITCHES];
extern bool switchFlipQueued[MAX_SWITCHES];
extern int switchEmergencyHaltTicks[MAX_SWITCHES];
extern int totalSwitches;

extern char grid[MAX_ROWS][MAX_COLS];
extern int gridRows;
extern int gridCols;

extern int currentTick;
extern int totalDelivered;
extern int totalCrashed;
extern int seed;
extern bool isPaused;
extern int weatherMode;
extern int signalViolations;
extern char levelName[100];

void initGame();
void loadLevel(const char* filename);
void updateTick();
void spawnTrains();
void determineRoutes();
void updateCounters();
void queueFlips();
void moveTrains();
void checkArrivals();
void printGridToTerminal();

void applyWeatherEffects();
void applyEmergencyHalts();
int checkSignalAtSwitch(int switchIdx);

int manhattanDistance(int x1, int y1, int x2, int y2);
int getNextX(int x, int dir);
int getNextY(int y, int dir);
bool isValidTile(int x, int y);
int findSwitchIndex(char letter);
int getOppositeDirection(int dir);
void resolveCollisions();
void addTraceEntry(int trainIdx);

#endif