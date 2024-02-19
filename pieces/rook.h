#ifndef __ROOK_H__
#define __ROOK_H__
#include "../piece.h"
using namespace std;

class Rook: public Piece {

    bool validMove(pair<int, int> start, pair<int, int> end) override;

    public:
        Rook(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new Rook(*this);
        }
};

#endif
