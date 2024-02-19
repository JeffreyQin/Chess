#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "../piece.h"
using namespace std;

class Knight: public Piece {

    bool validMove(pair<int, int> start, pair<int, int> end) override;

    public:
        Knight(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new Knight(*this);
        }
};

#endif
