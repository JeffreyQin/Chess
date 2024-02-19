#include "bishop.h"
#include "../board.h"
using namespace std;

Bishop::Bishop(Board *b, int color, int x, int y): Piece{b, color, 'b', 3, x, y} {}

int Bishop::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    int ctr = 1;
    bool tl, tr, bl, br;
    tl = tr = bl = br = true;
    while (tl || tr || bl || br) {
        if (x - ctr < 0) bl = tl = false;
        if (x + ctr > 7) br = tr = false;
        if (y - ctr < 0) br = bl = false;
        if (y + ctr > 7) tr = tl = false;
        if (tl) {
            board->board[x - ctr][y + ctr].addAttacker(getColor());
            if (board->board[x - ctr][y + ctr].piece != nullptr) {
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
            board->board[x + ctr][y - ctr].addAttacker(getColor());
            if (board->board[x + ctr][y - ctr].piece != nullptr) {
                br = false;
            }
        }
        ctr ++;
    }
    return 0;
}

bool Bishop::validMove(pair<int, int> start, pair<int, int> end) {
    if (start != coord || start == end) {
        return false;
    }
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

bool Bishop::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        coord = ec;
        return true;
    }
    return false;
}

