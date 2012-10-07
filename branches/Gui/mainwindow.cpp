#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    puzzle(NULL), pSolver(NULL), progress(NULL), watcher(NULL), signalMapper(NULL),
    size(0)
{
    ui->setupUi(this);
    this->setWindowTitle("Skyscraper-ps");

    QGridLayout *layout = ui->puzzle;
    QLCDNumber *entry = new QLCDNumber;
    entry->setDigitCount(3);
    entry->display(".o0");
    entry->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(entry,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(puzzle)
        delete puzzle;
    if(pSolver)
        delete pSolver;
    if(progress)
        delete progress;
    if(watcher)
        delete watcher;
    if(signalMapper)
        delete signalMapper;
}

bool MainWindow::createPuzzle()
{
    QGridLayout *layout = ui->puzzle;
    Puzzle::Group_pairs col;
    Puzzle::Group_pairs row;
    for(int i=0; i<size; i++)
    {
        int top, bottom, right, left;
        top = ((LCDEntry*)layout->itemAtPosition(0,i+1)->widget())->intValue();
        bottom = ((LCDEntry*)layout->itemAtPosition(size+1,i+1)->widget())->intValue();
        right = ((LCDEntry*)layout->itemAtPosition(i+1,0)->widget())->intValue();
        left = ((LCDEntry*)layout->itemAtPosition(i+1,size+1)->widget())->intValue();
        col.push_back(std::make_pair(top,bottom));
        row.push_back(std::make_pair(right,left));
    }
    setPuzzle(new Puzzle(col,row));

    for(int row=0; row<size; ++row)
    {
        for(int col=0; col<size; ++col)
        {
            int value = ((LCDEntry*)layout->itemAtPosition(row+1,col+1)->widget())->intValue();
            if(value!=0)
            {
                try
                {
                    puzzle->set(row,col,value);
                } catch(bool& err) {
                    warning("Failed","Puzzle is incorrect");
                    return false;
                }
            }
        }
    }
    return true;
}

void MainWindow::setPuzzle(Puzzle* puzzle)
{
    deletePuzzle();
    this->puzzle = puzzle;
}

void MainWindow::solve()
{
    if(puzzle!=NULL)
    {
        if(createPuzzle())
        {
            try
            {
                puzzle->solve();
                printPuzzle();
                if(puzzle->complete() && puzzle->correct())
                {
                    if(puzzle->correct())
                    {
                        information("Solved","Completely Solved!");
                    } else {
                        warning("Error","Puzzle not solved correctly");
                    }
                } else {
                    information("Partially Solved","Try the partial solver to complete the puzzle");
                    ui->partialSolver->setEnabled(true);
                    ui->actionPartial_Solver->setEnabled(true);
                }
            } catch (bool &err) {
                warning("Error","Puzzle cannot be solved!");
            }
        }
    }
    else {
        warning("Warning","No puzzle loaded!");
    }
}

void MainWindow::partialSolver()
{
    if(puzzle!=NULL)
    {
        if(pSolver!=NULL)
            delete pSolver;
        pSolver = new PartialSolver(*puzzle);
        if(watcher==NULL)
        {
            watcher = new QFutureWatcher<void>();
            connect(watcher, SIGNAL(finished()), this, SLOT(finishedPartialSolver()));
        }
        QFuture<void> future = QtConcurrent::run(pSolver, &PartialSolver::solve);
        watcher->setFuture(future);
        if(progress==NULL)
        {
            progress = new QProgressDialog("Solving puzzle...", "Cancel", 0, 0, this);
            progress->setWindowModality(Qt::WindowModal);
            progress->setCancelButton(0);
        }
        progress->show();
    }
}

void MainWindow::finishedPartialSolver()
{
    progress->cancel();
    const list<Puzzle>& solved = pSolver->solvedPuzzles();
    if(solved.size()==1)
    {
        setPuzzle(new Puzzle(solved.front()));
        printPuzzle();
        information("Completed","Finished solving the puzzle!");
    }
    else {
        information("Completed", QString("Finished with multiple solutions! (%1)").arg(solved.size()));
        pSolver->boundaryFinder();
    }
}

void MainWindow::printPuzzle()
{
    checkForDeletion();
    displayConstraints();
    displayEntries();
    displayNewConstraints();
}

void MainWindow::checkForDeletion()
{
    if(size != puzzle->size())
    {
        QLayout *layout = ui->puzzle;
        QLayoutItem *child;
        while ( (child = layout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    size = puzzle->size();
}

void MainWindow::displayConstraints()
{
    Puzzle::Group_pairs col = puzzle->columnPairs();
    Puzzle::Group_pairs row = puzzle->rowPairs();

    signalMapper = new QSignalMapper(this);
    for(int i=0; i<size; i++)
    {
        LCDEntry* num;
        QPalette *lcdpalette = new QPalette;
        lcdpalette->setColor(QPalette::Background, QColor(211, 211, 211));
        num = addLCD(col.front().first,0,i+1);
        num->setPalette(*lcdpalette);
        num->setAutoFillBackground(true);
        //Adds signal
        signalMapper->disconnect(num, 0, 0, 0);
        connect(num, SIGNAL(doubleclickedevent()), signalMapper, SLOT(map()));
        signalMapper->setMapping(num, i+1);
        num = addLCD(col.front().second,size+1,i+1);
        num->setPalette(*lcdpalette);
        num->setAutoFillBackground(true);
        //Adds signal
        signalMapper->disconnect(num, 0, 0, 0);
        connect(num, SIGNAL(doubleclickedevent()), signalMapper, SLOT(map()));
        signalMapper->setMapping(num, i+1);
        num = addLCD(row.front().first,i+1,0);
        num->setPalette(*lcdpalette);
        num->setAutoFillBackground(true);
        //Adds signal
        signalMapper->disconnect(num, 0, 0, 0);
        connect(num, SIGNAL(doubleclickedevent()), signalMapper, SLOT(map()));
        signalMapper->setMapping(num, -(i+1));
        num = addLCD(row.front().second,i+1,size+1);
        num->setPalette(*lcdpalette);
        num->setAutoFillBackground(true);
        //Adds signal
        signalMapper->disconnect(num, 0, 0, 0);
        connect(num, SIGNAL(doubleclickedevent()), signalMapper, SLOT(map()));
        signalMapper->setMapping(num, -(i+1));
        col.erase(col.begin());
        row.erase(row.begin());
    }
    connect(signalMapper, SIGNAL(mapped(int)),
                 this, SLOT(generateRows(int)));
}

void MainWindow::displayEntries()
{
    for(int row=0; row<size; ++row)
    {
        for(int col=0; col<size; ++col)
        {
            addLCD(puzzle->entry(row,col),row+1,col+1);
        }
    }
}

void MainWindow::displayNewConstraints()
{
    list<int> numbers;
    for(int x=0; x<size; x++)
    {
        for(int y=0; y<size; y++)
        {
            numbers.push_back(puzzle->entry(y,x));
        }
        int colUp = visableScore(numbers);
        changeLCD(colUp,0,x+1);
        numbers.reverse();
        int colDown = visableScore(numbers);
        changeLCD(colDown,size+1,x+1);
        numbers.clear();
        for(int y=0; y<size; y++)
        {
            numbers.push_back(puzzle->entry(x,y));
        }
        int rowLeft = visableScore(numbers);
        changeLCD(rowLeft,x+1,0);
        numbers.reverse();
        int rowRight = visableScore(numbers);
        changeLCD(rowRight,x+1,size+1);
        numbers.clear();
    }
}

LCDEntry* MainWindow::addLCD(int num, int row, int col)
{
    QGridLayout *layout = ui->puzzle;
    if(layout->itemAtPosition(row,col))
    {
        LCDEntry *item = (LCDEntry*)layout->itemAtPosition(row,col)->widget();
        item->display(num);
        QPalette p = item->palette();
        p.setColor(QPalette::WindowText, QColor(0,0,0));
        item->setPalette(p);
        return item;
    }
    LCDEntry *entry = new LCDEntry(size, num);
    layout->addWidget(entry,row,col);
    return entry;
}

LCDEntry* MainWindow::changeLCD(int num, int row, int col)
{
    QGridLayout *layout = ui->puzzle;
    LCDEntry *item = (LCDEntry*)layout->itemAtPosition(row,col)->widget();
    if(num!=0 && item->intValue()==0)
    {
        item->display(num);
        QPalette p = item->palette();
        p.setColor(QPalette::WindowText, QColor(255,0,0));
        item->setPalette(p);
    }
    return item;
}

void MainWindow::deletePuzzle()
{
    if(puzzle!=NULL)
    {
        delete puzzle;
        ui->partialSolver->setEnabled(false);
        ui->actionPartial_Solver->setEnabled(false);
    }
}

int MainWindow::visableScore(list<int>& objects)
{
    list<int>::iterator start = objects.begin(),
                                 end = objects.end();
    int value(0), score(0);
    for(; start!=end; start++)
    {
        if((*start)==0)
            return 0;
        else if((*start)>value)
        {
            value = (*start);
            score++;
        }
        if(value==size)
        {
            return score;
        }
    }
    /// Should reach here only if not complete vector
    return 0;
}

void MainWindow::warning(QString title, QString message)
{
    QMessageBox messageBox;
    messageBox.warning(0,title,message);
    messageBox.setFixedSize(500,200);
}

void MainWindow::information(QString title, QString message)
{
    QMessageBox messageBox;
    messageBox.information(0,title,message);
    messageBox.setFixedSize(500,200);
}

void MainWindow::on_actionOpen_File_triggered()
{
    OpenFile getFile(this);
    getFile.setModal(true);
    getFile.exec();
    QString path = getFile.getPath();
    if(path!="")
    {
        /** create puzzle with the file name here */
        setPuzzle(new Puzzle());
        puzzle->loadFile(path.toStdString());
        printPuzzle();
    }
}

void MainWindow::on_actionSolve_triggered()
{
    solve();
}

void MainWindow::on_solve_clicked()
{
    solve();
}

void MainWindow::on_partialSolver_clicked()
{
    partialSolver();
}

void MainWindow::on_actionPartial_Solver_triggered()
{
    partialSolver();
}

void MainWindow::on_actionCreate_Puzzle_triggered()
{
    bool ok;
    int num = QInputDialog::getInt(this, "Puzzle Size",
                          tr("Number:"), 4, 2, 100, 1, &ok);
    if (ok)
    {
        deletePuzzle();
        puzzle = new Puzzle(num);
        printPuzzle();
    }
}

void MainWindow::on_actionRow_Genetor_triggered()
{
    bool ok;
    if(puzzle!=NULL)
    {
        int num = QInputDialog::getInt(this, "Pick a number",
                              tr("Column positive(1) | Row Negative(-1):"), 1, -size, size, 1, &ok);
        if(ok && num!=0)
        {
            bool column = true;
            if(num<0)
            {
                column = false;
                num = -num;
            }
            list<vector<SkyScraper> > possible = puzzle->generatorRows(num-1, column);
            RowGenerator window(possible,this);
            window.setModal(true);
            window.exec();
        }
    }
    else {
        warning("Warning","No puzzle loaded!");
    }
}

void MainWindow::generateRows(int num)
{
    if(num!=0)
    {
        bool column = true;
        if(num<0)
        {
            column = false;
            num = -num;
        }
        list<vector<SkyScraper> > possible = puzzle->generatorRows(num-1, column);
        RowGenerator window(possible,this);
        window.setModal(true);
        window.exec();
    }
}
