
#include "searchwindow.h"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resource/img/search.png"));
    setWindowTitle(tr("Search"));

    verticalLayout = new QVBoxLayout();
    horizontalLayout = new QHBoxLayout();
    label = new QLabel();
    searchButton = new QPushButton();
    undoButton = new QPushButton();
    lineEdit = new QLineEdit();

    label->setText("Enter a letter or expression to search!");
    label->setMargin(100);

    searchButton->setText("Search");
    undoButton->setText("Undo");
    verticalLayout->addWidget(label);
    verticalLayout->addWidget(lineEdit);
    horizontalLayout->addWidget(searchButton);
    horizontalLayout->addWidget(undoButton);
    verticalLayout->addLayout(horizontalLayout);
    setLayout(verticalLayout);
}

SearchWindow::~SearchWindow()
{
    delete ui;
}
