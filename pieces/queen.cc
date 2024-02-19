#include "queen.h"
#include "../board.h"
using namespace std;

Queen::Queen(Board *b, int color, int x, int y): Piece{b, color, 'q', 9, x, y} {}

int Queen::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    int ctr = 1;
    bool tl, tr, bl, br, t, b, l, r;
    tl = tr = bl = br = t = b = l = r = true;
    while (tl || tr || bl || br || t || b || l || r) {
        if (x - ctr < 0) bl = br = b = false;
        if (x + ctr > 7) tl = tr = t = false;
        if (y - ctr < 0) tl = bl = l = false;
        if (y + ctr > 7) tr = br = r = false;
        if (tl) {
            board->board[x + ctr][y - ctr].addAttacker(getColor());
            if (board->board[x + ctr][y - ctr].piece != nullptr) {
                tl = false;
            }
        }
        if (tr) {
            board->board[x + ctr][y + ctr].addAttacker(getColor());
            if (board->board[x + ctr][y + ctr].piece != nullptr) {
                tr = false;
            }
        }
        if (bl) {
            board->board[x - ctr][y - ctr].addAttacker(getColor());
            if (board->board[x - ctr][y - ctr].piece != nullptr) {
                bl = false;
            }
        }
        if (br) {
            board->board[x - ctr][y + ctr].addAttacker(getColor());
            if (board->board[x - ctr][y + ctr].piece != nullptr) {
                br = false;
            }
        }
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


bool Queen::validMove(pair<int,int> start, pair<int,int> end) {
    if(start == end || start != coord) return false;
    if(start.first == end.first || start.second == end.second) {
        //do rook move
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
    //do bishop move
    int xDirection = start.first < end.first ? 1 : -1;
    int yDirection = start.second < end.second ? 1 : -1;
    if(abs(end.first - start.first) != abs(end.second - start.second)) return false;
    for(int x = start.first + xDirection, y = start.second + yDirection;; x += xDirection, y += yDirection) {
        if(x == end.first) { // check if friendly piece
            if(board->board[x][y].piece != nullptr && board->board[x][y].piece->getColor() == getColor()) {
                return false;
            }
            break;
        } else {
            if(board->board[x][y].piece != nullptr) {
                return false;
            }
        }
    }
    return true;
}

bool Queen::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        coord = ec;
        return true;
    }
    return false;
}



