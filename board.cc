#include "board.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "graphics/textdisplay.h"
#include "graphics/graphicsdisplay.h"
#include "observer.h"
using namespace std;

void printMovee(Move *mv) {
    cout << "[Move] " << mv->getPiece()->getPieceType() << ": " << (char)(mv->getStartCoord().first + 97) << (mv->getStartCoord().second + 1) << " -> " << (char)(mv->getEndCoord().first + 97) << (mv->getEndCoord().second + 1);
    if (mv->getCapturedPiece() != nullptr) {
        cout << " Captures " << mv->getCapturedPiece()->getPieceType();
    }
    cout << endl;
}

Board::Board(int startTurn) {
    for(int i = 0; i < 8; i++) {
        vector<Square> row;
        for(int j = 0; j < 8; j++) {
            Square thisSquare{i, j};
            row.push_back(thisSquare);
        }
        board.push_back(row);
    }
    turn = startTurn;
}

void Board::addPieces(const vector<vector<char>> state) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(state[i][j] != '_' && state[i][j] != ' ') {
                int color = state[i][j] >= 'a' && state[i][j] <= 'z'; //one for black, zero for white
                if(state[i][j] == 'p' || state[i][j] == 'P') {
                    board[i][j].piece = new Pawn{this, color, i, j};
                } else if(state[i][j] == 'n' || state[i][j] == 'N') {
                    board[i][j].piece = new Knight{this, color, i, j};
                } else if(state[i][j] == 'b' || state[i][j] == 'B') {
                    board[i][j].piece = new Bishop{this, color, i, j};
                } else if(state[i][j] == 'r' || state[i][j] == 'R') {
                    board[i][j].piece = new Rook{this, color, i, j};
                } else if(state[i][j] == 'q' || state[i][j] == 'Q') {
                    board[i][j].piece = new Queen{this, color, i, j};
                } else if(state[i][j] == 'k' || state[i][j] == 'K') {
                    board[i][j].piece = new King{this, color, i, j};
                }
                board[i][j].piece->board = this;
                attach(board[i][j].piece);
            }
        }
    }
    td = new TextDisplay{*this};
    gd = new GraphicsDisplay;
    attach(td);
    attach(gd);
    notifyObservers();
}

// assumes sc and ec are on the board (otherwise segfaults)
bool Board::movePiece(pair<int, int> sc, pair<int, int> ec, bool secondTime, bool checking) {
    if(board[sc.first][sc.second].piece == nullptr) return false;
    if(board[sc.first][sc.second].piece->movePiece(sc, ec)) { 
        Piece *capturedPiece = nullptr;
        bool enPassant = false;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                board[i][j].removeAttacker();
            }
        }
        if(board[ec.first][ec.second].piece != nullptr) {
            capturedPiece = board[ec.first][ec.second].piece->clone(); // captured piece
            detach(board[ec.first][ec.second].piece);
            delete board[ec.first][ec.second].piece;
            board[ec.first][ec.second].piece = nullptr;
        }
        board[ec.first][ec.second].piece = board[sc.first][sc.second].piece->clone(); //moves piece
        attach(board[ec.first][ec.second].piece);
        if(board[sc.first][sc.second].piece->getPieceType() == 'k' && abs(ec.first - sc.first) == 2) { 
            //castle, move rook
            if(ec.first < sc.first) {
                board[2][ec.second].piece = board[0][ec.second].piece->clone();
                attach(board[2][ec.second].piece);
                board[2][ec.second].piece->canCastle = false;
                detach(board[0][ec.second].piece);
                delete board[0][ec.second].piece;
                board[0][ec.second].piece = nullptr;
            } else {
                board[5][ec.second].piece = board[7][ec.second].piece->clone(); //new rook
                attach(board[5][ec.second].piece);
                board[5][ec.second].piece->canCastle = false;
                detach(board[7][ec.second].piece);
                delete board[7][ec.second].piece;
                board[7][ec.second].piece = nullptr;

            }
        } else if(board[sc.first][sc.second].piece->getPieceType() == 'p') { 
            if((ec.second == 0 || ec.second == 7) && !checking) { 
                //promote
                string promotePiece;
                while(1) {
                    cout << "Enter a piece you would like to promote to [r/n/b/q]\n";
                    getline(cin, promotePiece);
                    if(promotePiece != "r" && promotePiece != "n" && promotePiece != "b" && promotePiece != "q") {
                        cout << "Please enter a valid piece\n";
                    } else {
                        break;
                    }
                }
                int color = board[ec.first][ec.second].piece->getColor();
                detach(board[ec.first][ec.second].piece);
                delete board[ec.first][ec.second].piece;
                board[ec.first][ec.second].piece = nullptr;
                if(promotePiece == "r") {
                    board[ec.first][ec.second].piece = new Rook{this, color, ec.first, ec.second};
                } else if(promotePiece == "n") {
                    board[ec.first][ec.second].piece = new Knight{this, color, ec.first, ec.second};
                } else if(promotePiece == "b") {
                    board[ec.first][ec.second].piece = new Bishop{this, color, ec.first, ec.second};
                } else {
                    board[ec.first][ec.second].piece = new Queen{this, color, ec.first, ec.second};
                }
                board[ec.first][ec.second].piece->board = this;
                attach(board[ec.first][ec.second].piece);
            } else if(ec.first != sc.first && board[ec.first][sc.second].piece != nullptr && board[ec.first][sc.second].piece->getPieceType() == 'p' && board[ec.first][sc.second].piece->canEnPassant) {
                //en passant
                capturedPiece = board[ec.first][sc.second].piece->clone(); 
                enPassant = true;
                detach(board[ec.first][sc.second].piece);
                delete board[ec.first][sc.second].piece;
                board[ec.first][sc.second].piece = nullptr;
                board[ec.first][sc.second].piece = nullptr;
            } 
        } 
        Move mv{board[sc.first][sc.second].piece->clone(), capturedPiece, sc, ec, enPassant}; 
        detach(board[sc.first][sc.second].piece);
        delete board[sc.first][sc.second].piece; //delete piece
        board[sc.first][sc.second].piece = nullptr;
        undoMoveHistory.push(mv);
        notifyObservers();
        if(!secondTime) {
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    if(board[i][j].piece != nullptr && board[i][j].piece->getColor() == turn && board[i][j].piece->getPieceType() == 'k') {
                        if(board[i][j].isAttacked(!turn)) {
                            undoMove();
                            return false;
                        }
                    }
                }
            }
        }
        while(redoMoveHistory.size()) {
            redoMoveHistory.pop();
        }
        return true;
    }
    return false;
}

bool Board::undoMove() {
    if(!undoMoveHistory.size()) return false;
    Move mv = undoMoveHistory.top();
    undoMoveHistory.pop();
    if(mv.getPiece()->getPieceType() == 'k' && abs(mv.getStartCoord().first - mv.getEndCoord().first) > 1) { // castle
        if(mv.getStartCoord().first > mv.getEndCoord().first) {
            board[3][mv.getStartCoord().second].piece = board[1][mv.getStartCoord().second].piece->clone();
            attach(board[3][mv.getStartCoord().second].piece);
            board[3][mv.getStartCoord().second].piece->canCastle = true;
            board[3][mv.getStartCoord().second].piece->coord = {3, mv.getStartCoord().second};
            detach(board[1][mv.getStartCoord().second].piece);
            delete board[1][mv.getStartCoord().second].piece;
            board[1][mv.getStartCoord().second].piece = nullptr;
            board[0][mv.getStartCoord().second].piece = board[2][mv.getStartCoord().second].piece->clone();
            attach(board[0][mv.getStartCoord().second].piece);
            board[0][mv.getStartCoord().second].piece->canCastle = true;
            board[0][mv.getStartCoord().second].piece->coord = {0, mv.getStartCoord().second};
            detach(board[2][mv.getStartCoord().second].piece);
            delete board[2][mv.getStartCoord().second].piece;
            board[2][mv.getStartCoord().second].piece = nullptr;
        } else {
            board[4][mv.getStartCoord().second].piece = board[6][mv.getStartCoord().second].piece->clone();
            attach(board[4][mv.getStartCoord().second].piece);
            board[4][mv.getStartCoord().second].piece->canCastle = true;
            board[4][mv.getStartCoord().second].piece->coord = {4, mv.getStartCoord().second};
            detach(board[6][mv.getStartCoord().second].piece);
            delete board[6][mv.getStartCoord().second].piece;
            board[6][mv.getStartCoord().second].piece = nullptr;
            board[7][mv.getStartCoord().second].piece = board[5][mv.getStartCoord().second].piece->clone();
            attach(board[7][mv.getStartCoord().second].piece);
            board[7][mv.getStartCoord().second].piece->canCastle = true;
            board[7][mv.getStartCoord().second].piece->coord = {7, mv.getStartCoord().second};
            detach(board[5][mv.getStartCoord().second].piece);
            delete board[5][mv.getStartCoord().second].piece;
            board[5][mv.getStartCoord().second].piece = nullptr;
        }
    } else if(mv.getPiece()->getPieceType() == 'k') {
        board[mv.getStartCoord().first][mv.getStartCoord().second].piece = mv.getPiece()->clone();
        attach(board[mv.getStartCoord().first][mv.getStartCoord().second].piece);
        board[mv.getStartCoord().first][mv.getStartCoord().second].piece->coord = mv.getStartCoord();
        detach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
        delete board[mv.getEndCoord().first][mv.getEndCoord().second].piece;
        board[mv.getEndCoord().first][mv.getEndCoord().second].piece = nullptr;
        if(mv.getCapturedPiece() != nullptr) {
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece = mv.getCapturedPiece()->clone();
            attach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece->coord = mv.getEndCoord();
        }
    } else if(mv.getPiece()->getPieceType() == 'p') {
        if(mv.isEnPassant()) {
            board[mv.getStartCoord().first][mv.getStartCoord().second].piece = mv.getPiece()->clone();
            attach(board[mv.getStartCoord().first][mv.getStartCoord().second].piece);
            board[mv.getStartCoord().first][mv.getStartCoord().second].piece->coord = {mv.getStartCoord().first, mv.getStartCoord().second};
            detach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
            delete board[mv.getEndCoord().first][mv.getEndCoord().second].piece;
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece = nullptr;
            board[mv.getEndCoord().first][mv.getStartCoord().second].piece = mv.getCapturedPiece()->clone();
            attach(board[mv.getEndCoord().first][mv.getStartCoord().second].piece);
            board[mv.getEndCoord().first][mv.getStartCoord().second].piece->coord = {mv.getEndCoord().first, mv.getStartCoord().second};
        } else {
            board[mv.getStartCoord().first][mv.getStartCoord().second].piece = mv.getPiece()->clone();
            attach(board[mv.getStartCoord().first][mv.getStartCoord().second].piece);
            board[mv.getStartCoord().first][mv.getStartCoord().second].piece->coord = mv.getStartCoord();
            detach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
            delete board[mv.getEndCoord().first][mv.getEndCoord().second].piece;
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece = nullptr;
            if(mv.getCapturedPiece() != nullptr) {
                board[mv.getEndCoord().first][mv.getEndCoord().second].piece = mv.getCapturedPiece()->clone();
                attach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
                board[mv.getEndCoord().first][mv.getEndCoord().second].piece->coord = mv.getEndCoord();
            }
            if(abs(mv.getEndCoord().second - mv.getStartCoord().second) == 2) {
                board[mv.getStartCoord().first][mv.getStartCoord().second].piece->firstMove = true;
            }
        }
    } else {
        board[mv.getStartCoord().first][mv.getStartCoord().second].piece = mv.getPiece()->clone();
        attach(board[mv.getStartCoord().first][mv.getStartCoord().second].piece);
        board[mv.getStartCoord().first][mv.getStartCoord().second].piece->coord = mv.getStartCoord();
        detach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
        delete board[mv.getEndCoord().first][mv.getEndCoord().second].piece;
        board[mv.getEndCoord().first][mv.getEndCoord().second].piece = nullptr;
        if(mv.getCapturedPiece() != nullptr) {
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece = mv.getCapturedPiece()->clone();
            attach(board[mv.getEndCoord().first][mv.getEndCoord().second].piece);
            board[mv.getEndCoord().first][mv.getEndCoord().second].piece->coord = mv.getEndCoord();
        }
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j].removeAttacker();
        }
    }
    notifyObservers();
    redoMoveHistory.push(mv);
    return true;
}

bool Board::redoMove() {
    if(!redoMoveHistory.size()) return false;
    Move mv = redoMoveHistory.top();
    redoMoveHistory.pop();
    movePiece(mv.getStartCoord(), mv.getEndCoord());
    undoMoveHistory.push(mv);
    return true;
}

bool Board::isFinished() {
    //check if no move is possible
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board[i][j].piece == nullptr || board[i][j].piece->getColor() != turn) continue;
            for(int x = 0; x < 8; x++) {
                for(int y = 0; y < 8; y++) {
                    Move mv{board[i][j].piece, nullptr, {i, j}, {x, y}, false};
                    if(movePiece({i, j}, {x, y}, false, true)) {
                        undoMove();
                        return false;
                    }
                }
            }
        }
    }
    return true;
    return false;
}

const vector<vector<Square>>& Board::getTheBoard() const {
    return board;
}

void Board::printBoard() {
    notifyGraphics();
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j].removeAttacker();
            if(board[i][j].piece != nullptr && board[i][j].piece->canEnPassant) {
                if(!undoMoveHistory.size() || undoMoveHistory.top().getEndCoord() != (pair<int,int>){i, j}) {
                    board[i][j].piece->canEnPassant = false;
                }
            }
        }
    }
    notifyObservers();
    cout << *td << "\n";
}
