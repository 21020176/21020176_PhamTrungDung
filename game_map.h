#ifndef GAME_MAP_H_INCLUDED
#define GAME_MAP_H_INCLUDED

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAP_TILES 20


class TileMat : public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}
} ;

class GameMap
{
public:
    GameMap(){;}
    ~GameMap(){;}

    void LoadMap(char* nam);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map getMap() const {return game_map_;};
    void SetMap(Map& map_data){game_map_ = map_data;};
private:
    Map game_map_;
    TileMat tile_mat[MAP_TILES];
};

#endif // GAME_MAP_H_INCLUDED
