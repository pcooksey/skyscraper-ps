#include "rowgenerator.h"
#include "ui_rowgenerator.h"

RowGenerator::RowGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RowGenerator)
{
    ui->setupUi(this);
}

RowGenerator::RowGenerator(list<vector<SkyScraper> >& possible, QWidget *parent)
    : QDialog(parent), ui(new Ui::RowGenerator)
{
    ui->setupUi(this);
    this->possible = possible;

    ui->treeWidget->setHeaderHidden(true);
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,"All");
    ui->treeWidget->addTopLevelItem(item);
    generateRows();
}

RowGenerator::~RowGenerator()
{
    delete ui;
}

void RowGenerator::generateRows()
{
    QListWidget* listwidget = ui->listWidget;
    QString numbers = "";
    rowIterator begin = possible.begin(), end = possible.end();
    for(; begin!=end; begin++)
    {
        numbers = "";
        for(unsigned int i=0; i<begin->size(); i++)
        {
            int num = (*begin)[i];
            numbers.append(QString::number(num) + " ");
        }
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(numbers);
        newItem->setFont (QFont ("Courier", 20));
        listwidget->addItem(newItem);
    }
}
