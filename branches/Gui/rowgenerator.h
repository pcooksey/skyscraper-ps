#ifndef ROWGENERATOR_H
#define ROWGENERATOR_H

#include <QDialog>
#include <QTreeWidget>
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
    
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

private:
    Ui::RowGenerator *ui;
    QTreeWidgetItem *root;
    list<vector<SkyScraper> > possible;


    void generateRows();
    void generateTree();
};

#endif // ROWGENERATOR_H
