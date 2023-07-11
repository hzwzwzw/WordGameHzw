//
// Created by 23576 on 2023/3/2.
//

#ifndef WORDGAME_META_H
#define WORDGAME_META_H

#include <string>


class Meta {
private:

public:
    int graph[4][4] = {0};
    int lth[4] = {0};
    int width, height;
    int posX;
    std::string word;

    Meta() {}

    Meta(int width, int height);

    Meta(int width, int height, int graph[][4]);

    void SetWord(std::string w) {
        word = w;
    }

    std::pair<int, int> getPos(int index);

    int getWidth() { return width; }


    int getHeight() {
        return height;
    }

    int getHeight(int i) {
        int ans = 0;
        for (int j = 0; j < 4; j++) {
            if (graph[j][i] != 0)ans++;
        }
        return ans;
    }

    bool operator==(const Meta &b) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (graph[i][j] != b.graph[i][j])return false;
            }
        }
        return true;
    }
};


#endif //WORDGAME_META_H
