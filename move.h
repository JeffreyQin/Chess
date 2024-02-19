#ifndef __MOVE_H__
#define __MOVE_H__
#include "piece.h"
#include <utility>
using namespace std;

class Move {
    Piece *piece;
    pair<int, int> startCoord;
    pair<int, int> endCoord;
    bool enPassant;
    char promotePiece;

    public:
        Piece *capturedPiece;
        Move(Piece *pc, Piece *capturedPc, pair<int, int> sc, pair<int, int> ec, bool ep = false, char promote = 'q');
        Piece *getCapturedPiece();
        Piece *getPiece();
        pair<int, int> getStartCoord();
        pair<int, int> getEndCoord();
        bool isEnPassant();
        char getPromotePiece();
};

#endif
