#include <iostream>
#include <limits>
#include "level4.h"

using namespace std;

LevelFour::LevelFour(Board *board, int color): Player{board, color} {
    allMoves = new MoveTree();
    allMoves->evalScore = 0;
    allMoves->mv = nullptr;
    head = allMoves;
}
LevelFour::~LevelFour() {
    delete head; // head dummy var wont be needed if shared_ptr is used
}

LevelFour::MoveTree::MoveTree(): evalScore{0}, mv{nullptr} {}
LevelFour::MoveTree::MoveTree(int evalScore, Move *mv): evalScore{evalScore}, mv{mv} {}
LevelFour::MoveTree::~MoveTree() {
    delete mv;
}

int LevelFour::eval(int curColor) {
    int totalPhase = gamePhaseWeights.at('t');
    int phase = totalPhase;
    // determine game phase by looking at each piece on board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->board[i][j].piece != nullptr) {
                phase -= gamePhaseWeights.at(board->board[i][j].piece->getPieceType());
            }
        }
    }
    phase = (phase * 256 + (totalPhase / 2)) / totalPhase;
    int whiteScore = 0;
    int blackScore = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->board[i][j].piece != nullptr) {
                if (board->board[i][j].piece->getColor() == 0) {
                    whiteScore += baseWeights.at(board->board[i][j].piece->getPieceType());
                    if (board->board[i][j].piece->getPieceType() == 'k') {
                        int regular = positionalWeights.at('k')[i][j];
                        int endgame = positionalWeights.at('e')[i][j];
                        whiteScore += (((regular * (float)(256 - phase)) + (endgame * (float)phase)) / 256);
                    } else {
                        whiteScore += positionalWeights.at(board->board[i][j].piece->getPieceType())[i][j];
                    }
                } else if (board->board[i][j].piece->getColor() == 1) { // must invert col number
                    blackScore += baseWeights.at(board->board[i][j].piece->getPieceType());
                    if (board->board[i][j].piece->getPieceType() == 'k') {
                        int regular = positionalWeights.at('k')[i][7-j];
                        int endgame = positionalWeights.at('e')[i][7-j];
                        blackScore += (((regular * (float)(256 - phase)) + (endgame * (float)phase)) / 256);
                    } else {
                        blackScore += positionalWeights.at(board->board[i][j].piece->getPieceType())[i][7-j];
                    }
                }
            }
        }
    }
    int whiteEval = whiteScore - blackScore;
    if (curColor == 0) { // eval score from whites perspective
        return whiteEval;
    }
    // from blacks perspective
    return -whiteEval;
}

vector<LevelFour::MoveTree *> LevelFour::getAllMoves(int curColor) {
    vector<Move *> allMoves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // not our piece or no piece there
            if (board->board[i][j].piece == nullptr || board->board[i][j].piece->getColor() != curColor) {
                continue;
            }
            char pieceType = board->board[i][j].piece->getPieceType();
            Piece *curPc = board->board[i][j].piece;
            // the ordering of this check matters for quiescence search later (lowest value pieces are first in the resulting vector of moves)
            if (pieceType == 'p') {
                // white pawns move "down" the board (high to low row num), black pawns move "up"
                int moveAmount = -1;
                if (curColor == 0) {
                    moveAmount = 1;
                }
                // there is room in front of pawn
                if ((curColor == 0 && j < 7) || (curColor == 1 && j > 0)) {
                    // no piece in front of pawn
                    if (board->board[i][j+moveAmount].piece == nullptr) {
                        // standard move
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i,j + moveAmount), false, 'q'));
                        // 2nd last rank
                        if ((curColor == 0 && j == 6) || (curColor == 1 && j == 1)) {
                            // all other promotions
                            allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i,j + moveAmount), false, 'n'));
                            allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i,j + moveAmount), false, 'b'));
                            allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i,j + moveAmount), false, 'r'));
                        }
                        // first move can be twice (but only if on 2nd rank)
                        if ((curColor == 0 && j == 1) || (curColor == 1 && j == 6)) {
                            allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i,j + 2 * moveAmount), false, 'q'));
                        }
                    }
                    // capture moves
                    // white captures forward (right on array, + j coord)
                    if (curColor == 0) {
                        // not on left edge (top of array) and enemy piece left diagonal
                        if (i > 0 && board->board[i-1][j+1].piece != nullptr && board->board[i-1][j+1].piece->getColor() != curColor) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i-1][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j+1), false, 'q'));
                        }
                        // en passant to left: must make sure there is space, left diagonal is free, piece to its direct left is an enemy pawn, and cur pawn can en passant
                        if (i > 0 && board->board[i-1][j+1].piece == nullptr && board->board[i-1][j].piece != nullptr && board->board[i-1][j].piece->getColor() != curColor && board->board[i-1][j].piece->getPieceType() == 'p' && curPc->canEnPassant) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i-1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j+1), true, 'q'));
                        }
                        // not on right edge and enemy piece right diagonal
                        if (i < 7 && board->board[i+1][j+1].piece != nullptr && board->board[i+1][j+1].piece->getColor() != curColor) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i+1][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j+1), false, 'q'));
                        }
                        // en passant to right
                        if (i < 7 && board->board[i+1][j+1].piece == nullptr && board->board[i+1][j].piece != nullptr && board->board[i+1][j].piece->getColor() != curColor && board->board[i+1][j].piece->getPieceType() == 'p' && curPc->canEnPassant) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i+1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j+1), true, 'q'));
                        }
                    } else if (curColor == 1) { // black captures backwards (left on array, - j coord)
                        // capture down left (up on array)
                        if (i > 0 && board->board[i-1][j-1].piece != nullptr && board->board[i-1][j-1].piece->getColor() != curColor) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i-1][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j-1), false, 'q'));
                        }
                        if (i > 0 && board->board[i-1][j-1].piece == nullptr && board->board[i-1][j].piece != nullptr && board->board[i-1][j].piece->getColor() != curColor && board->board[i-1][j].piece->getPieceType() == 'p' && curPc->canEnPassant) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i-1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j-1), true, 'q'));
                        }
                        // capture down right (down on array)
                        if (i < 7 && board->board[i+1][j-1].piece != nullptr && board->board[i+1][j-1].piece->getColor() != curColor) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i+1][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j-1), false, 'q'));
                        }
                        if (i < 7 && board->board[i+1][j-1].piece == nullptr && board->board[i+1][j].piece != nullptr && board->board[i+1][j].piece->getColor() != curColor && board->board[i+1][j].piece->getPieceType() == 'p' && curPc->canEnPassant) {
                            allMoves.push_back(new Move(curPc->clone(), board->board[i+1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j-1), true, 'q'));
                        }
                    }
                }
            } else if (pieceType == 'n') {
                // 2 up (on array)
                if (i > 1) {
                    // 1 left (on array)
                    if (j > 0 && (board->board[i-2][j-1].piece == nullptr || board->board[i-2][j-1].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-2][j-1].piece == nullptr) ? nullptr : board->board[i-2][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-2,j-1), false, 'q'));
                    }
                    // 1 right (on array)
                    if (j < 7 && (board->board[i-2][j+1].piece == nullptr || board->board[i-2][j+1].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-2][j+1].piece == nullptr) ? nullptr : board->board[i-2][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-2,j+1), false, 'q'));
                    }
                }
                // 1 up
                if (i > 0) {
                    // 2 left
                    if (j > 1 && (board->board[i-1][j-2].piece == nullptr || board->board[i-1][j-2].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-1][j-2].piece == nullptr) ? nullptr : board->board[i-1][j-2].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j-2), false, 'q'));
                    }
                    // 2 right
                    if (j < 6 && (board->board[i-1][j+2].piece == nullptr || board->board[i-1][j+2].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-1][j+2].piece == nullptr) ? nullptr : board->board[i-1][j+2].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1,j+2), false, 'q'));
                    }
                }
                // 2 down
                if (i < 6) {
                    // 1 left
                    if (j > 0 && (board->board[i+2][j-1].piece == nullptr || board->board[i+2][j-1].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+2][j-1].piece == nullptr) ? nullptr : board->board[i+2][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+2,j-1), false, 'q'));
                    }
                    // 1 right
                    if (j < 7 && (board->board[i+2][j+1].piece == nullptr || board->board[i+2][j+1].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+2][j+1].piece == nullptr) ? nullptr : board->board[i+2][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+2,j+1), false, 'q'));
                    }
                }
                // 1 down
                if (i < 7) {
                    // 2 left
                    if (j > 1 && (board->board[i+1][j-2].piece == nullptr || board->board[i+1][j-2].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+1][j-2].piece == nullptr) ? nullptr : board->board[i+1][j-2].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j-2), false, 'q'));
                    }
                    // 2 right
                    if (j < 6 && (board->board[i+1][j+2].piece == nullptr || board->board[i+1][j+2].piece->getColor() != curColor)) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+1][j+2].piece == nullptr) ? nullptr : board->board[i+1][j+2].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1,j+2), false, 'q'));
                    }
                }
            } else if (pieceType == 'b') {
                // down + right
                int curI = i + 1;
                int curJ = j + 1;
                while (curI <= 7 && curJ <= 7) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                    curJ++;
                }
                // down + left
                curI = i + 1;
                curJ = j - 1;
                while (curI <= 7 && curJ >= 0) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                    curJ--;
                }
                // up + right
                curI = i - 1;
                curJ = j + 1;
                while (curI >= 0 && curJ <= 7) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                    curJ++;
                }
                // up + left
                curI = i - 1;
                curJ = j - 1;
                while (curI >= 0 && curJ >= 0) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                    curJ--;
                }
            } else if (pieceType == 'r') {
                // down
                int curI = i + 1;
                while (curI <= 7) {
                    // no piece there
                    if (board->board[curI][j].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                    } else if (board->board[curI][j].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                }
                // up
                curI = i - 1;
                while (curI >= 0) {
                    // no piece there
                    if (board->board[curI][j].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                    } else if (board->board[curI][j].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                }
                // right
                int curJ = i + 1;
                while (curJ <= 7) {
                    // no piece there
                    if (board->board[i][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                    } else if (board->board[i][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[i][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curJ++;
                }
                // left
                curJ = i - 1;
                while (curJ >= 0) {
                    // no piece there
                    if (board->board[i][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                    } else if (board->board[i][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[i][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curJ--;
                }
            } else if (pieceType == 'q') {
                // down
                int curI = i + 1;
                while (curI <= 7) {
                    // no piece there
                    if (board->board[curI][j].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                    } else if (board->board[curI][j].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                }
                // up
                curI = i - 1;
                while (curI >= 0) {
                    // no piece there
                    if (board->board[curI][j].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                    } else if (board->board[curI][j].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, j), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                }
                // right
                int curJ = i + 1;
                while (curJ <= 7) {
                    // no piece there
                    if (board->board[i][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                    } else if (board->board[i][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[i][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curJ++;
                }
                // left
                curJ = i - 1;
                while (curJ >= 0) {
                    // no piece there
                    if (board->board[i][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                    } else if (board->board[i][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[i][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curJ--;
                }
                // down + right
                curI = i + 1;
                curJ = j + 1;
                while (curI <= 7 && curJ <= 7) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                    curJ++;
                }
                // down + left
                curI = i + 1;
                curJ = j - 1;
                while (curI <= 7 && curJ >= 0) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI++;
                    curJ--;
                }
                // up + right
                curI = i - 1;
                curJ = j + 1;
                while (curI >= 0 && curJ <= 7) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                    curJ++;
                }
                // up + left
                curI = i - 1;
                curJ = j - 1;
                while (curI >= 0 && curJ >= 0) {
                    // no piece there
                    if (board->board[curI][curJ].piece == nullptr) {
                        allMoves.push_back(new Move(curPc->clone(), nullptr, pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                    } else if (board->board[curI][curJ].piece->getColor() != curColor) { // enemy piece
                        allMoves.push_back(new Move(curPc->clone(), board->board[curI][curJ].piece->clone(), pair<int,int>(i, j), pair<int,int>(curI, curJ), false, 'q'));
                        break;
                    } else { // ally piece
                        break;
                    }
                    curI--;
                    curJ--;
                }
            } else if (pieceType == 'k') {
                // check if squares attacked by opposite curColor, if not then add move
                // the 3 up moves
                if (i > 0) {
                    // "up"
                    if (!(board->board[i-1][j].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-1][j].piece == nullptr) ? nullptr : board->board[i-1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1, j), false, 'q'));
                    }
                    // "up" left
                    if (j > 0 && !(board->board[i-1][j-1].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-1][j-1].piece == nullptr) ? nullptr : board->board[i-1][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1, j-1), false, 'q'));
                    }
                    // "up" right
                    if (j < 7 && !(board->board[i-1][j+1].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i-1][j+1].piece == nullptr) ? nullptr : board->board[i-1][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i-1, j+1), false, 'q'));
                    }
                }
                // the 3 down moves
                if (i < 7) {
                    // "down"
                    if (!(board->board[i+1][j].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+1][j].piece == nullptr) ? nullptr : board->board[i+1][j].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1, j), false, 'q'));
                    } 
                    // "down" left
                    if (j > 0 && !(board->board[i+1][j-1].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+1][j-1].piece == nullptr) ? nullptr : board->board[i+1][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1, j-1), false, 'q'));
                    }
                    // "down" right
                    if (j < 7 && !(board->board[i+1][j+1].isAttacked(curColor == 0 ? 1 : 0))) {
                        allMoves.push_back(new Move(curPc->clone(), (board->board[i+1][j+1].piece == nullptr) ? nullptr : board->board[i+1][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i+1, j+1), false, 'q'));
                    }
                }
                 // left
                if (j > 0 && !(board->board[i][j-1].isAttacked(curColor == 0 ? 1 : 0))) {
                    allMoves.push_back(new Move(curPc->clone(), (board->board[i][j-1].piece == nullptr) ? nullptr : board->board[i][j-1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, j-1), false, 'q'));
                }
                // right
                if (j < 7 && !(board->board[i][j+1].isAttacked(curColor == 0 ? 1 : 0))) {
                    allMoves.push_back(new Move(curPc->clone(), (board->board[i][j+1].piece == nullptr) ? nullptr : board->board[i][j+1].piece->clone(), pair<int,int>(i, j), pair<int,int>(i, j+1), false, 'q'));
                }
            }
        }
    }
    
    vector<MoveTree *> allMoveTrees;
    for (auto mv : allMoves) {
        // passes if mv was a legal move
        if (board->movePiece(mv->getStartCoord(), mv->getEndCoord())) {
            board->turn = !board->turn;
            moves++;
            // need to switch color here, since for each of curColor's moves, we perceive it in the opposite perspective 
            // to see what they would ideally pick in response
            int evalScore = eval((curColor == 0) ? 1 : 0);
            board->undoMove();
            board->turn = !board->turn;
            undos++;
            allMoveTrees.push_back(new MoveTree(evalScore, mv)); // movetrees with just the evaluation for that move and the move itself
        }
    }
    return allMoveTrees;
}

// will populate allMoves tree, guaranteeing a population of at least depth moves ahead
void LevelFour::populateMoves(MoveTree *tree, int depth, int curColor) {
    if (tree == nullptr) {
        tree = new MoveTree();
    }
    // do not populate if depth is used up
    if (depth == 0) {
        return;
    }
    // if nextMoves array is not yet set
    if (tree->nextMoves.size() == 0) {
        // get all moves by curColor
        tree->nextMoves = getAllMoves(curColor);
        // curColor has no moves available in position, so if their king is in check they lose
        if (tree->nextMoves.size() == 0) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    // the square is nonempty, it is curColor's king, and it is being attacked by the enemy 
                    // since there were no possible moves for curColor, they are in checkmate
                    if (board->board[i][j].piece != nullptr && board->board[i][j].piece->getPieceType() == 'k' 
                    && board->board[i][j].piece->getColor() == curColor && board->board[i][j].isAttacked((curColor == 0) ? 1 : 0)) {
                        // this is a very bad position in the perspective of the opponent (which is the perspective the current position is in)
                        tree->evalScore = 999999999;
                    }
                }
            }
        }
    }
    // // for each next move (moveTree), recursively populate them
    for (auto nextMove : tree->nextMoves) {
        // move the piece (move of turn curColor), populate, and then undo the move
        if (board->movePiece(nextMove->mv->getStartCoord(), nextMove->mv->getEndCoord())) {
            board->turn = !board->turn;
            moves++;
            populateMoves(nextMove, depth - 1, (curColor == 0) ? 1 : 0); // swap color, so other color moves
            board->undoMove();
            board->turn = !board->turn;
            undos++;
        }
    }
}

// will quiesce the current board state in the perspective of curColor (i.e. check only noisy moves, i.e. check only capture moves)
// it is intended to be called with a modified board
// credit: https://www.chessprogramming.org/Quiescence_Search
pair<int, Move *> LevelFour::quiesce(MoveTree* curMv, int alpha, int beta, int depth, int curColor) {
    populateMoves(curMv, 1, curColor);
    // do nothing case
    int standPat = curMv->evalScore;
    if (depth == 0) {
        return pair<int, Move *>(standPat, curMv->mv);
    }
    if (standPat >= beta) {
        return pair<int, Move *>(beta, curMv->mv);
    }
    Move *bestMove = curMv->mv;
    if (alpha < standPat) {
        alpha = standPat;
    }
    if (curMv->nextMoves.size() == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // the square is nonempty, it is curColor's king, and it is being attacked by the enemy 
                // since there were no possible moves for curColor, they are in checkmate
                if (board->board[i][j].piece != nullptr && board->board[i][j].piece->getPieceType() == 'k' 
                && board->board[i][j].piece->getColor() == curColor && board->board[i][j].isAttacked((curColor == 0) ? 1 : 0)) {
                    // very very low score for a checkmate (in current perspective)
                  //  cout << "in qui search" << endl;
                    return pair<int, Move *>(-999999999, nullptr);
                }
            }
        }
        // otherwise, it is a stalemate
        return pair<int, Move *>(0, nullptr);
    }
    for (auto nextMove : curMv->nextMoves) {
        // we only look at moves where a piece was captured
        if (nextMove->mv->capturedPiece == nullptr) {
            continue;
        }
        if (board->movePiece(nextMove->mv->getStartCoord(), nextMove->mv->getEndCoord())) {
            board->turn = !board->turn;
            moves++;
            int score = -quiesce(nextMove, -beta, -alpha, depth - 1, (curColor == 0) ? 1 : 0).first; // the call returns best state for enemy, so negate it
            board->undoMove();
            board->turn = !board->turn;
            undos++;
            // exceeded global max (due to the way min-maxing each other's moves works)
            // opponent will never give you a better move.
            if (score >= beta) {
                return pair<int, Move *>(beta, bestMove);
            }
            // got a better move
            if (score > alpha) {
                bestMove = nextMove->mv;
                alpha = score;
            }
        }
    }
    return pair<int, Move *>(alpha, bestMove);
}

// negamax algorithm with alpha & beta pruning
// depth should be initially called as at least 1
pair<int, Move *> LevelFour::negamax(MoveTree *curMv, int alpha, int beta, int depth, int curColor) {
    if (depth == 0) {
        return quiesce(curMv, alpha, beta, 3, curColor);
    }
    populateMoves(curMv, 1, curColor);

    if (curMv->nextMoves.size() == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // the square is nonempty, it is curColor's king, and it is being attacked by the enemy 
                // since there were no possible moves for curColor, they are in checkmate
                if (board->board[i][j].piece != nullptr && board->board[i][j].piece->getPieceType() == 'k' 
                && board->board[i][j].piece->getColor() == curColor && board->board[i][j].isAttacked((curColor == 0) ? 1 : 0)) {
                    // very very low score for a checkmate (in current perspective)
                    return pair<int, Move *>(-999999999, nullptr);
                }
            }
        }
        // otherwise, it is a stalemate
        return pair<int, Move *>(0, nullptr);
    }
    Move *bestMove = nullptr;
    for (auto nextMove : curMv->nextMoves) {
        // try the move, analyze, then undo if successful
        if (board->movePiece(nextMove->mv->getStartCoord(), nextMove->mv->getEndCoord())) {
            board->turn = !board->turn;
            moves++;
            int score = -negamax(nextMove, -beta, -alpha, depth - 1, (curColor == 0) ? 1 : 0).first;
            board->undoMove();
            board->turn = !board->turn;
            undos++;
            if (score >= beta) {
                return pair<int, Move *>(beta, bestMove); // fail hard beta cutoff
            }
            if (score > alpha) {
                bestMove = nextMove->mv;
                alpha = score;
            }
        }
    }
    return pair<int, Move *>(alpha, bestMove);
}

Move LevelFour::makeMove() {
    // cout << "Calculating Move..." << endl;
    // locate the move the opponent just did, and set allMoves to it (now our turn)
    if (!board->undoMoveHistory.empty()) {
        Move opponentMove = board->undoMoveHistory.top();
        for (auto nextMove : allMoves->nextMoves) {
            if (nextMove->mv->getStartCoord() == opponentMove.getStartCoord() && nextMove->mv->getEndCoord() == opponentMove.getEndCoord() 
            && nextMove->mv->getPromotePiece() == opponentMove.getPromotePiece()) {
                allMoves = nextMove;
            }
        }
    }
    // run the move algorithm and store the best move (score, move) pair
    pair<int, Move *> bestMove = negamax(allMoves, -1000000000, 1000000000, 3, color);

    // locate the move the algorithm picked (bestMove.second) and set it as the new head of allMoves
    // a move is the same if it has the same start & end coords and same promotion piece if applicable
    for (auto nextMove : allMoves->nextMoves) {
        if (nextMove->mv->getStartCoord() == bestMove.second->getStartCoord() && nextMove->mv->getEndCoord() == bestMove.second->getEndCoord() 
        && nextMove->mv->getPromotePiece() == bestMove.second->getPromotePiece()) {
            allMoves = nextMove;
        }
    }
    return *(bestMove.second);
}
