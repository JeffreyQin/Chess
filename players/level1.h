#ifndef __LEVEL1_H__
#define __LEVEL1_H__
#include "player.h"

class LevelOne: public Player {
    public:
        LevelOne(Board *b, int color);
        Move makeMove() override;
};
#endif
