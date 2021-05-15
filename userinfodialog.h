#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>

namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
    ~UserInfoDialog();

private:
    Ui::UserInfoDialog *ui;
};

#endif // USERINFODIALOG_H
