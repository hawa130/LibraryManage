#include "bookinfodialog.h"
#include "ui_bookinfodialog.h"
#include "selectdialog.h"
#include "userinfodialog.h"

#include <QMessageBox>
#include <QIntValidator>

BookInfoDialog::BookInfoDialog(QWidget *parent, int _bookID) :
    QDialog(parent),
    ui(new Ui::BookInfoDialog) {
    ui->setupUi(this);

    userModel = new QStandardItemModel();

    ui->idEdit->setValidator(new QIntValidator(0, INT_MAX, this));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    book = lib.findBook(_bookID);
    if (book) {
        displayTable();
        ui->nameEdit->setText(tr(book->elem.name.data()));
        ui->idEdit->setText(QString::number(book->elem.identifier));
        ui->numEdit->setValue(book->elem.quantity);
    } else {
        ui->deleteButton->setDisabled(true);
        ui->borrowButton->setDisabled(true);
        ui->returnButton->setDisabled(true);
        ui->borrowThisButton->setDisabled(true);
        ui->returnThisButton->setDisabled(true);
    }

    if (loginUserID == -1) {
        ui->borrowThisButton->setHidden(true);
        ui->returnThisButton->setHidden(true);
        return;
    }

    if (!isLoginAdmin) {
        ui->borrowButton->setHidden(true);
        ui->returnButton->setHidden(true);
        ui->deleteButton->setHidden(true);
        ui->buttonBox->setHidden(true);
        ui->nameEdit->setDisabled(true);
        ui->idEdit->setDisabled(true);
        ui->numEdit->setDisabled(true);
    }
}

BookInfoDialog::~BookInfoDialog() {
    delete ui;
}

void BookInfoDialog::receiveData(QString data) {
    auto user = lib.findUser(data.toInt());
    lib.borrowBook(user, book);
    displayTable();
}

void BookInfoDialog::initUserTable() {
    disableButton();
    userModel->clear();
    userModel->setColumnCount(3);
    userModel->setHeaderData(0, Qt::Horizontal, tr("用户名"));
    userModel->setHeaderData(1, Qt::Horizontal, tr("编号"));
    userModel->setHeaderData(2, Qt::Horizontal, tr("已借阅总数"));
    ui->tableView->setModel(userModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 75);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

void BookInfoDialog::displayTable() {
    initUserTable();
    ui->numLabel->setText(tr("共借出 ") + QString::number(book->elem.readers.size()) + tr(" 本"));
    for (auto p = book->elem.readers.begin(); p != book->elem.readers.end(); p = p->next) {
        appendSingleUser(p->elem);
    }
}

void BookInfoDialog::appendSingleUser(Node<UserInfo>* p) {
    if (!p) return;
    QList<QStandardItem*> list;
    list << new QStandardItem(p->elem.name.data())
         << new QStandardItem(std::to_string(p->elem.identifier).data())
         << new QStandardItem(std::to_string(p->elem.books.size()).data());
    userModel->appendRow(list);
}

void BookInfoDialog::disableButton() {
    ui->returnButton->setDisabled(true);

    if (book->elem.quantity <= book->elem.readers.size()) {
        ui->borrowButton->setDisabled(true);
        ui->borrowThisButton->setDisabled(true);
    } else {
        ui->borrowButton->setDisabled(false);
        ui->borrowThisButton->setDisabled(false);
    }

    if (loginUserID == -1) {
        return;
    }
    if (lib.findUser(loginUserID)->elem.books.find(book)) {
        ui->returnThisButton->setDisabled(false);
    } else {
        ui->returnThisButton->setDisabled(true);
    }
}

void BookInfoDialog::updateButton(int bookID, int userID) {
    auto book = lib.findBook(bookID);
    if (book->elem.quantity == book->elem.readers.size()) {
        ui->borrowButton->setDisabled(true);
    } else {
        ui->borrowButton->setDisabled(false);
    }

    if (lib.findUser(userID)->elem.books.find(book)) {
        ui->returnButton->setDisabled(false);
    } else {
        ui->returnButton->setDisabled(true);
    }
}

void BookInfoDialog::on_returnButton_clicked() {
    if (!book) return;
    int userID = getSelection();
    lib.returnBook(userID, book->elem.identifier);
    displayTable();
}

int BookInfoDialog::getSelection() {
    int curRow = ui->tableView->currentIndex().row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

int BookInfoDialog::getSelection(const QModelIndex &index) {
    int curRow = index.row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

void BookInfoDialog::updateBookInfo() {
    string name = ui->nameEdit->text().toStdString();
    int    id   = ui->idEdit->text().toInt();
    int    num  = ui->numEdit->value();
    if (book) {
        book->elem.name = name;
        book->elem.identifier = id;
        book->elem.quantity   = num;
    } else {
        book = lib.add(BookInfo(name, id, num));
    }
}

void BookInfoDialog::on_buttonBox_accepted() {
    if (checkBookInfo()) {
        updateBookInfo();
        accept();
    }
}

void BookInfoDialog::on_deleteButton_clicked() {
    if (!book) return;
    if (QMessageBox::warning(this, tr("确认删除"), tr("要永久删除此图书吗？此操作不可撤销。"),
                             QMessageBox::Ok | QMessageBox::Cancel)
            == QMessageBox::Cancel) {
        return;
    }
    if (!lib.del(book)) {
        if (QMessageBox::warning(this, tr("警告"),
                             tr("还有 ") + QString::number(book->elem.readers.size())
                             + tr(" 本书仍处于借出状态。要强制删除吗？"),
                             QMessageBox::Ok | QMessageBox::Cancel)
                == QMessageBox::Cancel) {
            return;
        }
        lib.del(book, true);
    }
    accept();
}

void BookInfoDialog::on_numEdit_valueChanged() {
    if (!book) return;
    int readersNum = book->elem.readers.size();
    ui->numEdit->setMinimum(readersNum);
}

void BookInfoDialog::on_tableView_clicked(const QModelIndex &index) {
    int userID = getSelection(index);
    updateButton(book->elem.identifier, userID);
}

bool BookInfoDialog::checkBookInfo() {
    int id = ui->idEdit->text().toInt();
    auto findRes = lib.findBook(id);
    if (findRes && findRes != book) {
        QMessageBox::warning(this, tr("重复的编号"), tr("图书编号不能重复。"), QMessageBox::Ok);
        return false;
    }
    return id >= 0;
}

void BookInfoDialog::on_borrowButton_clicked() {
    SelectDialog selectDlg(this, book->elem.identifier, -1);
    connect(&selectDlg, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
    selectDlg.exec();
}

void BookInfoDialog::on_tableView_doubleClicked(const QModelIndex &index) {
    int userID = getSelection(index);
    UserInfoDialog userDialog(this, userID);
    userDialog.exec();
    displayTable();
}

void BookInfoDialog::on_borrowThisButton_clicked() {
    auto user = lib.findUser(loginUserID);
    lib.borrowBook(user, book);
    displayTable();
}

void BookInfoDialog::on_returnThisButton_clicked() {
    auto user = lib.findUser(loginUserID);
    lib.returnBook(user, book);
    displayTable();
}

