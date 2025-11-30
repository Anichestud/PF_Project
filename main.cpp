#include "core/game.h"
#include "sfml/render.h"
#include "core/fileio.h"
#include <iostream>

using namespace std;
using namespace sf;

int main(int argc, char* argv[]) {
    cout << "\n";
    cout << "============================================\n";
    cout << "     SWITCHBACK RAILS - Railway Simulation\n";
    cout << "         GUI Game with SFML Graphics\n";
    cout << "============================================\n";
    cout << "\n";

    cout << "Initializing game engine..." << endl;
    initGame();

    const char* levelFile = "levels/hard_level.lvl";
    if (argc > 1) {
        levelFile = argv[1];
    }

    cout << "Loading level: " << levelFile << endl;
    loadLevel(levelFile);

    cout << "Initializing SFML graphics..." << endl;
    if (!initRenderer()) {
        cout << "ERROR: Failed to initialize SFML renderer!" << endl;
        cout << "Make sure SFML is installed properly." << endl;
        return 1;
    }

    cout << "\n";
    cout << "============================================\n";
    cout << "              GAME STARTED\n";
    cout << "============================================\n";
    cout << "\n";
    cout << "SIMULATION SPEED: 2 ticks per second\n";
    cout << "(Much slower and easier to watch!)\n";
    cout << "\n";
    cout << "CONTROLS (shown in game window):\n";
    cout << "  SPACE       - Pause/Resume simulation\n";
    cout << "  . (period)  - Step one tick forward\n";
    cout << "  Arrow Keys  - Pan camera around\n";
    cout << "  Mouse Wheel - Zoom in/out\n";
    cout << "  R           - Reset camera position\n";
    cout << "  ESC         - Exit and save logs\n";
    cout << "  LEFT CLICK  - Place/Remove safety buffer\n";
    cout << "  RIGHT CLICK - Toggle switch state\n";
    cout << "  MIDDLE DRAG - Pan camera\n";
    cout << "  E (on switch) - Emergency halt\n";
    cout << "\n";
    cout << "VISUAL IMPROVEMENTS:\n";
    cout << "  - Trains are now MUCH LARGER (80% of tile)\n";
    cout << "  - Brighter colors for better visibility\n";
    cout << "  - Thicker borders and outlines\n";
    cout << "  - Larger direction arrows\n";
    cout << "  - Bigger signal lights\n";
    cout << "  - Slower speed (2 ticks/sec vs 10)\n";
    cout << "  - Mouse controls for interaction\n";
    cout << "\n";
    cout << "LEGEND:\n";
    cout << "  Blue tiles with 'S'  - Train spawn points\n";
    cout << "  Red checkered 'D'    - Destination stations\n";
    cout << "  Letters (A-Z)        - Railway switches\n";
    cout << "  Yellow stripes (=)   - Safety buffers\n";
    cout << "  Brown with rails     - Track tiles\n";
    cout << "  Colored rectangles   - Trains (with arrows)\n";
    cout << "  Circles on switches  - Signal lights\n";
    cout << "\n";
    cout << "Game is running in GUI window...\n";
    cout << "Terminal will show tick updates below:\n";
    cout << "--------------------------------------------\n";

    printGridToTerminal();

    Clock clock;
    float tickInterval = 0.5f;
    float accumulator = 0.0f;

    bool gameComplete = false;

    cout << "\n GAME IS NOW RUNNING IN THE GUI WINDOW!\n";
    cout << "   Watch the trains move slowly and clearly.\n";
    cout << "   Press SPACE to pause anytime.\n\n";

    while (window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;

        handleInput();

        while (accumulator >= tickInterval) {
            if (!isPaused && !gameComplete) {
                updateTick();
            }
            accumulator -= tickInterval;
        }

        renderFrame();

        if (!gameComplete) {
            bool allDone = true;
            for (int i = 0; i < totalTrains; i++) {
                if (!trainArrived[i] && !trainCrashed[i]) {
                    allDone = false;
                    break;
                }
            }

            if (allDone && totalTrains > 0 && currentTick > 10) {
                gameComplete = true;
                cout << "\n";
                cout << "============================================\n";
                cout << "         SIMULATION COMPLETE! \n";
                cout << "============================================\n";
                cout << "All trains have reached their final state.\n";
                cout << "\n";
                cout << " FINAL RESULTS:\n";
                cout << "   Total Ticks: " << currentTick << "\n";
                cout << "   Trains Delivered: " << totalDelivered << " \n";
                cout << "   Trains Crashed: " << totalCrashed << " \n";
                if (totalTrains > 0) {
                    int rate = (totalDelivered * 100) / totalTrains;
                    cout << "   Success Rate: " << rate << "%\n";
                }
                cout << "\n";
                cout << "Press ESC to exit and save logs...\n";
                cout << "\n";
                isPaused = true;
            }
        }
    }

    cout << "\n";
    cout << "============================================\n";
    cout << "         SAVING OUTPUT FILES\n";
    cout << "============================================\n";
    saveTraceLog();
    saveSwitchLog();
    saveSignalLog();
    saveMetrics();

    cout << "\n";
    cout << "============================================\n";
    cout << "          FINAL STATISTICS\n";
    cout << "============================================\n";
    cout << "Total Simulation Ticks: " << currentTick << endl;
    cout << "Total Trains: " << totalTrains << endl;
    cout << "Trains Delivered: " << totalDelivered << " " << endl;
    cout << "Trains Crashed: " << totalCrashed << " " << endl;

    if (totalTrains > 0) {
        int successRate = (totalDelivered * 100) / totalTrains;
        cout << "Success Rate: " << successRate << "%" << endl;
    }

    cout << "\n";
    cout << " Output files saved to out/ directory:\n";
    cout << "   trace.csv (train movement history)\n";
    cout << "   switches.csv (switch state changes)\n";
    cout << "   signals.csv (signal light states)\n";
    cout << "   metrics.txt (performance statistics)\n";

    cleanupRenderer();

    cout << "\n";
    cout << "============================================\n";
    cout << "    Thank you for playing Switchback Rails!\n";
    cout << "============================================\n";
    cout << "\n";

    return 0;
}