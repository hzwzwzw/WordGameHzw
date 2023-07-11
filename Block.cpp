//
// Created by 23576 on 2023/3/2.
//

#include "Block.h"

Block::Block(int parent, int index, QString wordS) :
        parentID(parent), wordIndex(index), word(wordS) {}

Block::Block(int parent, int index) :
        parentID(parent), wordIndex(index) {}

int Block::getParent() { return parentID; }

int Block::getIndex() { return wordIndex; }

QString Block::getCharacter() {
    return word[wordIndex - 1];
}
