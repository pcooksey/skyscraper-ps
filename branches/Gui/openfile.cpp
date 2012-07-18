#include "openfile.h"
#include "ui_openfile.h"
#include <QFileSystemModel>

OpenFile::OpenFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenFile), path("")
{
    ui->setupUi(this);

    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->setRootPath(QApplication::applicationFilePath()));

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QStringList filters;
    filters << "*.txt";
    filemodel->setNameFilters(filters);
    filemodel->setNameFilterDisables(false);
    ui->listView->setModel(filemodel);
}

OpenFile::~OpenFile()
{
    delete ui;
}

void OpenFile::on_treeView_clicked(const QModelIndex &index)
{
    QString tempPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(tempPath));
}

void OpenFile::on_listView_clicked(const QModelIndex &index)
{
    path = filemodel->fileInfo(index).absoluteFilePath();
}
