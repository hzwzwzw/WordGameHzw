//
// Created by 23576 on 2023/3/2.
//

#ifndef WORDGAME_METALIST_H
#define WORDGAME_METALIST_H

#include "Meta.h"

class MetaList {
public:
    const Meta *stickW = new Meta(4, 1, (int[][4])
            {{1, 2, 3, 4},
             {0, 0, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *stickH = new Meta(1, 4, (int[][4])
            {{1, 0, 0, 0},
             {2, 0, 0, 0},
             {3, 0, 0, 0},
             {4, 0, 0, 0}});

    const Meta *sevenHighL = new Meta(2, 3, (int[][4])
            {{1, 0, 0, 0},
             {2, 0, 0, 0},
             {3, 4, 0, 0},
             {0, 0, 0, 0}});

    const Meta *sevenHighR = new Meta(2, 3, (int[][4])
            {{0, 1, 0, 0},
             {0, 2, 0, 0},
             {4, 3, 0, 0},
             {0, 0, 0, 0}});

    const Meta *sevenLowL = new Meta(3, 2, (int[][4])
            {{1, 0, 0, 0},
             {2, 3, 4, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *sevenLowR = new Meta(3, 2, (int[][4])
            {{0, 0, 1, 0},
             {4, 3, 2, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *square = new Meta(2, 2, (int[][4])
            {{1, 4, 0, 0},
             {2, 3, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});


    const Meta *F_sevenHighL = new Meta(2, 3, (int[][4])
            {{2, 1, 0, 0},
             {3, 0, 0, 0},
             {4, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_sevenHighR = new Meta(2, 3, (int[][4])
            {{1, 2, 0, 0},
             {0, 3, 0, 0},
             {0, 4, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_sevenLowL = new Meta(3, 2, (int[][4])
            {{2, 3, 4, 0},
             {1, 0, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_sevenLowR = new Meta(3, 2, (int[][4])
            {{1, 2, 3, 0},
             {0, 0, 4, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_interHighL = new Meta(2, 3, (int[][4])
            {{1, 0, 0, 0},
             {2, 3, 0, 0},
             {0, 4, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_interHighR = new Meta(2, 3, (int[][4])
            {{0, 1, 0, 0},
             {3, 2, 0, 0},
             {4, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_interLowL = new Meta(3, 2, (int[][4])
            {{1, 2, 0, 0},
             {0, 3, 4, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *F_interLowR = new Meta(3, 2, (int[][4])
            {{0, 3, 4, 0},
             {1, 2, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}});

    const Meta *MetaList_Ground[7] = {stickW, stickH,
                                      sevenHighL, sevenHighR, sevenLowL, sevenLowR,
                                      square};
    const Meta *MetaList_Float[8] = {F_sevenHighL, F_sevenHighR, F_sevenLowL, F_sevenLowR,
                                     F_interHighL, F_interHighR, F_interLowL, F_interLowR};

    const Meta *MetaList_All[15] = {stickW, stickH,
                                    sevenHighL, sevenHighR, sevenLowL, sevenLowR,
                                    square,
                                    F_sevenHighL, F_sevenHighR, F_sevenLowL, F_sevenLowR,
                                    F_interHighL, F_interHighR, F_interLowL, F_interLowR};
};


#endif //WORDGAME_METALIST_H
