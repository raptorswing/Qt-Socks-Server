#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SocksServer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SocksServer * server = new SocksServer(this);
    server->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
