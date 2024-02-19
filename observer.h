#ifndef __OBSERVER_H__
#define __OBSERVER_H__
#include "subject.h"
using namespace std;

class Observer {
    public:
        bool graphics = false;
        virtual int notify(Subject &s) = 0;
};

#endif
