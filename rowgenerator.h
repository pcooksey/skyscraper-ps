#ifndef ROWGENERATOR_H
#define ROWGENERATOR_H

#include <QDialog>
#include "Skyscraper/SkyScraper.hh"

namespace Ui {
class RowGenerator;
}

class RowGenerator : public QDialog
{
    Q_OBJECT
    typedef list<vector<SkyScraper> >::iterator rowIterator;
    
public:
    explicit RowGenerator(QWidget *parent = 0);
    RowGenerator(list<vector<SkyScraper> >& possible, QWidget *parent = 0);
    ~RowGenerator();
    
private:
    Ui::RowGenerator *ui;
    list<vector<SkyScraper> > possible;


    void generateRows();
};

#endif // ROWGENERATOR_H
