#ifndef LCDENTRY_H
#define LCDENTRY_H

#include <QtGui>

class LCDEntry : public QLCDNumber
{
    public:
        LCDEntry(int size, int num);

        void display(int num);
    private:
        int size;
        int num;
        bool locked;

        void lockedDisplay(int num);

        void mousePressEvent(QMouseEvent*);
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
};

#endif // LCDENTRY_H
