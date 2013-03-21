#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->localhostRadioButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//private slot
void MainWindow::on_runButton_clicked(bool checked)
{
    if (checked)
    {
        QHostAddress address;
        if (this->ui->allInterfacesRadioButton->isChecked())
            address = QHostAddress::Any;
        else if (this->ui->localhostRadioButton->isChecked())
            address = QHostAddress::LocalHost;

        const quint16 port = this->ui->listenPortSpinbox->value();

        _server = QSharedPointer<SocksServer>(new SocksServer(address,
                                                              port,
                                                              this->ui->throttleSpinBox->value() * 1024));
        _server->start();
        if (!_server->isStarted())
        {
            QMessageBox::warning(this, "Error", "Failed to start server");
            QTimer::singleShot(1, this->ui->runButton, SLOT(click()));
        }
    }
    else
        _server = QSharedPointer<SocksServer>();

    this->ui->localhostRadioButton->setDisabled(checked);
    this->ui->allInterfacesRadioButton->setDisabled(checked);
    this->ui->listenPortSpinbox->setDisabled(checked);
    this->ui->throttleSpinBox->setDisabled(checked);
}
