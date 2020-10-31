#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);

    ~SearchWindow();

public:
    QString getSearchExp();
    QPushButton *searchButton;
    QPushButton *undoButton;
    QLineEdit *lineEdit;

private:
    Ui::SearchWindow *ui;
    QLabel *label;

    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
};

#endif // SEARCHWINDOW_H
