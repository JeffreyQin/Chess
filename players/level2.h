#ifndef __LEVEL2_H__
#define __LEVEL2_H__
#include "player.h"

class LevelTwo: public Player {
    public:
        LevelTwo(Board *b, int color);
        Move makeMove() override;
};
#endif
