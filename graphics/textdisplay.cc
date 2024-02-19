#include "textdisplay.h"
using namespace std;

TextDisplay::TextDisplay(Board b) {
    for (int i = 0; i < 8; i++) {
        vector<char> row;
        for (int j = 0; j < 8; j++) {
            if (b.board[i][j].piece == nullptr) {
                row.push_back((i + j) % 2 ? '_' : ' ');
            } else {
                if (b.board[i][j].piece->getColor() == 0) {
                    row.push_back(toupper(b.board[i][j].piece->getPieceType()));
                } else {
                    row.push_back(b.board[i][j].piece->getPieceType());
                }
            }
        }
        theDisplay.push_back(row);
    }
}

int TextDisplay::notify(Subject &c) {
    vector<vector<Square>> theBoard = c.getTheBoard();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (theBoard[i][j].piece == nullptr) {
                theDisplay[i][j] = ((i + j) % 2 ? '_' : ' ');
            } else {
                if (theBoard[i][j].piece->getColor() == 0) {
                    theDisplay[i][j] = toupper(theBoard[i][j].piece->getPieceType());
                } else {
                    theDisplay[i][j] = theBoard[i][j].piece->getPieceType();
                }
            }
        }
    }
    return 0;
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
    for (int y = 7; y >= 0; y--) {
        out << y + 1 << " ";
        for (int x = 0; x < 8; x++) {
            out << td.theDisplay[x][y];
        }
        out << "\n";
    }
    out << "  ";
    for (int x = 0; x < 8; x++) {
        out << string(1, 'a' + x);
    }
    out << "\n";
    return out;
}
