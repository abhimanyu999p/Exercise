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
#include "loginwizard.h"
#include <QApplication>
#include <QDesktopWidget>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    loginWizard l;

    l.setModal(true);
    l.exec();
    if(l.loginOk)
    {
        w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
        w.show();
        return app.exec();
    }
    else
    {
        return 0;
    }

}
