//
// Created by 23576 on 2023/3/2.
//

#include "Meta.h"

Meta::Meta(int width, int height) {
    this->width = width;
    this->height = height;
}

Meta::Meta(int width, int height, int graph[][4]) {
    this->width = width;
    this->height = height;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->graph[i][j] = graph[i][j];
            if (graph[i][j])this->lth[j]++;
        }
    }
}

std::pair<int, int> Meta::getPos(int index) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->graph[i][j] == index)return std::make_pair(i, j);
        }
    }
    return std::make_pair(-1, -1);
}
