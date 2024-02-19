#include "rook.h"
#include "../board.h"
using namespace std;

Rook::Rook(Board *b, int color, int x, int y): Piece{b, color, 'r', 5, x, y} {
    canCastle = true;
}

int Rook::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    int ctr = 1;
    bool t, b, l, r;
    t = b = l = r = true;
    while (t || b || l || r) {
        if (x - ctr < 0) b = false;
        if (x + ctr > 7) t = false;
        if (y - ctr < 0) l = false;
        if (y + ctr > 7) r = false;
        if (t) {
            board->board[x + ctr][y].addAttacker(getColor());
            if (board->board[x + ctr][y].piece != nullptr) {
                t = false;
            } 
        }
        if (b) {
            board->board[x - ctr][y].addAttacker(getColor());
            if (board->board[x - ctr][y].piece != nullptr) {
                b = false;
            } 
        }
        if (l) {
            board->board[x][y - ctr].addAttacker(getColor());
            if (board->board[x][y - ctr].piece != nullptr) {
                l = false;
            } 
        }
        if (r) {
            board->board[x][y + ctr].addAttacker(getColor());
            if (board->board[x][y + ctr].piece != nullptr) {
                r = false;
            } 
        }
        ctr ++;
    }
    return 0;
}

bool Rook::validMove(pair<int, int> start, pair<int, int> end) {
    if (start != coord || start == end) return false;
    if (start.first != end.first && start.second != end.second) return false;
    if (board->board[end.first][end.second].piece != nullptr && board->board[end.first][end.second].piece->getColor() == getColor()) return false;
    if (start.first == end.first) {
        int dir = (start.second < end.second)? 1 : -1;
        for (int i = start.second + dir; i * dir < end.second * dir; i += dir) {
            if (board->board[start.first][i].piece != nullptr) {
                return false;
            }
        }
    } else {
        int dir = (start.first < end.first)? 1: -1;
        for (int i = start.first + dir; i * dir < end.first * dir; i += dir) {
            if (board->board[i][start.second].piece != nullptr) {
                return false;
            }
        }
    }
    return true;
}

bool Rook::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        coord = ec;
        canCastle = false;
        return true;
    }
    return false;
}

