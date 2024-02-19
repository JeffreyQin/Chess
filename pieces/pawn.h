#ifndef __PAWN_H__
#define __PAWN_H__
#include "../piece.h"
using namespace std;

class Pawn: public Piece {
    bool validMove(pair<int,int> start, pair<int,int> end) override;

    public:
        Pawn(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new Pawn(*this);
        }
};

#endif
