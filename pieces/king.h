#ifndef __KING_H__
#define __KING_H__
#include "../piece.h"
using namespace std;

class King: public Piece {

    bool validMove(pair<int, int> start, pair<int, int> end) override;

    bool isInCheck();
    
    public: 
        King(Board *b, int color, int x, int y);
        int notify(Subject &s) override;
        bool movePiece(pair<int, int> sc, pair<int, int> ec) override;
        Piece* clone() const override {
            return new King(*this);
        }
};

#endif
