//
// Created by 23576 on 2023/3/2.
//

#ifndef WORDGAME_BLOCK_H
#define WORDGAME_BLOCK_H

#include "QString"

class Block {
protected:
    int wordIndex;
    int parentID;
    QString character, word;
public:
    Block(int parent, int index, QString wordS);;

    Block(int parent, int index);;

    Block() {}

    QString getCharacter();

    int getParent();

    int getIndex();
};

class BlockCapture {
public:
    Block nullBlock = Block(-2, 0);
    Block map[8][8];
};

#endif //WORDGAME_BLOCK_H
