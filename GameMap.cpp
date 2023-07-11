//
// Created by 23576 on 2023/3/7.
//


#include "GameMap.h"
#include "iostream"


std::vector<int> gene_arrangement(int n) {
    //srand(time(NULL));
    std::vector<bool> vis;
    std::vector<int> ans;
    for (int i = 0; i < n; i++) {
        vis.push_back((bool) false);
    }

    for (int i = 0; i < n; i++) {
        int t = QRandomGenerator::global()->bounded(100) % n;
        while (vis[t])t = QRandomGenerator::global()->bounded(100) % n;
        vis[t] = true;
        ans.push_back(t);
    }
    return ans;
}

MetaList ml;
IdiomList il;
std::vector<Meta> metaQueue[8];
std::vector<Meta> totQueue;
int heightT[8];
int cnt = 0, cntIni;

void pushUpdate(int x, Meta m) {
    for (int i = 0; i < m.getWidth(); i++) {
        heightT[x + i] += m.getHeight(i);
    }
    m.posX = x;
    metaQueue[x].push_back(m);
    totQueue.push_back(m);
    cnt++;
}

void popUpdate(int x) {
    Meta m = metaQueue[x].back();
    metaQueue[x].pop_back();
    totQueue.pop_back();
    for (int i = 0; i < m.getWidth(); i++) {
        heightT[x + i] -= m.getHeight(i);
    }
    cnt--;
}

int cnttmp = 0;

bool dfs(int depth) {

    cnttmp++;
    //set an arrangement
    std::vector<int> arrMeta = gene_arrangement(15);
    std::vector<int> arrPos = gene_arrangement(8);
    for (int ti = 0; ti < 8; ti++) {
        for (int tj = 0; tj < 15; tj++) {
            int i = arrPos[ti];
            int j = arrMeta[tj];
            Meta m = *ml.MetaList_All[j];
            //JUDGE: can it be pushed?
            if (m.getWidth() + i > 8)continue;

            bool as = true;
            for (int k = 0; k < m.getWidth(); k++) {
                if (heightT[i + k] + m.getHeight(k) > 8) {
                    as = false;
                    break;
                }
            }
            if (as == false)continue;

            //a special situation: cant load the meta
            if (m == *ml.F_sevenHighL && heightT[i + 1] == 1)m = *ml.sevenHighL;
            if (m == *ml.F_sevenHighR && heightT[i] == 1)m = *ml.sevenHighR;

            pushUpdate(i, m);
            //std::cout<<i<<std::endl;
            if (depth == 15) {
                return true;
            }
            if (!dfs(depth + 1)) {
                popUpdate(i);
                if (cnttmp > 100) {
                    if (depth <= cntIni + 2) {
                        //std::cout << "!";
                        cnttmp = 0;
                        continue;
                    } else {
                        return false;
                    }
                }
            } else return true;

        }
    }
    //if(depth == cntIni){
    //    return dfs(cntIni);
    //}else
    return false;
}

//int a[15][5];
void GameMap::generate() {
    for(int i=0;i<8;i++){
        metaQueue[i].clear();
        heightT[i] = 0;
    }
    totQueue.clear();
    cnt = 0;
    //First Step: Set the base Meta in random;
    //01. 1 or 3?
    if (QRandomGenerator::global()->bounded(100) % 5 > 1) {
        if (QRandomGenerator::global()->bounded(100) % 2) {
            //1,3
            int posfirst = (QRandomGenerator::global()->bounded(100) % 3) * 2;
            pushUpdate(posfirst, QRandomGenerator::global()->bounded(100) % 2 == 1 ? *ml.sevenLowR : *ml.sevenLowL);
            if (posfirst % 2) {
                posfirst = QRandomGenerator::global()->bounded(100) % posfirst;
                if (posfirst % 2)posfirst--;
            } else {
                posfirst = 7 - (QRandomGenerator::global()->bounded(100) % (5 - posfirst));
                if (posfirst % 2 == 0)posfirst++;
            }
            pushUpdate(posfirst, *ml.stickH);
        } else {
            //3,3
            int posfirst = (QRandomGenerator::global()->bounded(100) % 2) * 2;
            pushUpdate(posfirst, QRandomGenerator::global()->bounded(100) % 2 == 1 ? *ml.sevenLowR : *ml.sevenLowL);
            posfirst = posfirst + 3;
            if (QRandomGenerator::global()->bounded(100) % 3 > 1)posfirst = 5;
            pushUpdate(posfirst, QRandomGenerator::global()->bounded(100) % 2 == 1 ? *ml.sevenLowR : *ml.sevenLowL);
        }
    } else if (QRandomGenerator::global()->bounded(100) % 5 > 3) {//02. 4?
        int posfirst = (QRandomGenerator::global()->bounded(100) % 3) * 2;
        pushUpdate(posfirst, *ml.stickW);
    }
    //03. fill with 2
    for (int i = 0; i < 8; i += 2) {
        if (metaQueue[i].empty()) {
            int tmp[3] = {2, 3, 6};
            pushUpdate(i, *ml.MetaList_Ground[tmp[QRandomGenerator::global()->bounded(100) % 3]]);
        } else if (metaQueue[i][0].getWidth() == 3)i++;
        else if (metaQueue[i][0].getWidth() == 1)i--;
        else if (metaQueue[i][0].getWidth() == 4)i += 2;
    }
    cnt = totQueue.size();
    //Second Step: Fill the map in random;
    //Method: Search, which may fail several times, but wouldn't take so much time.
    cntIni = cnt;
    dfs(cnt);


    //Third Step: Set the block;
    int tmpH[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int tmp = 0; tmp < 16; tmp++) {
        Meta mt = totQueue[tmp];
        int i = mt.posX;
        int posH;
        //        if (base) {
        if (tmp < cntIni) {
            posH = mt.getHeight();
        } else {
            int maxH = 8, minH = mt.getHeight();
            for (int j = 0; j < mt.getWidth(); j++) {
                int disToBottom;
                for (int k = 0; k < 4; k++) {
                    if (mt.graph[k][j])disToBottom = k;
                }
                maxH = fmin(maxH, tmpH[i + j] + mt.getHeight(j) - disToBottom - 1);
            }
            if (maxH < minH)maxH = minH;
            posH = QRandomGenerator::global()->bounded(100) % (maxH - minH + 1) + minH;
        }

        //inverse
        if (QRandomGenerator::global()->bounded(100) % 2) {
            for (int k = 0; k < 4; k++) {
                for (int j = 0; j < 4; j++) {
                    if (mt.graph[k][j])mt.graph[k][j] = 5 - mt.graph[k][j];
                }
            }
        }
        QString word = il.idiom[QRandomGenerator::global()->bounded(100) % 1000];
        for (int x = 0; x < mt.getWidth(); x++) {
            //warn: as 0 means "no block", the index of each column begin in 1, while the index in vector begin in 0
            tmpH[i + x] += mt.getHeight(x);
            for (int y = 3; y >= 0; y--) { //begin from the bottom
                if (mt.graph[y][x] == 0)continue;
                Block tmpB(tmp, mt.graph[y][x], word);
                this->mapBlock[i + x].insert(this->mapBlock[i + x].begin() + posH - y - 1, tmpB);
            }
        }
    }
    //printID();
    //printMap();
}

void GameMap::printMap() {
    for (int j = 7; j >= 0; j--) {
        for (int it = 0; it < 8; it++) {
            if (j >= this->mapBlock[it].size()) {
                std::cout << "  ";
            } else {
                std::cout << this->mapBlock[it][j].getCharacter().toStdString();

            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameMap::printID() {
    for (int j = 7; j >= 0; j--) {
        for (int it = 0; it < 8; it++) {
            if (j >= this->mapBlock[it].size())std::cout << "0";
            else {
                char tmpC = 'a' - 1;
                tmpC += this->mapBlock[it][j].getParent();
                std::cout << tmpC;

            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool GameMap::removeable(int parent) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < mapBlock[i].size(); j++) {
            if (mapBlock[i][j].getParent() == parent && mapBlock[i][j].getIndex() == 1)return removeable(i, j);
        }
    }
    return false;
}

bool GameMap::removeable(int posx, int posy) {
    int pid = mapBlock[posx][posy].getParent();
    if (mapBlock[posx][posy].getIndex() == 1) {
        int t = 2;
        while (t <= 4) {
            if (posx - 1 >= 0 && posy < mapBlock[posx - 1].size() && mapBlock[posx - 1][posy].getParent() == pid &&
                mapBlock[posx - 1][posy].getIndex() == t) {
                posx--;
            } else if (posy + 1 < mapBlock[posx].size() && mapBlock[posx][posy + 1].getParent() == pid &&
                       mapBlock[posx][posy + 1].getIndex() == t) {
                posy++;
            } else if (posy - 1 >= 0 && mapBlock[posx][posy - 1].getParent() == pid &&
                       mapBlock[posx][posy - 1].getIndex() == t) {
                posy--;
            } else if (posx + 1 < 8 && posy < mapBlock[posx + 1].size() &&
                       mapBlock[posx + 1][posy].getParent() == pid &&
                       mapBlock[posx + 1][posy].getIndex() == t) {
                posx++;
            } else break;
            t++;
        }
        if (t == 5) {
            return true;
        }
    }
    return false;
}

void GameMap::updateTip() {
    tiplist.clear();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < mapBlock[i].size(); j++) {
            if (removeable(i, j))tiplist.push_back(mapBlock[i][j].getParent());
        }
    }
}

bool GameMap::rmMeta(int parent) {
    //Capture to history first
    BlockCapture bp;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (j < mapBlock[i].size()) {
                bp.map[i][j] = mapBlock[i][j];
            } else {
                bp.map[i][j] = bp.nullBlock;
            }
        }
    }
    historyMap.push(bp);
    //
    //if (!removeable(parent))return false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < mapBlock[i].size(); j++) {
            if (mapBlock[i][j].getParent() == parent) {
                mapBlock[i].erase(mapBlock[i].begin() + j);
                j--;
            }
        }
    }
    printID();
    updateTip();
    for (int i = 0; i < tiplist.size(); i++)std::cout << (char) ('a' - 1 + tiplist[i]);
    return true;
}

bool GameMap::rmMeta(int posx, int posy) {
    return rmMeta(mapBlock[posx][posy].getParent());
}

bool GameMap::revoke() {
    if (historyMap.empty())return false;
    BlockCapture bp = historyMap.top();
    historyMap.pop();
    for (int i = 0; i < 8; i++) {
        mapBlock[i].clear();
        for (int j = 0; j < 8; j++) {
            if (bp.map[i][j].getParent() != -2) {
                mapBlock[i].push_back(bp.map[i][j]);
            }
        }
    }
    return true;
}
