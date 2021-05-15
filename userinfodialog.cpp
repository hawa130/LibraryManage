#include "userinfodialog.h"
#include "ui_userinfodialog.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog) {
    ui->setupUi(this);
}

UserInfoDialog::~UserInfoDialog() {
    delete ui;
}
