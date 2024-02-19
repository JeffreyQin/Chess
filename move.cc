#include "move.h"
#include <utility>
using namespace std;

Move::Move(Piece *pc, Piece *capturedPc, pair<int, int> sc, pair<int, int> ec, bool ep, char promote):
    piece{pc}, capturedPiece{capturedPc}, startCoord{sc}, endCoord{ec}, enPassant{ep}, promotePiece{promote} {}


Piece *Move::getCapturedPiece() {
    return capturedPiece;
}

Piece *Move::getPiece() {
    return piece;
}

pair<int, int> Move::getStartCoord() {
    return startCoord;
}

pair<int, int> Move::getEndCoord() {
    return endCoord;
}

bool Move::isEnPassant() {
    return enPassant;
}

char Move::getPromotePiece() {
    return promotePiece;
}
