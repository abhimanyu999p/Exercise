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

#include "loginwizard.h"
#include "ui_loginwizard.h"
#include<iostream>

using namespace std;

loginWizard::loginWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginWizard)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(267,177));
    psd = "abracadabra";
    loginOk = false;

    connect(&myTimer,SIGNAL(timeout()),this,SLOT(on_loginTimeout()));
}

loginWizard::~loginWizard()
{
    delete ui;
}

void loginWizard::on_pushButton_Enter_clicked()
{
    psdEntered = ui->lineEdit_password->text();

    if(psd == psdEntered)
    {
        ui->label_2->setText("Welcome Home!");
        loginOk = true;
    }
    else
    {
        ui->label_2->setText("Access Denied");
    }

    myTimer.setSingleShot(true);
    myTimer.start(1000);
}

void loginWizard::on_loginTimeout()
{
    close();
}
