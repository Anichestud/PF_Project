#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>

class Grid {
public:
    int rows;
    int cols;
    std::vector<std::string> mapData; // Stores characters like '-', '+', 'S'

    Grid();
    void setDimensions(int r, int c);
    void setRow(int rowIndex, const std::string& rowString);
    char getTile(int r, int c) const;
    bool isWithinBounds(int r, int c) const;
};

#endif