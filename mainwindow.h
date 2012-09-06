#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "openfile.h"
#include "lcdentry.h"
#include "Skyscraper/Puzzle.hh"
#include "Skyscraper/PartialSolver.hh"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionOpen_File_triggered();

    void on_actionSolve_triggered();

    void on_solve_clicked();

    void on_partialSolver_clicked();

    void on_actionPartial_Solver_triggered();

    void on_actionCreate_Puzzle_triggered();

    void finishedPartialSolver();

private:
    Ui::MainWindow *ui;
    Puzzle *puzzle;
    PartialSolver *pSolver;
    QProgressDialog *progress;
    QFutureWatcher<void> *watcher;
    int size;

    bool createPuzzle();
    void setPuzzle(Puzzle* puzzle);

    void solve();
    void partialSolver();
    //Checking if the puzzle size is the same otherwise
    //If not we have to delete all of the lcdnumbers
    void printPuzzle();
    void checkForDeletion();
    void displayConstraints();
    void displayEntries();
    void displayNewConstraints();
    LCDEntry* addLCD(int num, int row, int col);
    LCDEntry* changeLCD(int num, int row, int col);
    void deletePuzzle();

    int visableScore(list<int>& objects);

    //Popups
    void warning(QString title, QString message);
    void information(QString title, QString message);
};

#endif // MAINWINDOW_H
