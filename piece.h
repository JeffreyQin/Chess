#ifndef __PIECE_H__
#define __PIECE_H__
#include "observer.h"
#include <utility>
#include <iostream>

using namespace std;

class Board;

class Piece: public Observer {
    int color;
    char pieceType;
    int pointValue;

    virtual bool validMove(pair<int, int> start, pair<int, int> end) = 0;

    public:
        pair<int, int> coord;
        Board *board;
        bool canCastle;
        bool canEnPassant;
        bool firstMove;
        Piece(Board *b, int color, char pieceType, int pointValue, int x, int y);
        int getColor();
        char getPieceType();
        int getPointValue();
        virtual bool movePiece(pair<int, int> sc, pair<int, int> ec) = 0;
        virtual Piece* clone() const = 0;
        pair<int,int> getCoord();
        virtual ~Piece();
        Board *getBoard();
};



#endif
