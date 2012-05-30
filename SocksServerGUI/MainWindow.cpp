#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server = new SocksServer(this);

    _server->start();

    if (!_server->isStarted())
    {
        qWarning() << "Failed to start server.";
        _server->deleteLater();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
