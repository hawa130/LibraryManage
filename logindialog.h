#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <librarydata.h>
#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::LoginDialog *ui;
    string username;
    string password;

    void readSavedData();

    bool isSaved();
};

#endif // LOGINDIALOG_H
