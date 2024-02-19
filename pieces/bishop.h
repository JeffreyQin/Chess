#ifndef __BISHOP_H__
#define __BISHOP_H__
#include "../piece.h"
using namespace std;

class Bishop: public Piece {

    bool validMove(pair<int, int> start, pair<int, int> end) override;

    public:
        Bishop(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new Bishop(*this);
        }
};

#endif
