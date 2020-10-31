#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    searchWind = new SearchWindow();
    setWindowTitle("[*]Text Editor");
    setWindowIcon(QIcon(":/resource/img/filenew.png"));

    textEdit = new QTextEdit();

    setCentralWidget(textEdit);

    createActions();
    createMenu();

    createStatusBar();
    readSettings();

    connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    newAction = new QAction(QIcon(":/resource/img/filenew.png"),tr("&New"),this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new file!"));
    connect(newAction, SIGNAL(triggered()),this, SLOT(newFile()));

    openAction = new QAction(QIcon(":/resource/img/fileopen.png"), tr("&Open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an existing file!"));
    connect(openAction, SIGNAL(triggered()),this, SLOT(openFile()));

    saveAction = new QAction(QIcon(":/resource/img/filesave.png"), tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save the document to disk!"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As"), this);
    saveAsAction->setStatusTip(tr("Save the document under a new name!"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(QIcon(":/resource/img/fileexit"), tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+E"));
    exitAction->setStatusTip(tr("Exit the application!"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(QIcon(":/resource/img/editcut.png"), tr("Cu&t"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard!"));
    connect(cutAction, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAction = new QAction(QIcon(":/resource/img/editcopy.png"), tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard!"));
    connect(copyAction, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAction = new QAction(QIcon(":/resource/img/editpaste.png"), tr("&Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection!"));
    connect(pasteAction, SIGNAL(triggered()), textEdit, SLOT(paste()));

    searchAction = new QAction(QIcon(":/resource/img/search.png"), tr("&Search"), this);
    searchAction->setShortcut(tr("Ctrl+H"));
    searchAction->setStatusTip(tr("Search single letter or expression"));
    connect(searchAction, SIGNAL(triggered()), this, SLOT(openSearchWindow()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box!"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box!"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    copyAction->setEnabled(false);
    cutAction->setEnabled(false);

    connect(textEdit, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(searchAction);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
        event->accept();
}

void MainWindow::openSearchWindow()
{
    searchWind->show();
    connect(searchWind->searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
    connect(searchWind->undoButton, SIGNAL(clicked()), this, SLOT(undoButtonClicked()));
}

void MainWindow::searchButtonClicked()
{
    if(!searchWind->lineEdit->isModified())
        QMessageBox::warning(this, "Warning", "Please enter a letter or expression to search!");
    else
    {
        searchText = searchWind->lineEdit->text();
        findSearchText();
    }
}

void MainWindow::undoButtonClicked()
{
    textEdit->setTextColor(Qt::black);
    textEdit->clear();
    textEdit->setText(mainText);
}

void MainWindow::setTextEdit()
{
    text = textEdit->toPlainText();
    mainText = text;
}

void MainWindow::findSearchText()
{
    setTextEdit();

    currentString = text;
    //searchText = "e";
    //addString = "WOW";

    textEdit->clear();

    while(!currentString.isEmpty())
    {
        position = currentString.toStdString().find(searchText.toStdString());

        if(position > - 1)
        {
            cur.append(currentString.left(position));
            currentString = currentString.right(currentString.size() - searchText.size() - position);

            textEdit->setTextColor(Qt::black);
            textEdit->insertPlainText(cur);
            textEdit->setTextColor(Qt::red);
            textEdit->insertPlainText(searchText);
            cur.remove(0, cur.size());
        }
        else
        {
            textEdit->setTextColor(Qt::black);
            currentString = currentString.right(currentString.size() + searchText.size() - position);
            textEdit->insertPlainText(currentString);
            currentString.remove(0, currentString.size());
        }
   }
    /*
        while(!currentString.isEmpty())
        {
            position = currentString.toStdString().find(searchText.toStdString());

            if (position > -1)
            {
                copyString.append(currentString.left(position));
                copyString.append(addString);
                currentString = currentString.right(currentString.size() - searchText.size() - position);
            }
            else
            {
                copyString.append(currentString.right(currentString.size() - searchText.size() - position + 1));
                currentString.remove(0, currentString.size());
            }
        }

        text = copyString;
        textEdit->setText(text);
    */
}

void MainWindow::createStatusBar()
{    
    statusBar()->showMessage("Ready!");
}

void MainWindow::newFile()
{
    if(maybeSave())
    {
        textEdit->clear();
        setCurrentFile("Untitled.txt");
    }
}

void MainWindow::openFile()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if(fileName.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(fileName);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return saveFile(fileName);
    }
}

bool MainWindow::maybeSave()
{
   if(textEdit->document()->isModified())
   {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Application"),
                                  tr("The document has been modified.\n"
                                     "Do you want to save the changes?"),
                                  QMessageBox::Save|QMessageBox::Discard|QMessageBox::Ok|QMessageBox::Cancel);

   if(ret == QMessageBox::Save)
       return save();
   else if(ret == QMessageBox::Cancel)
       return false;
   }
   return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cant read the file %1:\n %2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded!"), 2000);
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

bool MainWindow::saveFile(const QString &fileName)
{

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly|QFile::ReadOnly))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Can't read the file %1:\n %2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }


    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved!"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &curFileName)
{
    fileName = curFileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if(fileName.isEmpty())
    {
        shownName = tr("Untitled.txt");
    }
    else
        shownName = strippedName(fileName);
    setWindowTitle(tr("%1[*] - %2").arg(tr("Application")));
}

void MainWindow::readSettings()
{
    QSettings settings("QT5 Cookbook","Application");
    QPoint pos = settings.value("pos", QPoint(150,100)).toPoint();
    QSize size = settings.value("size", QSize(500, 350)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("QT5 Cookbook", "Application");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::about()
{   
    QMessageBox::about(this, tr("About"), tr("Welcome to QT! This is 'Notepad' GUI application"
                                             " with a menu bar and a status bar!"));
}
