#include "king.h"
#include "../board.h"
using namespace std;

King::King(Board *b, int color, int x, int y): Piece{b, color, 'k', 100000000, x, y} {
    canCastle = true;
}

int King::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    if (x > 0) {
        board->board[x - 1][y].addAttacker(getColor());
    }
    if (x < 7) {
        board->board[x + 1][y].addAttacker(getColor());
    }
    if (y > 0) {
        board->board[x][y - 1].addAttacker(getColor());
    }
    if (y < 7) {
        board->board[x][y + 1].addAttacker(getColor());
    }
    if (x > 0 && y > 0) {
        board->board[x - 1][y - 1].addAttacker(getColor());
    }
    if (x > 0 && y < 7) {
        board->board[x - 1][y + 1].addAttacker(getColor());
    }
    if (x < 7 && y > 0) {
        board->board[x + 1][y - 1].addAttacker(getColor());
    }
    if (x < 7 && y < 7) {
        board->board[x + 1][y + 1].addAttacker(getColor());
    }
    return 0;
}

bool King::validMove(pair<int, int> start, pair<int, int> end) {
    if (start != coord || start == end) return false;
    if (board->board[end.first][end.second].piece != nullptr && board->board[end.first][end.second].piece->getColor() == getColor()) return false;
    if (canCastle && abs(end.first - start.first) == 2) { //castling
        if(start.second != end.second) return false;
        int dir = (end.first > start.first)? 1 : -1;
        for (int i = start.first + dir; i > 0 && i < 7; i += dir) {
            if (board->board[i][coord.second].piece != nullptr) {
                return false;
            }
        }
        int rookPos = dir == -1 ? 0 : 7;
        if(board->board[rookPos][start.second].piece == nullptr || 
        board->board[rookPos][start.second].piece->getPieceType() != 'r' || 
        !board->board[rookPos][start.second].piece->canCastle ||
        board->board[rookPos][start.second].piece->getColor() != getColor()) {
            return false;
        }
        return true;
    } else {
        if (abs(end.first - start.first) <= 1 && abs(end.second - start.second) <= 1) {
            return true;
        }
    }
    return false;
}

bool King::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        canCastle = false;
        coord = ec;
        return true;
    }
    return false;
}

bool King::isInCheck() {
    return board->board[coord.first][coord.second].isAttacked(getColor());
}
