#include "pawn.h"
#include "../board.h"
#include <iostream>
using namespace std;

Pawn::Pawn(Board *b, int color, int x, int y): Piece{b, color, 'p', 1, x, y} {
    firstMove = true;
    canEnPassant = false;
}

int Pawn::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    if (getColor()) {
        if(x - 1 >= 0 && y - 1 >= 0) board->board[x - 1][y - 1].addAttacker(getColor());
        if(x + 1 < 8 && y - 1 >= 0) board->board[x + 1][y - 1].addAttacker(getColor());
    } else {
        if(x - 1 >= 0 && y + 1 < 8) board->board[x - 1][y + 1].addAttacker(getColor());
        if(x + 1 < 8 && y + 1 < 8) board->board[x + 1][y + 1].addAttacker(getColor());
    }
    return 0;
}

bool Pawn::validMove(pair<int,int> start, pair<int,int> end) {
    if(start != coord || start == end) return false;
    int dir = getColor() ? -1 : 1;
    if(start.second + 2 * dir == end.second) { //first move
        if(start.first != end.first) {
            return false;
        }
        if((getColor() && start.second != 6) || (!getColor() && start.second != 1)) {
            return false;
        }
        if(board->board[start.first][start.second + 2 * dir].piece != nullptr || board->board[start.first][start.second  + dir].piece != nullptr) {
            return false;
        }
        return true;
    } else if(start.first != end.first) { //capture
        if(abs(start.first - end.first) > 1) return false;
        if(start.second + dir != end.second) return false;
        if(board->board[end.first][end.second].piece == nullptr) {
            if(board->board[end.first][start.second].piece == nullptr || 
            board->board[end.first][start.second].piece->getPieceType() != 'p' || 
            board->board[end.first][start.second].piece->getColor() == getColor() ||
            !board->board[end.first][start.second].piece->canEnPassant) {
                return false;
            }
            return true;
        }
        return board->board[end.first][end.second].piece != nullptr && board->board[end.first][end.second].piece->getColor() != getColor();
    }
    return start.first == end.first && start.second + dir == end.second && board->board[end.first][end.second].piece == nullptr;
}

bool Pawn::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        coord = ec;
        if(abs(sc.second - ec.second) == 2) {
            canEnPassant = true;
        }
        else canEnPassant = false;
        firstMove = false;
        return true;
    }
    return false;
}


