#include "knight.h"
#include "../board.h"
using namespace std;

Knight::Knight(Board *b, int color, int x, int y): Piece{b, color, 'n', 3, x, y} {}

int Knight::notify(Subject &s) {
    int x = getCoord().first;
    int y = getCoord().second;
    if (x >= 2) {
        if (y >= 1) board->board[x - 2][y - 1].addAttacker(getColor());
        if (y <= 6) board->board[x - 2][y + 1].addAttacker(getColor());
    }
    if (x >= 1) {
        if (y >= 2) board->board[x - 1][y - 2].addAttacker(getColor());
        if (y <= 5) board->board[x - 1][y + 2].addAttacker(getColor());
    }
    if (x <= 5) {
        if (y >= 1) board->board[x + 2][y - 1].addAttacker(getColor());
        if (y <= 6) board->board[x + 2][y + 1].addAttacker(getColor());
    }
    if (x <= 6) {
        if (y >= 2) board->board[x + 1][y - 2].addAttacker(getColor());
        if (y <= 5) board->board[x + 1][y + 2].addAttacker(getColor());
    }
    return 0;
}

bool Knight::validMove(pair<int,int> start, pair<int,int> end) {
    if(start != coord || start == end) return false;
    int xChange = abs(start.first - end.first);
    int yChange = abs(start.second - end.second);
    if(minmax(xChange, yChange) != minmax(1, 2)) return false;
    if(board->board[end.first][end.second].piece != nullptr && board->board[end.first][end.second].piece->getColor() == getColor()) {
        return false;
    }
    return true;
}

bool Knight::movePiece(pair<int, int> sc, pair<int, int> ec) {
    if (validMove(sc, ec)) {
        coord = ec;
        return true;
    }
    return false;
}
