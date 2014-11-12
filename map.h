struct Tile {
    bool canWalk; 	// can we walk through this tile?
    bool Seen;		// has been seen by player
    bool Infered;	// has been on edge of vision
    Tile();
};
 
class Map {
public:
    int width,height;
    int level;
    int moves;
    bool proceed;
 
    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    bool isSeen(int x, int y) const;
    void wasSeen(int x,int y);
    bool isInfered(int x, int y) const;
    void wasInfered(int x, int y);
    void render() const;
    void newView(int x, int y, int fac);    //updates view
    void Daedalus();	//builds a maze
private:
    Tile *tiles;
 
    void setWall(int x, int y);
};
