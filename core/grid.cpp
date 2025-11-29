#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.

// ---------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x,int y) {
    if(x<0){
    //cout<<"Out of tile track"
    return false;
}
if(x>=grid_row){
  //  cout<<"Out of tile track"
    return false;
}
if(y<0){
  //  cout<<"Out of tile track"
    return false;
}
if(y>=grid_col){
  //  cout<<"Out of tile track"
    return false;
}
return  true;
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.

// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(int x,int y) {
    if (!isInBounds(x,y)){
        return false;
    }
 char tile =grid[y][x];
if (tile =='-'||tile == '|'||tile == '/'||tile == '\\'||tile == '+'){
        return true;
    }

    if (tile =='S'||tile == 'D'||tile =='='){
        return true;
    }
        if (tile >= 'A'&&tile <= 'Z') {
        return true;
    }
    if(tile=='R'||tile=='Y'||tile=='G'||tile=='B'||tile=='F'){
        return true;
    }
    if(tile>='1'&&tile<='9'){
        return true;

    }
   return false;
}

// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(int x,int y){ 
        if (!isInBounds(x,y)){
        return false;
    }
    char tile=grid[y][x];
if(tile>='A'&&tile<='Z'){
    return true;}
    return false;
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char tile) {
    if(tile>='A'&&tile<='Z'){
        return tile -'A';
    }
    return -1;
}

// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint(int x, int y) {
        if (!isInBounds(x, y)) {
        return false;
    }
      char tile=grid[y][x];
    if(tile=='S'){
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x, int y) {
        if (!isInBounds(x, y)) {
        return false;
    }
     char tile=grid[y][x];
    if(tile=='D'){
        grid[y][x]='-';
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile(int x, int y){
            if (!isInBounds(x, y)) {
        return false;
    }
     char tile=grid[y][x];
    if(tile=='='){
        return true;
    }
    if(tile == '-'||tile == '|'){
        grid[y][x]='=';
        return true;
    } 
    return false; 
}

