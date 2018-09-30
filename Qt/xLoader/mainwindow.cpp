/*
 * This file is a part of xLoader.
 *
 * xLoader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<iostream>
#include<QtCore>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setFixedSize(QSize(576,358));

    m = new modem(this);
    ui->progressBar->reset();
    ui->progressBar->hide();
    ui->lineEdit_baudrate->setText("115200");
    ui->lineEdit_comport->setText("/dev/ttyUSB0");

    connect(m,SIGNAL(updateProgressBar(void)),this,SLOT(on_updateProgressBar(void)));
    connect(m,SIGNAL(updateConsole(QString)),this,SLOT(on_updateConsole(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString str1 = QFileDialog::getOpenFileName(
                this,
                tr("select binary"),
                "/home/abhimanyu999p/","All(*.*);;bin(*.bin*);;hex(*.hex*)");
    ui->lineEdit_binpath->setText(str1);

}

void MainWindow::on_flashButton_clicked()
{
    ui->label_console->setText(" ");
    ui->progressBar->reset();

    QString str1 = ui->lineEdit_binpath->text();
    QByteArray ba = str1.toLatin1();
    m->filePath = ba.data();

    str1 = ui->lineEdit_comport->text();
    ba = str1.toLatin1();
    const char* port = ba.data();

    int baudRate = ui->lineEdit_baudrate->text().toInt();
    bool stopBit = ui->checkBox_Stopbit->isChecked();
    bool parity = ui->checkBox_noParity->isChecked();

    if(0 <= m->sModemOpen(port, baudRate, stopBit, parity))
    {
        on_updateConsole("Modem Opened");
        ui->progressBar->show();
        progressBarStep = m->sGetTotalBlocks();
        m->sflash();
    }
    ui->progressBar->hide();

}

void MainWindow::on_updateProgressBar()
{
  static int update = 0;

  update++;
  ui->progressBar->setValue((update/progressBarStep)*100);

}

void MainWindow::on_updateConsole(QString msg)
{
    ui->label_console->setText(msg);
}
