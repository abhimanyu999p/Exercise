#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    modem *m;

    void vCloseNExit();

public slots:

    void on_updateProgressBar(void);

    void on_updateConsole(QString);

private slots:

    void on_browseButton_clicked();

    void on_flashButton_clicked();

private:
    Ui::MainWindow *ui;
    int progressBarStep;
};

#endif // MAINWINDOW_H
