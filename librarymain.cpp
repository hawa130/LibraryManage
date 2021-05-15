#include "librarymain.h"
#include "ui_librarymain.h"
#include "bookinfodialog.h"
#include "userinfodialog.h"

#include <QTableView>
#include <QMessageBox>

LibraryMain::LibraryMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LibraryMain) {
    ui->setupUi(this);
    ui->pageUpButton->setHidden(true);
    ui->pageDnButton->setHidden(true);
    ui->bookSwitchButton->setDisabled(true);

    if (!lib.isAdmin(lib.findUser(loginUserID))) {
        isLoginAdmin = false;
        ui->addButton->setHidden(true);
    } else {
        isLoginAdmin = true;
    }

    bookModel = new QStandardItemModel();
    userModel = new QStandardItemModel();

    displayBookData();
}

LibraryMain::~LibraryMain() {
    delete ui;
}

void LibraryMain::initBookTable() {
    disableButton();
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

void LibraryMain::initUserTable() {
    disableButton();
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

void LibraryMain::displayBookData() {
    initBookTable();
    for (auto p = lib.books.begin(); p != lib.books.end(); p = p->next) {
        appendSingleBook(p);
    }
}

void LibraryMain::displayUserData() {
    initUserTable();
    for (auto p = lib.users.begin(); p != lib.users.end(); p = p->next) {
        appendSingleUser(p);
    }
}

void LibraryMain::on_bookSwitchButton_clicked() {
    ui->bookSwitchButton->setDisabled(true);
    ui->bookSwitchButton->setChecked(true);
    ui->userSwitchButton->setDisabled(false);
    ui->userSwitchButton->setChecked(false);
    ui->borrowButton->setDisabled(true);
    ui->returnButton->setDisabled(true);
    ui->editButton->setDisabled(true);
    ui->searchBox->setPlaceholderText(tr("搜索图书..."));
    ui->searchButton->click();
}

void LibraryMain::on_userSwitchButton_clicked() {
    ui->userSwitchButton->setDisabled(true);
    ui->userSwitchButton->setChecked(true);
    ui->bookSwitchButton->setDisabled(false);
    ui->bookSwitchButton->setChecked(false);
    ui->borrowButton->setDisabled(true);
    ui->returnButton->setDisabled(true);
    ui->editButton->setDisabled(true);
    ui->searchBox->setPlaceholderText(tr("搜索用户..."));
    ui->searchButton->click();
}

void LibraryMain::displayBookList(List<Node<BookInfo>*> list) {
    initBookTable();
    for (auto p = list.begin(); p != list.end(); p = p->next) {
        appendSingleBook(p->elem);
    }
}

void LibraryMain::displayUserList(List<Node<UserInfo>*> list) {
    initUserTable();
    for (auto p = list.begin(); p != list.end(); p = p->next) {
        appendSingleUser(p->elem);
    }
}

void LibraryMain::appendSingleBook(Node<BookInfo>* p) {
    if (!p) return;
    QList<QStandardItem*> list;
    list << new QStandardItem(p->elem.name.data())
         << new QStandardItem(std::to_string(p->elem.identifier).data())
         << new QStandardItem(std::to_string(p->elem.quantity).data())
         << new QStandardItem(std::to_string(p->elem.quantity - p->elem.readers.size()).data());
    bookModel->appendRow(list);
}

void LibraryMain::appendSingleUser(Node<UserInfo>* p) {
    if (!p) return;
    QList<QStandardItem*> list;
    list << new QStandardItem(p->elem.name.data())
         << new QStandardItem(std::to_string(p->elem.identifier).data())
         << new QStandardItem(std::to_string(p->elem.books.size()).data());
    userModel->appendRow(list);
}

void LibraryMain::displaySingleBook(Node<BookInfo>* p) {
    initBookTable();
    appendSingleBook(p);
}

void LibraryMain::displaySingleUser(Node<UserInfo>* p) {
    initUserTable();
    appendSingleUser(p);
}

void LibraryMain::on_searchButton_clicked() {
    QString query = ui->searchBox->text();
    if (!ui->bookSwitchButton->isEnabled()) {
        if (query.isEmpty()) {
            displayBookData();
            return;
        }
        if (ui->selectNameButton->isChecked()) {
            displayBookList(lib.fuzzyFindBook(query.toStdString()));
        } else {
            displaySingleBook(lib.findBook(query.toInt()));
        }
    } else {
        if (query.isEmpty()) {
            displayUserData();
            return;
        }
        if (ui->selectNameButton->isChecked()) {
            displayUserList(lib.fuzzyFindUser(query.toStdString()));
        } else {
            displaySingleUser(lib.findUser(query.toInt()));
        }
    }
}

int LibraryMain::getSelection() {
    int curRow = ui->tableView->currentIndex().row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

int LibraryMain::getSelection(const QModelIndex &index) {
    int curRow = index.row();
    QAbstractItemModel* itemModel = ui->tableView->model();
    QModelIndex itemIdx = itemModel->index(curRow, 1);
    QVariant dataTmp = itemModel->data(itemIdx);
    return dataTmp.toInt();
}

void LibraryMain::on_borrowButton_clicked() {
    int bookID = getSelection();
    if (lib.borrowBook(loginUserID, bookID)) {
        QMessageBox::information(this, tr("提示"), tr("没有这本书剩余了。"), QMessageBox::Ok);
        return;
    }
    displayBookData();
    ui->statusbar->showMessage(tr("成功借阅《") + tr(lib.findBook(bookID)->elem.name.data())
                               + tr("》。"), 3000);
}

void LibraryMain::on_returnButton_clicked() {
    int bookID = getSelection();
    if (lib.returnBook(loginUserID, bookID)) {
        QMessageBox::information(this, tr("提示"), tr("你没有借阅这本书。"), QMessageBox::Ok);
        return;
    }
    displayBookData();
    ui->statusbar->showMessage(tr("成功归还《") + tr(lib.findBook(bookID)->elem.name.data())
                               + tr("》。"), 3000);
}

void LibraryMain::on_tableView_clicked(const QModelIndex &index) {
    if (!ui->bookSwitchButton->isEnabled()) {
        int bookID = getSelection(index);
        updateButton(bookID, loginUserID);
    } else {
        ui->editButton->setDisabled(false);
    }
}

void LibraryMain::updateButton(int bookID, int userID) {
    auto book = lib.findBook(bookID);
    ui->editButton->setDisabled(false);

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

void LibraryMain::disableButton() {
    ui->borrowButton->setDisabled(true);
    ui->returnButton->setDisabled(true);
}

void LibraryMain::on_editButton_clicked() {
    if (!ui->bookSwitchButton->isEnabled()) {
        int bookID = getSelection();
        BookInfoDialog bookDialog(this, bookID);
        bookDialog.exec();
        ui->searchButton->click();
    }
}

void LibraryMain::on_addButton_clicked() {
    if (!ui->bookSwitchButton->isEnabled()) {
        BookInfoDialog bookDialog(this);
        bookDialog.exec();
        ui->searchButton->click();
    }
}

void LibraryMain::on_tableView_doubleClicked() {
    ui->editButton->click();
}


void LibraryMain::on_writeDataAction_triggered() {
    int bookState = lib.writeBook(lib.bookPath);
    int userState = lib.writeUser(lib.userPath);
    if (bookState || userState) {
        QMessageBox::warning(this, tr("错误"), tr("写入文件失败。"), QMessageBox::Ok);
        return;
    }
    ui->statusbar->showMessage(tr("成功写入文件 ") + tr(lib.bookPath)
                               + tr(", ") + tr(lib.userPath), 3000);
}

