#ifndef __SUBJECT_H__
#define __SUBJECT_H__
#include <vector>

class Observer;
class Square;

class Subject {
    protected:
        std::vector<Observer *> obs;

    public:
        Subject();
        void attach(Observer *ob);
        void detach(Observer *ob);
        void notifyObservers();
        void notifyGraphics();
        virtual const std::vector<std::vector<Square>>& getTheBoard() const = 0;
};

#endif
