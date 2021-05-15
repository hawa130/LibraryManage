#include "selectdialog.h"
#include "ui_selectdialog.h"

#include "bookinfodialog.h"
#include "userinfodialog.h"

SelectDialog::SelectDialog(QWidget *parent, int _bookID, int _userID) :
    QDialog(parent),
    ui(new Ui::SelectDialog) {
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(0, INT_MAX, this));

    bookModel = new QStandardItemModel();
    userModel = new QStandardItemModel();

    if (_bookID != -1) {
        ui->lineEdit->setPlaceholderText(tr("用户编号..."));
        this->setWindowTitle(tr("选择用户"));
        displayUserData();
    } else if (_userID != -1) {
        ui->lineEdit->setPlaceholderText(tr("图书编号..."));
        this->setWindowTitle(tr("选择图书"));
        displayBookData();
    } else {

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
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(1, 125);
    ui->tableView->setColumnWidth(2, 75);
    ui->tableView->setColumnWidth(3, 75);
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

int SelectDialog::getSelection() {
    int curRow = ui->tableView->currentIndex().row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

int SelectDialog::getSelection(const QModelIndex &index) {
    int curRow = index.row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

void SelectDialog::on_tableView_clicked(const QModelIndex &index) {
    ui->lineEdit->setText(QString::number(getSelection(index)));
}

void SelectDialog::on_buttonBox_accepted() {
    emit sendData(ui->lineEdit->text());
    accept();
}

void SelectDialog::on_tableView_doubleClicked() {
    emit sendData(ui->lineEdit->text());
    accept();
}
