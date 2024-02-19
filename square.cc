#include "square.h"
using namespace std;

Square::Square(int x, int y): coord{x, y} {
    piece = nullptr;
    numAttackers = {0, 0};
}

int Square::notify(Subject &s) {
    // note when moving piece: DO NOT delete piece, rather just move it
    return 0;
}

pair<int, int> Square::getCoord() {
    return coord;
}

void Square::addAttacker(int index) {
    numAttackers[index]++;
}

void Square::removeAttacker() {
    numAttackers = {0, 0};
}

bool Square::isAttacked(int index) {
    return numAttackers[index] > 0;
}
