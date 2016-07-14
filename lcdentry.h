#ifndef LCDENTRY_H
#define LCDENTRY_H

#include <QtGui>

class LCDEntry : public QLCDNumber
{
    Q_OBJECT
    public:
        LCDEntry(int size, int num);

        void display(int num);

    Q_SIGNALS:
        void doubleclickedevent();

    private:
        int size;
        int num;
        bool locked;
        QColor original;

        void lockedDisplay(int num);

        void mouseDoubleClickEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent* event);
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
};

#endif // LCDENTRY_H
