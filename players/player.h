#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "../board.h"
#include "../move.h"

class Player {
    protected:
        int color;
        Board *board;

    public:
        Player(Board *b, int color);
        virtual Move makeMove() = 0;
};

#endif
