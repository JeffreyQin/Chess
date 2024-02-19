#ifndef __LEVEL3_H__
#define __LEVEL3_H__
#include "player.h"

class LevelThree: public Player {
    public:
        LevelThree(Board *b, int color);
        Move makeMove() override;
};
#endif
