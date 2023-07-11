//
// Created by 23576 on 2023/3/7.
//

#ifndef WORDGAME_GAMEMAP_H
#define WORDGAME_GAMEMAP_H

#include "Meta.h"
#include "Block.h"
#include "math.h"
#include "vector"
#include "stack"
#include <QRandomGenerator>
#include "IdiomList.h"
#include "MetaList.h"

class GameMap {
    std::vector<Block> mapBlock[8];
    std::stack<BlockCapture> historyMap;
public:
    GameMap() {};

    void generate();

    Block readBlock(int x, int y) {
        return mapBlock[x][y];
    }

    int getHeight(int x) {
        return mapBlock[x].size();
    }

    void updateTip();

    void printMap();

    void printID();

    bool removeable(int parent);

    bool removeable(int posx, int posy);

    bool rmMeta(int parent);

    bool rmMeta(int posx, int posy);

    Block getBlock(int x, int y) {
        return mapBlock[x][y];
    }

    std::vector<int> tiplist;

    bool revoke();
};


#endif //WORDGAME_GAMEMAP_H
