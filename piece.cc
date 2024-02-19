#include "piece.h"
#include "board.h"
#include <utility>
using namespace std;

Piece::Piece(Board *b, int color, char pieceType, int pointValue, int x, int y): 
board{board}, color{color}, pieceType{pieceType}, pointValue{pointValue}, coord{x, y} {
}

int Piece::getColor() {
    return color;
}

char Piece::getPieceType() {
    return pieceType;
}

int Piece::getPointValue() {
    return pointValue;
}

pair<int, int> Piece::getCoord() {
    return coord;
}

Board *Piece::getBoard() {
    return board;
}

Piece::~Piece() {}
