#include "render.h"
#include <iostream>

using namespace std;
using namespace sf;

extern int trainId[];
extern int trainX[];
extern int trainY[];
extern int trainDir[];
extern int trainColor[];
extern bool trainActive[];
extern bool trainCrashed[];
extern int totalTrains;

extern char switchLetter[];
extern int switchMode[];
extern int switchCurrentState[];
extern int switchEmergencyHaltTicks[];
extern int totalSwitches;

extern char grid[][50];
extern int gridRows;
extern int gridCols;

extern int currentTick;
extern int totalDelivered;
extern int totalCrashed;
extern bool isPaused;
extern int weatherMode;
extern int signalViolations;

int findSwitchIndex(char letter);
int manhattanDistance(int x1, int y1, int x2, int y2);

Texture textures[5];
Sprite sprites[5];
bool texturesLoaded[5];

Texture signalGreen, signalYellow, signalRed;
bool signalsLoaded = false;

RenderWindow* window = nullptr;

float cameraX = 100;
float cameraY = 50;
float zoomLevel = 1.0f;

bool isDragging = false;
int lastMouseX = 0;
int lastMouseY = 0;

const float TILE_SIZE = 64.0f;

Font gameFont;
bool fontLoaded = false;

bool initRenderer()
{
    window = new RenderWindow(VideoMode(1600, 900), "Switchback Rails - Railway Simulation");
    if (!window) {
        return false;
    }

    window->setFramerateLimit(60);

    fontLoaded = gameFont.loadFromFile("arial.ttf");
    if (!fontLoaded) {
        cout << "Font not loaded, using shapes for text" << endl;
    }

    loadSprites();
    loadSignalSprites();

    return true;
}

void loadSprites() {
    for (int i = 0; i < 5; i++) {
        texturesLoaded[i] = false;
    }

    for (int i = 0; i < 5; i++) {
        char path[50];
        path[0] = 'S'; path[1] = 'p'; path[2] = 'r';
        path[3] = 'i'; path[4] = 't'; path[5] = 'e'; path[6] = 's';
        path[7] = '/'; path[8] = '0'+(i+1); path[9] = '.'; path[10] = 'p';
        path[11] = 'n'; path[12] = 'g'; path[13] = '\0';

        if (textures[i].loadFromFile(path)) {
            sprites[i].setTexture(textures[i]);
            texturesLoaded[i] = true;
            cout << "Loaded sprite: " << path << endl;
        }
        else {
            cout << "Warning: Could not load " << path << endl;
        }
    }
}

void loadSignalSprites() {
    if (signalGreen.loadFromFile("Sprites/Green.png")) {
        signalGreen.setSmooth(true);
        cout << "Loaded: Green.png" << endl;
    }
    else {
        cout << "Warning: Could not load Green.png" << endl;
    }

    if (signalYellow.loadFromFile("Sprites/Yellow.png")) {
        signalYellow.setSmooth(true);
        cout << "Loaded: Yellow.png" << endl;
    }
    else {
        cout << "Warning: Could not load Yellow.png" << endl;
    }

    if (signalRed.loadFromFile("Sprites/Red.png")) {
        signalRed.setSmooth(true);
        cout << "Loaded: Red.png" << endl;
    }
    else {
        cout << "Warning: Could not load Red.png" << endl;
    }

    signalsLoaded = (signalGreen.getSize().x > 0 &&
        signalYellow.getSize().x > 0 &&
        signalRed.getSize().x > 0);
}

void drawTile(int x, int y, char tile) {
    float screenX = x * TILE_SIZE + cameraX;
    float screenY = y * TILE_SIZE + cameraY;

    RectangleShape tileRect(Vector2f(TILE_SIZE, TILE_SIZE));
    tileRect.setPosition(screenX, screenY);

    if (tile == '-' || tile == '|') {
        tileRect.setFillColor(Color(101, 67, 33));
        window->draw(tileRect);

        RectangleShape rail1(Vector2f(tile == '-' ? TILE_SIZE : 12, tile == '|' ? TILE_SIZE : 12));
        rail1.setFillColor(Color(200, 200, 200));
        rail1.setPosition(screenX + (tile == '-' ? 0 : TILE_SIZE * 0.25f),
            screenY + (tile == '|' ? 0 : TILE_SIZE * 0.25f));
        window->draw(rail1);

        RectangleShape rail2(Vector2f(tile == '-' ? TILE_SIZE : 12, tile == '|' ? TILE_SIZE : 12));
        rail2.setFillColor(Color(200, 200, 200));
        rail2.setPosition(screenX + (tile == '-' ? 0 : TILE_SIZE * 0.63f),
            screenY + (tile == '|' ? 0 : TILE_SIZE * 0.63f));
        window->draw(rail2);

        if (tile == '-') {
            for (int i = 0; i < 5; i++) {
                RectangleShape sleeper(Vector2f(10, TILE_SIZE * 0.7f));
                sleeper.setFillColor(Color(139, 90, 43));
                sleeper.setPosition(screenX + i * TILE_SIZE / 5.0f, screenY + TILE_SIZE * 0.15f);
                window->draw(sleeper);
            }
        }
        else {
            for (int i = 0; i < 5; i++) {
                RectangleShape sleeper(Vector2f(TILE_SIZE * 0.7f, 10));
                sleeper.setFillColor(Color(139, 90, 43));
                sleeper.setPosition(screenX + TILE_SIZE * 0.15f, screenY + i * TILE_SIZE / 5.0f);
                window->draw(sleeper);
            }
        }
    }
    else if (tile == '/' || tile == '\\') {
        tileRect.setFillColor(Color(101, 67, 33));
        window->draw(tileRect);

        RectangleShape rail(Vector2f(TILE_SIZE * 1.2f, 12));
        rail.setFillColor(Color(200, 200, 200));
        rail.setPosition(screenX + TILE_SIZE * 0.5f, screenY + TILE_SIZE * 0.5f);
        rail.setOrigin(TILE_SIZE * 0.6f, 6);
        rail.setRotation(tile == '/' ? -45 : 45);
        window->draw(rail);
    }
    else if (tile == '+') {
        tileRect.setFillColor(Color(101, 67, 33));
        window->draw(tileRect);

        RectangleShape rail1(Vector2f(TILE_SIZE, 12));
        rail1.setFillColor(Color(200, 200, 200));
        rail1.setPosition(screenX, screenY + TILE_SIZE * 0.25f);
        window->draw(rail1);

        RectangleShape rail2(Vector2f(TILE_SIZE, 12));
        rail2.setFillColor(Color(200, 200, 200));
        rail2.setPosition(screenX, screenY + TILE_SIZE * 0.63f);
        window->draw(rail2);

        RectangleShape rail3(Vector2f(12, TILE_SIZE));
        rail3.setFillColor(Color(200, 200, 200));
        rail3.setPosition(screenX + TILE_SIZE * 0.25f, screenY);
        window->draw(rail3);

        RectangleShape rail4(Vector2f(12, TILE_SIZE));
        rail4.setFillColor(Color(200, 200, 200));
        rail4.setPosition(screenX + TILE_SIZE * 0.63f, screenY);
        window->draw(rail4);
    }
    else if (tile == 'S') {
        tileRect.setFillColor(Color(0, 120, 255));
        window->draw(tileRect);

        RectangleShape border(Vector2f(TILE_SIZE, TILE_SIZE));
        border.setPosition(screenX, screenY);
        border.setFillColor(Color::Transparent);
        border.setOutlineColor(Color::Yellow);
        border.setOutlineThickness(5);
        window->draw(border);

        if (fontLoaded) {
            Text text("S", gameFont, (unsigned int)(TILE_SIZE * 0.7f));
            text.setFillColor(Color::Yellow);
            text.setStyle(Text::Bold);
            text.setPosition(screenX + TILE_SIZE * 0.2f, screenY + TILE_SIZE * 0.1f);
            window->draw(text);
        }
    }
    else if (tile == 'D') {
        tileRect.setFillColor(Color(255, 50, 50));
        window->draw(tileRect);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if ((i + j) % 2 == 0) {
                    RectangleShape checker(Vector2f(TILE_SIZE / 3, TILE_SIZE / 3));
                    checker.setFillColor(Color::White);
                    checker.setPosition(screenX + i * TILE_SIZE / 3, screenY + j * TILE_SIZE / 3);
                    window->draw(checker);
                }
            }
        }

        if (fontLoaded) {
            Text text("D", gameFont, (unsigned int)(TILE_SIZE * 0.7f));
            text.setFillColor(Color::Black);
            text.setStyle(Text::Bold);
            text.setPosition(screenX + TILE_SIZE * 0.2f, screenY + TILE_SIZE * 0.1f);
            window->draw(text);
        }
    }
    else if (tile >= 'A' && tile <= 'Z') {
        tileRect.setFillColor(Color(128, 128, 180));
        window->draw(tileRect);

        RectangleShape switchBase(Vector2f(TILE_SIZE * 0.7f, TILE_SIZE * 0.7f));
        switchBase.setFillColor(Color(80, 80, 120));
        switchBase.setPosition(screenX + TILE_SIZE * 0.15f, screenY + TILE_SIZE * 0.15f);
        window->draw(switchBase);

        int swIdx = findSwitchIndex(tile);
        if (swIdx != -1) {
            CircleShape stateIndicator(TILE_SIZE * 0.15f);
            stateIndicator.setFillColor(switchCurrentState[swIdx] == 0 ? Color::Green : Color::Yellow);
            stateIndicator.setPosition(screenX + TILE_SIZE * 0.7f, screenY + TILE_SIZE * 0.05f);
            window->draw(stateIndicator);

            if (switchEmergencyHaltTicks[swIdx] > 0) {
                CircleShape haltIndicator(TILE_SIZE * 0.2f);
                haltIndicator.setFillColor(Color(255, 0, 0, 150));
                haltIndicator.setPosition(screenX + TILE_SIZE * 0.3f, screenY + TILE_SIZE * 0.3f);
                window->draw(haltIndicator);
            }
        }

        if (fontLoaded) {
            Text text;
            text.setFont(gameFont);
            text.setCharacterSize((unsigned int)(TILE_SIZE * 0.5f));
            text.setFillColor(Color::White);
            text.setStyle(Text::Bold);
            char str[2];
            str[0] = tile;
            str[1] = '\0';
            text.setString(str);
            text.setPosition(screenX + TILE_SIZE * 0.3f, screenY + TILE_SIZE * 0.25f);
            window->draw(text);
        }
    }
    else if (tile == '=') {
        tileRect.setFillColor(Color(255, 220, 0));
        window->draw(tileRect);

        for (int i = 0; i < 5; i++) {
            RectangleShape stripe(Vector2f(TILE_SIZE, TILE_SIZE * 0.15f));
            stripe.setFillColor(Color::Black);
            stripe.setPosition(screenX, screenY + i * TILE_SIZE / 5.0f);
            if (i % 2 == 0) window->draw(stripe);
        }
    }
    else {
        tileRect.setFillColor(Color(40, 40, 45));
        window->draw(tileRect);
    }

    RectangleShape gridLine(Vector2f(TILE_SIZE, TILE_SIZE));
    gridLine.setPosition(screenX, screenY);
    gridLine.setFillColor(Color::Transparent);
    gridLine.setOutlineColor(Color(60, 60, 65));
    gridLine.setOutlineThickness(1);
    window->draw(gridLine);
}

void drawTrain(int x, int y, int dir, int color)
{
    static bool loaded = false;
    static Texture texUp, texDown, texLeft, texRight;

    if (!loaded)
    {
        texUp.loadFromFile("Sprites/Up.png");
        texDown.loadFromFile("Sprites/Down.png");
        texLeft.loadFromFile("Sprites/Left.png");
        texRight.loadFromFile("Sprites/Right.png");

        texUp.setSmooth(false);
        texDown.setSmooth(false);
        texLeft.setSmooth(false);
        texRight.setSmooth(false);

        loaded = true;
    }

    Sprite train;

    switch (dir)
    {
    case 0: train.setTexture(texUp);    break;
    case 1: train.setTexture(texRight); break;
    case 2: train.setTexture(texDown);  break;
    case 3: train.setTexture(texLeft);  break;
    default: train.setTexture(texUp);   break;
    }

    const int SPRITE_SIZE = 48;
    const float TRAIN_SCALE = 1.5f;

    float screenX = x * TILE_SIZE + cameraX;
    float screenY = y * TILE_SIZE + cameraY;

    train.setOrigin(SPRITE_SIZE / 2 + 1, SPRITE_SIZE / 2 + 1);
    float scale = (TILE_SIZE / (float)SPRITE_SIZE) * TRAIN_SCALE;
    train.setScale(scale, scale);
    train.setPosition(screenX + TILE_SIZE / 2.0f, screenY + TILE_SIZE / 2.0f);

    switch (color)
    {
    case 1: train.setColor(Color(255, 120, 120)); break;
    case 2: train.setColor(Color(120, 255, 120)); break;
    case 3: train.setColor(Color(255, 255, 120)); break;
    case 4: train.setColor(Color(255, 150, 255)); break;
    default: train.setColor(Color::White); break;
    }

    window->draw(train);
}

void drawSignal(int x, int y, int signalColor) {
    float screenX = x * TILE_SIZE + cameraX;
    float screenY = y * TILE_SIZE + cameraY;

    if (weatherMode == 2) {
        signalColor = (signalColor + 1) % 3;
    }

    Sprite signalSprite;

    if (signalColor == 0) signalSprite.setTexture(signalGreen);
    else if (signalColor == 1) signalSprite.setTexture(signalYellow);
    else signalSprite.setTexture(signalRed);

    float scale = (TILE_SIZE * 1.5f) / 48.0f;
    signalSprite.setScale(scale, scale);
    signalSprite.setPosition(screenX + TILE_SIZE * 0.5f, screenY - 80);

    window->draw(signalSprite);
}

void renderGrid() {
    for (int r = 0; r < gridRows; r++) {
        for (int c = 0; c < gridCols; c++) {
            drawTile(c, r, grid[r][c]);
        }
    }
}

void renderTrains() {
    for (int i = 0; i < totalTrains; i++) {
        if (trainActive[i] && !trainCrashed[i]) {
            drawTrain(trainX[i], trainY[i], trainDir[i], trainColor[i]);
        }
    }
}

void renderSignals() {
    for (int i = 0; i < totalSwitches; i++) {
        char letter = switchLetter[i];

        for (int r = 0; r < gridRows; r++) {
            for (int c = 0; c < gridCols; c++) {
                if (grid[r][c] == letter) {
                    int signalColor = 0;

                    for (int t = 0; t < totalTrains; t++) {
                        if (trainActive[t]) {
                            int dist = manhattanDistance(c, r, trainX[t], trainY[t]);
                            if (dist == 1) {
                                signalColor = 2;
                                break;
                            }
                            else if (dist <= 2) {
                                signalColor = 1;
                            }
                        }
                    }

                    drawSignal(c, r, signalColor);
                }
            }
        }
    }
}

void renderUI() {
    RectangleShape uiPanel(Vector2f(450, 280));
    uiPanel.setPosition(10, 10);
    uiPanel.setFillColor(Color(0, 0, 0, 220));
    uiPanel.setOutlineColor(Color(100, 150, 255));
    uiPanel.setOutlineThickness(4);
    window->draw(uiPanel);

    if (fontLoaded) {
        Text title("SWITCHBACK RAILS", gameFont, 32);
        title.setFillColor(Color(100, 200, 255));
        title.setStyle(Text::Bold);
        title.setPosition(20, 20);
        window->draw(title);

        char buffer[100];
        Text tickText;
        tickText.setFont(gameFont);
        tickText.setCharacterSize(22);
        tickText.setFillColor(Color::White);

        buffer[0] = 'T'; buffer[1] = 'i'; buffer[2] = 'c'; buffer[3] = 'k';
        buffer[4] = ':'; buffer[5] = ' ';
        int temp = currentTick;
        int idx = 6;
        if (temp == 0) { buffer[idx++] = '0'; }
        else {
            char digits[20];
            int dc = 0;
            while (temp > 0) { digits[dc++] = '0' + (temp % 10); temp /= 10; }
            for (int i = dc - 1; i >= 0; i--) buffer[idx++] = digits[i];
        }
        buffer[idx] = '\0';
        tickText.setString(buffer);
        tickText.setPosition(20, 70);
        window->draw(tickText);

        Text deliveredText;
        deliveredText.setFont(gameFont);
        deliveredText.setCharacterSize(22);
        deliveredText.setFillColor(Color(50, 255, 50));
        buffer[0] = 'D'; buffer[1] = 'e'; buffer[2] = 'l'; buffer[3] = 'i';
        buffer[4] = 'v'; buffer[5] = 'e'; buffer[6] = 'r'; buffer[7] = 'e';
        buffer[8] = 'd'; buffer[9] = ':'; buffer[10] = ' ';
        temp = totalDelivered;
        idx = 11;
        if (temp == 0) { buffer[idx++] = '0'; }
        else {
            char digits[20];
            int dc = 0;
            while (temp > 0) { digits[dc++] = '0' + (temp % 10); temp /= 10; }
            for (int i = dc - 1; i >= 0; i--) buffer[idx++] = digits[i];
        }
        buffer[idx] = '\0';
        deliveredText.setString(buffer);
        deliveredText.setPosition(20, 105);
        window->draw(deliveredText);

        Text crashedText;
        crashedText.setFont(gameFont);
        crashedText.setCharacterSize(22);
        crashedText.setFillColor(Color(255, 50, 50));
        buffer[0] = 'C'; buffer[1] = 'r'; buffer[2] = 'a'; buffer[3] = 's';
        buffer[4] = 'h'; buffer[5] = 'e'; buffer[6] = 'd'; buffer[7] = ':';
        buffer[8] = ' ';
        temp = totalCrashed;
        idx = 9;
        if (temp == 0) { buffer[idx++] = '0'; }
        else {
            char digits[20];
            int dc = 0;
            while (temp > 0) { digits[dc++] = '0' + (temp % 10); temp /= 10; }
            for (int i = dc - 1; i >= 0; i--) buffer[idx++] = digits[i];
        }
        buffer[idx] = '\0';
        crashedText.setString(buffer);
        crashedText.setPosition(20, 140);
        window->draw(crashedText);

        int activeCount = 0;
        for (int i = 0; i < totalTrains; i++) {
            if (trainActive[i]) activeCount++;
        }
        Text activeText;
        activeText.setFont(gameFont);
        activeText.setCharacterSize(22);
        activeText.setFillColor(Color(255, 255, 50));
        buffer[0] = 'A'; buffer[1] = 'c'; buffer[2] = 't'; buffer[3] = 'i';
        buffer[4] = 'v'; buffer[5] = 'e'; buffer[6] = ':'; buffer[7] = ' ';
        temp = activeCount;
        idx = 8;
        if (temp == 0) { buffer[idx++] = '0'; }
        else {
            char digits[20];
            int dc = 0;
            while (temp > 0) { digits[dc++] = '0' + (temp % 10); temp /= 10; }
            for (int i = dc - 1; i >= 0; i--) buffer[idx++] = digits[i];
        }
        buffer[idx] = '\0';
        activeText.setString(buffer);
        activeText.setPosition(20, 175);
        window->draw(activeText);

        Text violationsText;
        violationsText.setFont(gameFont);
        violationsText.setCharacterSize(20);
        violationsText.setFillColor(Color(255, 100, 100));
        buffer[0] = 'S'; buffer[1] = 'i'; buffer[2] = 'g'; buffer[3] = 'n';
        buffer[4] = 'a'; buffer[5] = 'l'; buffer[6] = ' '; buffer[7] = 'V';
        buffer[8] = 'i'; buffer[9] = 'o'; buffer[10] = 'l'; buffer[11] = 'a';
        buffer[12] = 't'; buffer[13] = 'i'; buffer[14] = 'o'; buffer[15] = 'n';
        buffer[16] = 's'; buffer[17] = ':'; buffer[18] = ' ';
        temp = signalViolations;
        idx = 19;
        if (temp == 0) { buffer[idx++] = '0'; }
        else {
            char digits[20];
            int dc = 0;
            while (temp > 0) { digits[dc++] = '0' + (temp % 10); temp /= 10; }
            for (int i = dc - 1; i >= 0; i--) buffer[idx++] = digits[i];
        }
        buffer[idx] = '\0';
        violationsText.setString(buffer);
        violationsText.setPosition(20, 210);
        window->draw(violationsText);

        const char* weatherStr = "Normal";
        if (weatherMode == 1) weatherStr = "RAIN";
        else if (weatherMode == 2) weatherStr = "FOG";

        Text weatherText("Weather: ", gameFont, 18);
        weatherText.setFillColor(Color(150, 200, 255));
        weatherText.setPosition(20, 240);
        window->draw(weatherText);

        Text weatherType(weatherStr, gameFont, 18);
        weatherType.setFillColor(Color(255, 255, 100));
        weatherType.setStyle(Text::Bold);
        weatherType.setPosition(120, 240);
        window->draw(weatherType);

        if (isPaused) {
            Text pauseText("|| PAUSED", gameFont, 24);
            pauseText.setFillColor(Color(255, 255, 0));
            pauseText.setStyle(Text::Bold);
            pauseText.setPosition(280, 245);
            window->draw(pauseText);
        }
    }

    RectangleShape legendPanel(Vector2f(380, 360));
    legendPanel.setPosition(window->getSize().x - 390, 10);
    legendPanel.setFillColor(Color(0, 0, 0, 220));
    legendPanel.setOutlineColor(Color(100, 150, 255));
    legendPanel.setOutlineThickness(4);
    window->draw(legendPanel);

    if (fontLoaded) {
        Text legendTitle("CONTROLS", gameFont, 24);
        legendTitle.setFillColor(Color(100, 200, 255));
        legendTitle.setStyle(Text::Bold);
        legendTitle.setPosition(window->getSize().x - 380, 20);
        window->draw(legendTitle);

        Text controls;
        controls.setFont(gameFont);
        controls.setCharacterSize(16);
        controls.setFillColor(Color::White);
        controls.setString(
            "SPACE - Pause/Resume\n"
            ". - Step one tick\n"
            "Arrow Keys - Pan camera\n"
            "Mouse Wheel - Zoom\n"
            "Middle Click - Pan (drag)\n"
            "R - Reset camera\n"
            "ESC - Exit & Save\n\n"
            "LEFT CLICK - Place/Remove =\n"
            "RIGHT CLICK - Toggle Switch\n"
            "E (on switch) - Emergency Halt\n\n"
            "FEATURES:\n"
            "- Distance-based priority\n"
            "- Weather effects active\n"
            "- Signal violation tracking\n"
            "- Emergency halt system"
        );
        controls.setPosition(window->getSize().x - 375, 60);
        window->draw(controls);
    }
}

void handleInput() {
    Event event;
    while (window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            window->close();
        }

        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Escape) {
                window->close();
            }
            if (event.key.code == Keyboard::Space) {
                isPaused = !isPaused;
                cout << (isPaused ? "PAUSED" : "RESUMED") << endl;
            }
            if (event.key.code == Keyboard::Period) {
                extern void updateTick();
                updateTick();
            }
            if (event.key.code == Keyboard::Up) {
                cameraY += 30;
            }
            if (event.key.code == Keyboard::Down) {
                cameraY -= 30;
            }
            if (event.key.code == Keyboard::Left) {
                cameraX += 30;
            }
            if (event.key.code == Keyboard::Right) {
                cameraX -= 30;
            }
            if (event.key.code == Keyboard::R) {
                cameraX = 100;
                cameraY = 50;
                zoomLevel = 1.0f;
            }
        }

        if (event.type == Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                zoomLevel *= 1.1f;
            }
            else {
                zoomLevel *= 0.9f;
            }
        }

        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int gridX = (mouseX - cameraX) / TILE_SIZE;
                int gridY = (mouseY - cameraY) / TILE_SIZE;

                if (gridX >= 0 && gridX < gridCols && gridY >= 0 && gridY < gridRows) {
                    char current = grid[gridY][gridX];
                    if (current == '-' || current == '|') {
                        grid[gridY][gridX] = '=';
                        cout << "Safety buffer placed at (" << gridX << "," << gridY << ")" << endl;
                    }
                    else if (current == '=') {
                        grid[gridY][gridX] = '-';
                        cout << "Safety buffer removed from (" << gridX << "," << gridY << ")" << endl;
                    }
                }
            }
            else if (event.mouseButton.button == Mouse::Right) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int gridX = (mouseX - cameraX) / TILE_SIZE;
                int gridY = (mouseY - cameraY) / TILE_SIZE;

                if (gridX >= 0 && gridX < gridCols && gridY >= 0 && gridY < gridRows) {
                    char tile = grid[gridY][gridX];
                    if (tile >= 'A' && tile <= 'Z') {
                        int swIdx = findSwitchIndex(tile);
                        if (swIdx != -1) {
                            switchCurrentState[swIdx] = 1 - switchCurrentState[swIdx];
                            cout << "Switch " << tile << " toggled to state "
                                << switchCurrentState[swIdx] << endl;
                        }
                    }
                }
            }
            else if (event.mouseButton.button == Mouse::Middle) {
                isDragging = true;
                lastMouseX = event.mouseButton.x;
                lastMouseY = event.mouseButton.y;
            }
        }

        if (event.type == Event::MouseButtonReleased) {
            if (event.mouseButton.button == Mouse::Middle) {
                isDragging = false;
            }
        }

        if (event.type == Event::MouseMoved) {
            if (isDragging) {
                int currentX = event.mouseMove.x;
                int currentY = event.mouseMove.y;

                cameraX += (currentX - lastMouseX);
                cameraY += (currentY - lastMouseY);

                lastMouseX = currentX;
                lastMouseY = currentY;
            }
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::E) {
            int mousePos = Mouse::getPosition(*window).x;
            int mousePosY = Mouse::getPosition(*window).y;

            int gridX = (mousePos - cameraX) / TILE_SIZE;
            int gridY = (mousePosY - cameraY) / TILE_SIZE;

            if (gridX >= 0 && gridX < gridCols && gridY >= 0 && gridY < gridRows) {
                char tile = grid[gridY][gridX];
                if (tile >= 'A' && tile <= 'Z') {
                    int swIdx = findSwitchIndex(tile);
                    if (swIdx != -1) {
                        switchEmergencyHaltTicks[swIdx] = 3;
                        cout << "Emergency halt activated on switch " << tile
                            << " for 3 ticks!" << endl;
                    }
                }
            }
        }
    }
}

void renderFrame() {
    window->clear(Color(25, 25, 30));

    renderGrid();
    renderSignals();
    renderTrains();
    renderUI();

    window->display();
}

void cleanupRenderer() {
    if (window) {
        delete window;
        window = nullptr;
    }
}