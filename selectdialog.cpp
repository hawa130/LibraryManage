#include "selectdialog.h"
#include "ui_selectdialog.h"

#include "bookinfodialog.h"
#include "userinfodialog.h"

SelectDialog::SelectDialog(QWidget *parent, int _bookID, int _userID) :
    QDialog(parent),
    ui(new Ui::SelectDialog) {
    ui->setupUi(this);
    if (_bookID != -1) {
        book = lib.findBook(_bookID);
        user = nullptr;
        displayBookData();
    } else if (_userID != -1) {
        user = lib.findUser(_userID);
        book = nullptr;
        displayUserData();
    }
}

SelectDialog::~SelectDialog() {
    delete ui;
}

void SelectDialog::initBookTable() {
    bookModel->clear();
    bookModel->setColumnCount(4);
    bookModel->setHeaderData(0, Qt::Horizontal, tr("名称"));
    bookModel->setHeaderData(1, Qt::Horizontal, tr("编号"));
    bookModel->setHeaderData(2, Qt::Horizontal, tr("总数量"));
    bookModel->setHeaderData(3, Qt::Horizontal, tr("剩余数量"));
    ui->tableView->setModel(bookModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    if (this->size().width() >= 300) {
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        ui->tableView->setColumnWidth(1, 125);
        ui->tableView->setColumnWidth(2, 75);
        ui->tableView->setColumnWidth(3, 75);
    }
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

void SelectDialog::initUserTable() {
    userModel->clear();
    userModel->setColumnCount(3);
    userModel->setHeaderData(0, Qt::Horizontal, tr("用户名"));
    userModel->setHeaderData(1, Qt::Horizontal, tr("编号"));
    userModel->setHeaderData(2, Qt::Horizontal, tr("已借阅数量"));
    ui->tableView->setModel(userModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 75);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

void SelectDialog::displayBookData() {
    initBookTable();
    for (auto p = lib.books.begin(); p != lib.books.end(); p = p->next) {
        appendSingleBook(p);
    }
}

void SelectDialog::displayUserData() {
    initUserTable();
    for (auto p = lib.users.begin(); p != lib.users.end(); p = p->next) {
        appendSingleUser(p);
    }
}

void SelectDialog::appendSingleBook(Node<BookInfo>* p) {
    if (!p) return;
    QList<QStandardItem*> list;
    list << new QStandardItem(p->elem.name.data())
         << new QStandardItem(std::to_string(p->elem.identifier).data())
         << new QStandardItem(std::to_string(p->elem.quantity).data())
         << new QStandardItem(std::to_string(p->elem.quantity - p->elem.readers.size()).data());
    bookModel->appendRow(list);
}

void SelectDialog::appendSingleUser(Node<UserInfo>* p) {
    if (!p) return;
    QList<QStandardItem*> list;
    list << new QStandardItem(p->elem.name.data())
         << new QStandardItem(std::to_string(p->elem.identifier).data())
         << new QStandardItem(std::to_string(p->elem.books.size()).data());
    userModel->appendRow(list);
}
