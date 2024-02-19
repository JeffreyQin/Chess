#ifndef __SQUARE_H__
#define __SQUARE_H__
#include "observer.h"
#include "piece.h"
#include <utility>

class Square: public Observer {
    std::pair<int, int> coord;
    std::vector<int> numAttackers;

    public:
        Piece *piece;

        Square(int x, int y);
        int notify(Subject &s) override;
        pair<int, int> getCoord();
        void addAttacker(int index);
        void removeAttacker();
        bool isAttacked(int index);
};

#endif
