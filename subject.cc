#include <algorithm>
#include <iostream>
#include <cassert>
#include "subject.h"
#include "observer.h"
#include "square.h"
using namespace std;

Subject::Subject() {}

void Subject::attach(Observer *ob) {
    obs.emplace_back(ob);
}

void Subject::detach(Observer *ob) {
    auto target = find(obs.begin(), obs.end(), ob);
    if (target != obs.end()) {
        obs.erase(target);
    }
}

void Subject::notifyObservers() {
    for (auto &ob: obs) {
        assert(ob != nullptr);
        if(!ob->graphics) ob->notify(*this);
    }
}

void Subject::notifyGraphics() {
    for(auto &ob: obs) {
        if(ob->graphics) ob->notify(*this);
    }
}
