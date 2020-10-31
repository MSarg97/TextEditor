#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include "searchwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent*);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();
    void about();
    void openSearchWindow();
    void findSearchText();
    void documentWasModified();
    void searchButtonClicked();
    void undoButtonClicked();

private:
    Ui::MainWindow *ui;

    void createMenu();
    void createActions();
    void createSearch();
    void createStatusBar();
    bool maybeSave();
    void writeSettings();
    void readSettings();
    void loadFile(const QString &);
    bool saveFile(const QString &);
    void setCurrentFile(const QString &);
    void setTextEdit();
    QString strippedName(const QString &);

    QString searchText;
    QString text;
    QString currentString;
    QString copyString;
    QString addString;    
    QString cur;
    QString mainText;
    QString fileName;

    int position;
    QMessageBox *messageBox;
    QTextEdit *textEdit;
    QTextEdit *searchTextEdit;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *searchAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;

    QAction *aboutAction;
    QAction *aboutQtAction;

    SearchWindow *searchWind;    
};
#endif // MAINWINDOW_H
