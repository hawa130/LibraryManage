#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "userinfodialog.h"

PasswordDialog::PasswordDialog(QWidget *parent, Node<UserInfo> *_user) :
    QDialog(parent),
    ui(new Ui::PasswordDialog),
    user(_user) {
    ui->setupUi(this);

    if (isLoginAdmin) {
        ui->oldPwdEdit->setHidden(true);
        ui->oldPwdLabel->setHidden(true);
    }
}

PasswordDialog::~PasswordDialog() {
    delete ui;
}

bool PasswordDialog::checkPassword() {
    string  realOldPwd  = user->elem.password;
    QString inputOldPwd = ui->oldPwdEdit->text();
    return realOldPwd == inputOldPwd.toStdString();
}

void PasswordDialog::on_buttonBox_accepted() {
    if (!isLoginAdmin) {
        if (checkPassword()) {
            emit sendPwdData(ui->newPwdEdit->text());
        } else {
            QMessageBox::information(this, tr("信息"), tr("请输入正确的旧密码。<br>若忘记请联系管理员修改密码。"), QMessageBox::Ok);
            return;
        }
    } else {
        emit sendPwdData(ui->newPwdEdit->text());
    }
    accept();
}

