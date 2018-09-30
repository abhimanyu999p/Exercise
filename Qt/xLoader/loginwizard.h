#ifndef LOGINWIZARD_H
#define LOGINWIZARD_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class loginWizard;
}

class loginWizard : public QDialog
{
    Q_OBJECT

public:
    explicit loginWizard(QWidget *parent = 0);
    ~loginWizard();

    bool loginOk;

private slots:
    void on_pushButton_Enter_clicked(void);
    void on_loginTimeout(void);

private:
    Ui::loginWizard *ui;

    QString psd;
    QString psdEntered;
    QTimer myTimer;

};

#endif // LOGINWIZARD_H
