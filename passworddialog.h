#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "librarydata.h"
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr, Node<UserInfo> *_user = nullptr);
    ~PasswordDialog();

signals:
    void sendPwdData(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PasswordDialog *ui;
    Node<UserInfo>* user;

    bool checkPassword();
};

#endif // PASSWORDDIALOG_H
