#ifndef LIBRARYMAIN_H
#define LIBRARYMAIN_H

#include "librarydata.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class LibraryMain;
}
QT_END_NAMESPACE

class LibraryMain : public QMainWindow {
    Q_OBJECT

public:
    LibraryMain(QWidget *parent = nullptr);
    ~LibraryMain();

    virtual void closeEvent(QCloseEvent*);

private slots:

    void on_bookSwitchButton_clicked();

    void on_userSwitchButton_clicked();

    void on_searchButton_clicked();

    void on_borrowButton_clicked();

    void on_returnButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_editButton_clicked();

    void on_tableView_doubleClicked();

    void on_writeDataAction_triggered();

    void on_addButton_clicked();

    void on_aboutMeAction_triggered();

    void on_signOutAction_triggered();

    void on_importAction_triggered();

    void on_exportAction_triggered();

    void on_readDataAction_triggered();

    void on_aboutAction_triggered();

private:
    Ui::LibraryMain *ui;
    QStandardItemModel* userModel;
    QStandardItemModel* bookModel;

    void initBookTable();

    void initUserTable();

    void displayBookData();

    void displayUserData();

    void displayBookList(List<Node<BookInfo>*>);

    void displayUserList(List<Node<UserInfo>*>);

    void appendSingleBook(Node<BookInfo>*);

    void appendSingleUser(Node<UserInfo>*);

    void displaySingleBook(Node<BookInfo>*);

    void displaySingleUser(Node<UserInfo>*);

    int getSelection();

    int getSelection(const QModelIndex&);

    void updateButton(int, int);

    void disableButton();
};
#endif // LIBRARYMAIN_H
