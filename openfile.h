#ifndef OPENFILE_H
#define OPENFILE_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class OpenFile;
}

class OpenFile : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenFile(QWidget *parent = 0);
    ~OpenFile();
    QString getPath() { return path; }
    
private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::OpenFile *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QString path;
};

#endif // OPENFILE_H
