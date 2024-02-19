#include "graphicsdisplay.h"
using namespace std;

GraphicsDisplay::GraphicsDisplay() {
    gridSize = 500;
    cellSize = 500 / 9;
    xw = new Xwindow();
    for (int y = 0; y < 8; y++) {
        xw->drawString(10, (y + 1) * cellSize, to_string(8 - y));
        for (int x = 0; x < 8; x++) {
            unsigned long color;
            if ((x + y) % 2 == 0) {
                color = Xwindow::White;
            } else {
                color = Xwindow::Green;
            }
            xw->fillRectangle((x + 1) * cellSize, y * cellSize, cellSize, cellSize, color);
        }
    }
    for (int x = 0; x < 8; x++) {
        xw->drawString((x + 1) * cellSize, 9 * cellSize, string(1, 'a' + x));
    }
    graphics = true;
}

int GraphicsDisplay::notify(Subject &c) {
    vector<vector<Square>> theBoard = c.getTheBoard();
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            unsigned long color;
            if ((x + y )% 2 == 0) {
                color = Xwindow::White;
            } else {
                color = Xwindow::Green;
            }
            xw->fillRectangle((x + 1) * cellSize, y * cellSize, cellSize, cellSize, color);
            if(theBoard[x][7 - y].piece != nullptr) {
                char pieceType = theBoard[x][7 - y].piece->getPieceType();
                if(theBoard[x][7 - y].piece->getColor() == 0) {
                    pieceType = toupper(pieceType);
                }
                string pc = "";
                pc += pieceType;
                xw->drawString((x + 1) * cellSize + cellSize / 2, (y + 1) * cellSize - cellSize / 2, pc);   
            } 
        }
    }
    return 0;
}

GraphicsDisplay::~GraphicsDisplay() {
    delete xw;
}
