#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include "librarydata.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = nullptr, int _userID = -1);
    ~UserInfoDialog();

private slots:
    void receiveBookData(QString data);

    void receivePwdData(QString data);

    void on_returnButton_clicked();

    void on_buttonBox_accepted();

    void on_deleteButton_clicked();

    void on_tableView_clicked();

    void on_borrowButton_clicked();

    void on_pwdButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::UserInfoDialog *ui;
    QStandardItemModel* bookModel;
    Node<UserInfo>* user;

    void initBookTable();

    void displayTable();

    void displayBookList(List<Node<BookInfo>*>);

    void appendSingleBook(Node<BookInfo>*);

    int getSelection();

    int getSelection(const QModelIndex&);

    void updateUserInfo();

    bool checkUserInfo();
};

#endif // USERINFODIALOG_H
