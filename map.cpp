#include "libtcod.hpp"
#include "map.h"

Tile::Tile(){
    canWalk = true;
    Seen = false;
    Infered = false;
}

Map::Map(int width, int height) : width(width),height(height) {
    tiles=new Tile[width*height];
    level = 0;
    moves = 0;
    proceed = false;
    for(int i=0; i < width; i++){	//sets up outer walls on edge of screen
        setWall(i,0);
        setWall(i,height-1);
    }
    for(int j=1; j < height-1; j++){
        setWall(0,j);
        setWall(width-1,j);
    }
    Daedalus();
}

Map::~Map() {
    delete [] tiles;
}

bool Map::isWall(int x, int y) const {	//is there a wall here?
    return !tiles[x+y*width].canWalk;
}
 
void Map::setWall(int x, int y) {	//create wall here
    tiles[x+y*width].canWalk=false;
}

bool Map::isSeen(int x, int y) const {	//tile has been seen
    return tiles[x+y*width].Seen;
}

void Map::wasSeen(int x, int y) {	//tile was just seen
    tiles[x+y*width].Seen=true;
}

bool Map::isInfered(int x, int y) const {	//tile has been infered
    return tiles[x+y*width].Infered;
}

void Map::wasInfered(int x, int y) {	//tile was just infered
    tiles[x+y*width].Infered=true;
}

void Map::newView(int x, int y, int fac) {
    if(fac == 0){	//reveals all that are above
        wasSeen(x-1,y);
        wasSeen(x+1,y);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x,y-1);
            wasInfered(x-1,y-1);
            wasInfered(x+1,y-1);
            y--;
        }
    }
    else if(fac == 1){	//reveals all that are right
        wasSeen(x,y-1);
        wasSeen(x,y+1);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x+1,y);
            wasInfered(x+1,y-1);
            wasInfered(x+1,y+1);
            x++;
        }
    }
    else if(fac == 2){	//reveals all that are below
        wasSeen(x-1,y);
        wasSeen(x+1,y);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x,y+1);
            wasInfered(x-1,y+1);
            wasInfered(x+1,y+1);
            y++;
        }
    }
    else if(fac == 3){	//reveals all that are left
        wasSeen(x,y-1);
        wasSeen(x,y+1);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x-1,y);
            wasInfered(x-1,y-1);
            wasInfered(x-1,y+1);
            x--;
        }
    }
}

void Map::render() const {
    static const TCODColor seenWall(31,24,15);
    static const TCODColor inferedGround(50,50,50);
    static const TCODColor seenGround(100,100,100);
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if (isSeen(x,y))
                TCODConsole::root->setCharBackground(x,y,isWall(x,y) ? seenWall : seenGround );
            else if (isInfered(x,y))
                TCODConsole::root->setCharBackground(x,y,isWall(x,y) ? seenWall : inferedGround );
        }
    }
}

void Map::Daedalus() {	//builds a maze
    level++;
    moves = 0;
    for (int i=0; i <= width-1; i++){	//sets up the nodes/grid
        for (int j=0; j <= height-1; j++){
            if(((i%2)==0)||((j%2)==0))
                tiles[i+j*width].canWalk=false;
            else
                tiles[i+j*width].canWalk=true;

            tiles[i+j*width].Seen=false;	//for resetting the map
            tiles[i+j*width].Infered=false;
        }
    }

    TCODRandom *rnd = new TCODRandom();

    //maze builder algorithim
    //Eller's Algorithm
    // http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm

    int c = 0;
    bool connected = false;
    int count = 0;
    for (int j=1; j<height-3; j = j+2){	//randomly meges within columns
        for(int i=1; i<width-1; i = i+2){
            c = rnd->getInt(1,100);
            if (c < 70)			//column merge probability
                tiles[i+(j+1)*width].canWalk=true;
        }
    }

    for (int i=1; i<width-3; i = i+2){	//meges across rows
        for(int j=1; j<height-1; j = j+2){
            count = count + 1;
            if (tiles[i+(j-1)*width].canWalk == false){
                count = 1;
                connected = false;
            }
            c = rnd->getInt(1,100);
            if (c < 30){		//row merge probability
                tiles[(i+1)+j*width].canWalk=true;
                connected = true;
            }
            if ((tiles[i+(j+1)*width].canWalk==false)&&(connected == false)){
                j = j-(2*count);
                count = 0;
            }
        }
    }

    for (int j = 1; j<height-1; j++)	//ensure all pts are connected
        tiles[(width-2)+(j)*width].canWalk = true;

    delete rnd;
}
