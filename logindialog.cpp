#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    if (isSaved()) {
        ui->checkBox->setChecked(true);
        readSavedData();
    }
}

LoginDialog::~LoginDialog() {
    delete ui;
}

bool LoginDialog::isSaved() {
    ifstream input("saved");
    return input ? true : false;
}

void LoginDialog::readSavedData() {
    if (!ui->checkBox->isChecked()) return;
    ifstream input("saved");
    if (!input) return;
    getline(input, username);
    getline(input, password);
    ui->unameEdit->setText(username.data());
    ui->pwdEdit->setText(password.data());
}

void LoginDialog::on_buttonBox_accepted() {
    username = ui->unameEdit->text().toStdString();
    password = ui->pwdEdit->text().toStdString();
    if (lib.login(username, password)) {
        if (ui->checkBox->isChecked()) {
            ofstream output("saved");
            output << username << endl << password << endl;
        }
        loginUserID = lib.findUser(ui->unameEdit->text().toStdString())->elem.identifier;
        accept();
    } else {
        QMessageBox::warning(this, tr("提示"), tr("用户名或密码错误。"), QMessageBox::Ok);
    }
}

