#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlparse.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    GpxParse xml("files/treck.gpx");
    if(!xml.outMsg().isEmpty()){
        QString msg = xml.outMsg();
        ui->textBrowser->setText(msg);
        QString dist = QString("%1").arg(Geo::distance(xml.treckData()));
        ui->label->setText(dist);

    }
}
