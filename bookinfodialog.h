#ifndef BOOKINFODIALOG_H
#define BOOKINFODIALOG_H

#include "librarydata.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class BookInfoDialog;
}

class BookInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookInfoDialog(QWidget *parent = nullptr, int _bookID = -1);
    ~BookInfoDialog();

private slots:
    void receiveData(QString data);

    void on_returnButton_clicked();

    void on_buttonBox_accepted();

    void on_deleteButton_clicked();

    void on_numEdit_valueChanged();

    void on_tableView_clicked(const QModelIndex &index);

    void on_borrowButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_borrowThisButton_clicked();

    void on_returnThisButton_clicked();

private:
    Ui::BookInfoDialog *ui;
    QStandardItemModel* userModel;
    Node<BookInfo>* book;

    void initUserTable();

    void displayTable();

    void disableButton();

    void displayUserList(List<Node<UserInfo>*>);

    void appendSingleUser(Node<UserInfo>*);

    void updateButton(int, int);

    int getSelection();

    int getSelection(const QModelIndex&);

    void updateBookInfo();

    bool checkBookInfo();
};

#endif // BOOKINFODIALOG_H
