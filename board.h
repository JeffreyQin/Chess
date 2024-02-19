#ifndef __BOARD_H__
#define __BOARD_H__
#include "move.h"
#include "square.h"
#include <stack>
#include <vector>
using namespace std;

class Observer;
class TextDisplay;
class GraphicsDisplay;

class Board: Subject {
    stack<Move> redoMoveHistory;
    GraphicsDisplay *gd;
    TextDisplay *td;
    std::vector<Observer *> obs;

    public:
        stack<Move> undoMoveHistory;
        vector<vector<Square>> board;
        int turn;
        bool inCheck;
        Board(int startTurn = 0);
        bool movePiece(pair<int, int> sc, pair<int, int> ec, bool secondTime = false, bool checking = false);
        bool undoMove();
        bool redoMove();
        bool isFinished();
        const vector<vector<Square>>& getTheBoard() const override;
        void addPieces(const vector<vector<char>> state = {
            {'R', 'P', ' ', '_', ' ', '_', 'p', 'r'}, 
            {'N', 'P', '_', ' ', '_', ' ', 'p', 'n'},
            {'B', 'P', ' ', '_', ' ', '_', 'p', 'b'},
            {'Q', 'P', '_', ' ', '_', ' ', 'p', 'q'},
            {'K', 'P', ' ', '_', ' ', '_', 'p', 'k'},
            {'B', 'P', '_', ' ', '_', ' ', 'p', 'b'},
            {'N', 'P', ' ', '_', ' ', '_', 'p', 'n'},
            {'R', 'P', '_', ' ', '_', ' ', 'p', 'r'} 
        });
        void printBoard();
};

#endif
