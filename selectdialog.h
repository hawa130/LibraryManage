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

signals:
    void sendData(QString);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_tableView_doubleClicked();

private:
    Ui::SelectDialog *ui;
    QStandardItemModel* userModel;
    QStandardItemModel* bookModel;

    void initBookTable();

    void initUserTable();

    void displayBookData();

    void displayUserData();

    void appendSingleBook(Node<BookInfo>*);

    void appendSingleUser(Node<UserInfo>*);

    int getSelection();

    int getSelection(const QModelIndex&);

};

#endif // SELECTDIALOG_H
