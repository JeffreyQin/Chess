#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include "window.h"
#include "../observer.h"
#include "../board.h"
#include <vector>

using namespace std;

class GraphicsDisplay: public Observer {
    Xwindow *xw; //the graphics window
    int gridSize;
    int cellSize;
    public:
        

public:
    GraphicsDisplay();
    int notify(Subject &c) override;
    ~GraphicsDisplay();
};

#endif
