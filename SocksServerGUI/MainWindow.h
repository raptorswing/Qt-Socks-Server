#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "SocksServer.h"

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

    void on_runButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSharedPointer<SocksServer> _server;
};

#endif // MAINWINDOW_H
