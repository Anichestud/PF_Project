#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

extern Texture textures[5];
extern Sprite sprites[5];
extern bool texturesLoaded[5];

extern Texture signalGreen;
extern Texture signalYellow;
extern Texture signalRed;
extern bool signalsLoaded;

extern RenderWindow* window;

extern float cameraX;
extern float cameraY;
extern float zoomLevel;

extern bool isDragging;
extern int lastMouseX;
extern int lastMouseY;

bool initRenderer();
void loadSprites();
void loadSignalSprites();
void renderFrame();
void renderGrid();
void renderTrains();
void renderSignals();
void renderUI();
void handleInput();
void cleanupRenderer();

void drawTile(int x, int y, char tile);
void drawTrain(int x, int y, int dir, int color);
void drawSignal(int x, int y, int signalColor);

#endif