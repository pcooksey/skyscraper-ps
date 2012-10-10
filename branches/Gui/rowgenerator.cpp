#include "rowgenerator.h"
#include "ui_rowgenerator.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

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

    //ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setColumnCount(2);
    if(QTreeWidgetItem* header = ui->treeWidget->headerItem()) {
      header->setText(0,"Type");
      header->setText(1,"Size");
    }
    root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,"All");
    root->setText(1, QString::number(possible.size()));
    ui->treeWidget->addTopLevelItem(root);
    ui->treeWidget->expandAll();

    generateTree();
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

void RowGenerator::generateTree()
{
    rowIterator begin = possible.begin(), end = possible.end();
    int typenum(0), count(0);
    while(begin!=end)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        root->addChild(item);
        typenum = (*begin)[0];
        item->setText(0,QString::number(typenum));
        while((*begin)[0]==typenum)
        {
            count++;
            begin++;
        }
        item->setText(1,QString::number(count));
        count = 0;
    }
}

void RowGenerator::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    int number = item->text(0).toInt();
    QListWidget* listwidget = ui->listWidget;
    listwidget->clear();
    QString numbers = "";
    rowIterator begin = possible.begin(), end = possible.end();
    for(; begin!=end; begin++)
    {
        //If number is zero then it mean they selected all.
        if(!number || (*begin)[0]==number)
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

}

void RowGenerator::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Rows",
                                                    QApplication::applicationFilePath(),
                                                    "Text files (*.txt);; CVS files (*.csv)");
    QFile file( filename );
    if(filename!="" && file.open( QIODevice::WriteOnly ))
    {
        QTextStream out(&file);
        rowIterator begin = possible.begin(), end = possible.end();
        for(; begin!=end; begin++)
        {
            for(unsigned int i=0; i<begin->size(); i++)
            {
                int num = (*begin)[i];
                out<< QString::number(num);
            }
            out<<"\n";
        }
        file.close();
        QMessageBox messageBox;
        messageBox.information(0,"Saved Filed","The file has been saved successfully!");
        messageBox.setFixedSize(500,200);

    } else {
        return;
    }
}
