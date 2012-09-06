#include "lcdentry.h"

LCDEntry::LCDEntry(int size, int num)
    :original(0,0,0)
{
    this->size = size;
    int length = QString::number(size).length();
    display(num);
    setSegmentStyle(QLCDNumber::Flat);
    setNumDigits(length);
}

void LCDEntry::display(int num)
{
    this->num = num;
    this->QLCDNumber::display(num);
    if(num!=0)
        locked = true;
    else
        locked = false;
}
void LCDEntry::lockedDisplay(int num)
{
    if(locked)
        return;
    this->num = num;
    this->QLCDNumber::display(num);
}

void LCDEntry::mousePressEvent(QMouseEvent *)
{
    if(locked)
        return;
    int value = intValue();
    if(value+1>size)
        lockedDisplay(0);
    else
        lockedDisplay(value+1);
}

void LCDEntry::enterEvent(QEvent*)
{
    QPalette p = this->palette();
    original = p.color(QPalette::WindowText);
    p.setColor(QPalette::WindowText, QColor(255,0,0));
    setPalette(p);
}

void LCDEntry::leaveEvent(QEvent*)
{
    QPalette p = this->palette();
    p.setColor(QPalette::WindowText, original);
    setPalette(p);
}
