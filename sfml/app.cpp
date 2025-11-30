#include "app.h"
#include "core/simulation_state.h"
#include "core/simulation.h"
#include "core/grid.h"
#include "core/switches.h"
#include "core/io.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>


// ============================================================================
// GLOBAL VARIABLES FOR APP STATE
// ============================================================================
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;
static sf::View g_camera;
static bool g_isPaused = false;
static bool g_isStepMode = false;
static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;
static float g_cellSize = 40.0f;
static float g_gridOffsetX = 50.0f;
static float g_gridOffsetY = 50.0f;

// ============================================================================
// FUNCTIONS
// ============================================================================

bool initializeApp() {
    g_window = new sf::RenderWindow(sf::VideoMode(1200, 800), "Switchback Rails");
    g_window->setFramerateLimit(60);
    
    if (!g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return false;
    }
    
    g_camera = g_window->getDefaultView();
    return true;
}

void runApp() {
    while (g_window->isOpen()) {
        sf::Event event;
        while (g_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                g_window->close();
            }
            
           if (event.type == sf::Event::KeyPressed) {  
                if (event.key.code == sf::Keyboard::Escape) {
                    g_window->close();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    g_isPaused = !g_isPaused;
                }
            }
        }
        
        if (!g_isPaused) {
            simulateOneTick();
        }
        
        g_window->clear(sf::Color::Black);
        g_window->display();
    }
}

void cleanupApp() {
    delete g_window;
    g_window = nullptr;
}