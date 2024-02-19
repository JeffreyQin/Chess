#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "../observer.h"
#include "../board.h"
using namespace std;

class TextDisplay: public Observer {
    vector<vector<char>> theDisplay;

    public:
        TextDisplay(Board b);
        int notify(Subject &c) override;

        friend ostream &operator<<(ostream &out, const TextDisplay &td);
};      
#endif
