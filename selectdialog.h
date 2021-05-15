#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include "librarydata.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class SelectDialog;
}

class SelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDialog(QWidget *parent = nullptr, int _bookID = -1, int _userID = -1);
    ~SelectDialog();

private:
    Ui::SelectDialog *ui;
    Node<BookInfo>* book;
    Node<UserInfo>* user;
    QStandardItemModel* userModel;
    QStandardItemModel* bookModel;

    void initBookTable();

    void initUserTable();

    void displayBookData();

    void displayUserData();

    void appendSingleBook(Node<BookInfo>*);

    void appendSingleUser(Node<UserInfo>*);

};

#endif // SELECTDIALOG_H
