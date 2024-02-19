#ifndef __QUEEN_H__
#define __QUEEN_H__
#include "../piece.h"
using namespace std;

class Queen: public Piece {

    bool validMove(pair<int,int> start, pair<int,int> end) override;

    public: 
        Queen(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new Queen(*this);
        }
};

#endif
