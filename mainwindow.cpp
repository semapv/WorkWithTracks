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
        QString info = xml.outMsg();
        ui->GpxInfo->setText(info);

        QString statistic = QString("Расстояние:\t %1 м \n").arg(Geo::distance(xml.treckData()));
        statistic += QString("Ср. скорость:\t %1 км/ч \n").arg(Geo::avrSpeed_kmh(xml.treckData()));
        statistic += QString("Ср. скорость:\t %1 м/c  \n").arg(Geo::avrSpeed_ms(xml.treckData()));
        statistic += QString("Ср. темп:\t %1 мин/км \n").arg(Geo::avrPace(xml.treckData()));

        ui->GpxStatistic->setText(statistic);

    }
}
