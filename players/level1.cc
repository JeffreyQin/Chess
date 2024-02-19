#include "level1.h"
#include <algorithm>
#include <random>
#include <cassert>
using namespace std;

LevelOne::LevelOne(Board *b, int color): Player{b, color} {}

Move LevelOne::makeMove() {
    vector<int> iOrder(8), jOrder(8), xOrder(8), yOrder(8);
    for(int i = 0; i < 8; i++) {
        iOrder[i] = i;
        jOrder[i] = i;
        xOrder[i] = i;
        yOrder[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(iOrder.begin(), iOrder.end(), g);
    shuffle(jOrder.begin(), jOrder.end(), g);
    shuffle(xOrder.begin(), xOrder.end(), g);
    shuffle(yOrder.begin(), yOrder.end(), g);

    for(int i: iOrder) {
        for(int j: jOrder) {
            if(board->board[i][j].piece == nullptr || board->board[i][j].piece->getColor() != color) continue;
            for(int x: xOrder) {
                for(int y: yOrder) {
                    Move mv{board->board[i][j].piece->clone(), nullptr, {i, j}, {x, y}, false};
                    if(board->movePiece({i, j}, {x, y})) {
                        board->undoMove();
                        return mv;
                    }
                }
            }
        }
    }
    assert(false);
    return {nullptr, nullptr, {0, 0}, {0, 0}, false};
}
