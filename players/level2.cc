#include "level2.h"
#include <algorithm>
#include <random>
#include <cassert>
using namespace std;

LevelTwo::LevelTwo(Board *b, int color): Player{b, color} {}

Move LevelTwo::makeMove() {
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
    Move arbitraryMv{nullptr, nullptr, {0, 0}, {0, 0}, false};
    Move checkMv{nullptr, nullptr, {0, 0}, {0, 0}, false};
    Move captureMv{nullptr, nullptr, {0, 0}, {0, 0}, false};
    Move checkAndCapture{nullptr, nullptr, {0, 0}, {0, 0}, false};
    for(int i: iOrder) {
        for(int j: jOrder) {
            if(board->board[i][j].piece == nullptr || board->board[i][j].piece->getColor() != color) continue;
            for(int x: xOrder) {
                for(int y: yOrder) {
                    if(board->movePiece({i, j}, {x, y})) {
                        Move mv = board->undoMoveHistory.top();
                        bool isCapture = mv.getCapturedPiece() != nullptr;
                        bool isCheck = false;
                        for(int ii = 0; ii < 8; ii++) {
                            for(int jj = 0; jj < 8; jj++) {
                                if(board->board[ii][jj].isAttacked(color) && board->board[ii][jj].piece != nullptr && board->board[ii][jj].piece->getColor() != color && board->board[ii][jj].piece->getPieceType() == 'k') {
                                    isCheck = true;
                                }
                            }
                        }
                        board->undoMove();
                        if(isCheck && isCapture) checkAndCapture = mv;
                        else if(isCapture) captureMv = mv;
                        else if(isCheck) checkMv = mv;
                        else arbitraryMv = mv;
                    }
                }
            }
        }
    }
    if(checkAndCapture.getPiece() != nullptr) {
        return checkAndCapture;
    }
    if(captureMv.getPiece() != nullptr) {
        return captureMv;
    }
    if(checkMv.getPiece() != nullptr) {
        return checkMv;
    }
    return arbitraryMv;
}
