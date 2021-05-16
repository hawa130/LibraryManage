#ifndef LIBRARYDATA_H
#define LIBRARYDATA_H

#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <climits>
#include <cstdlib>

using std::string;
using std::ofstream;
using std::ifstream;
using std::ostream;
using std::istream;
using std::cout;
using std::cerr;
using std::endl;

class BookInfo;		// 图书信息类
class UserInfo;		// 用户类

// 链表节点
template<class T> struct Node {
    T elem;			// 链表元素
    Node<T> *prev;	// 前向指针
    Node<T> *next;	// 后向指针

    friend ostream &operator <<(ostream &output, const Node &node) {
        output << node.elem;
        return output;
    }

    friend ostream &operator <<(ostream &output, const Node *&node) {
        output << node->elem;
        return output;
    }
};
// 链表
template<class T> class List {
public:
    // 初始化链表
    List() {
        head = new Node<T>;
        if (!head) {
            cerr << "内存分配失败.请检查剩余内存是否充足。" << endl;
            exit(1);
        }
        head->elem = T(-1);
        head->prev = head;
        head->next = head;
    }

    ~List() {}
    // 重载下标运算符，便于访问链表元素
    T &operator [](int idx) {
        Node<T> *p = getNode(idx);
        if (!p) return head->elem;
        return p->elem;
    }
    // 重载输出流，输出链表元素
    friend ostream &operator <<(ostream &output, const List &list) {
        output << '[';
        for (Node<T> *p = list.head->next; p != list.head; p = p->next) {
            output << *p;
            if (p->next != list.head) output << ", ";
        }
        output << ']';
        return output;
    }

    friend ostream &operator <<(ostream &output, const List *&list) {
        output << '[';
        for (Node<T> *p = list->head->next; p != list->head; p = p->next) {
            output << *p;
            if (p->next != list->head) output << ", ";
        }
        output << ']';
        return output;
    }
    // 获得链表长度
    int size() {
        int ret = 0;
        for (Node<T> *p = head->next; p != head; p = p->next)
            ret++;
        return ret;
    }
    // 判断链表是否为空，空则返回true
    bool isEmpty() {
        return head->next == head;
    }
    // 根据索引值获取节点指针，若获取失败返回空指针
    Node<T>* getNode(int idx) {
        Node<T> *ret = nullptr;
        int len = size();
        if (idx < 0 || idx > len) {
            cerr << "无效的索引值。" << endl;
            return nullptr;
        }
        int i = 1;
        for (Node<T> *p = head->next; p != head; p = p->next, i++) {
            if (i == idx) {
                ret = p;
                break;
            }
        }
        return ret;
    }
    // 在指定节点后插入节点，返回插入的元素的指针，若插入失败返回空指针
    Node<T>* add(T val, Node<T> *pos) {
        Node<T> *item = new Node<T>;
        if (!item) {
            cerr << "内存分配失败.请检查剩余内存是否充足。" << endl;
            return nullptr;
        }
        item->elem		 = val;
        item->next		 = pos->next;
        pos->next		 = item;
        item->next->prev = item;
        item->prev		 = pos;
        return item;
    }

    Node<T>* add(T val, int pos) {
        return add(val, getNode(pos));
    }
    // 在链表末尾插入元素
    Node<T>* append(T val) {
        return add(val, head->prev);
    }
    // 删除指定位置的节点，返回被删除节点后一个节点的指针，若删除失败返回空指针
    Node<T>* del(Node<T> *pos) {
        if (pos == nullptr) {
            return nullptr;
        }
        if (pos == head) {
            cerr << "不能删除链表头元素。" << endl;
            return nullptr;
        }
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        Node<T> *ret = pos->next;
        delete pos;
        return ret;
    }

    Node<T>* del(int pos) {
        return del(getNode(pos));
    }
    // 删除第一个值为val的节点
    Node<T>* delByValue(T val) {
        return del(find(val));
    }
    // 删除末尾元素
    Node<T>* pop() {
        return del(head->prev);
    }
    // 修改链表元素，若修改成功返回改节点指针，若修改失败则返回空指针
    Node<T>* modify(Node<T> *pos, T val) {
        if (pos == nullptr) {
            return nullptr;
        }
        if (pos == head) {
            cerr << "不能修改链表头元素。" << endl;
            return nullptr;
        }
        pos->elem = val;
        return pos;
    }

    Node<T>* modify(int pos, T val) {
        return modify(getNode(pos), val);
    }
    // 查找第一个值为val的节点
    Node<T>* find(T val) {
        for (Node<T> *p = head->next; p != head; p = p->next) {
            if (p->elem == val) return p;
        }
        return nullptr;
    }
    // 清空并重置链表
    int clear() {
        for (Node<T> *p = head->prev->prev; p != head->prev; p = p->prev)
            delete p->next;
        head->prev = head;
        head->next = head;
        return 0;
    }
    // 返回链表第一个节点的指针
    Node<T>* begin() {
        return head->next;
    }
    // 返回链表最后一个节点的后一个元素的指针（即头节点的指针）
    Node<T>* end() {
        return head;
    }

private:
    Node<T> *head;

};

class UserInfo {
public:
    string name;					// 姓名
    string password;				// 密码
    int identifier;					// 编号
    int type;						// 用户类型
    List<Node<BookInfo>*> books;	// 已借阅的书籍
    List<int> booksID;

    UserInfo(): identifier(-1), type(-1) {}

    UserInfo(int n): identifier(n), type(0) {}

    UserInfo(string reader, int id, int _type):
        name(reader), identifier(id), type(_type) {}

    UserInfo(string reader, string pwd, int id, int _type):
        name(reader), password(pwd), identifier(id), type(_type) {}

    UserInfo(string reader, int id, int _type, List<int> books):
        name(reader), identifier(id), type(_type), booksID(books) {}

    UserInfo(string reader, string pwd, int id, int _type, List<int> books):
        name(reader), password(pwd), identifier(id), type(_type), booksID(books) {}

    ~UserInfo() {}

    friend ostream &operator <<(ostream &output, const UserInfo &reader) {
        output << "{\"" << reader.name << "\", \"" << reader.password << "\", " << reader.identifier
               << ", " << reader.type << ", " << reader.booksID << "}";
        return output;
    }

    friend ostream &operator <<(ostream &output, const UserInfo *&reader) {
        output << "{\"" << reader->name << "\", \"" << reader->password << "\", " << reader->identifier
               << ", " << reader->type << ", " << reader->booksID << "}";
        return output;
    }

};

class BookInfo {
public:
    string name;					// 名称
    int identifier;					// 编号
    int quantity;					// 数量
    List<Node<UserInfo>*> readers;	// 借阅该书的读者
    List<int> readersID;

    BookInfo(): identifier(-1), quantity(-1) {}

    BookInfo(int n): identifier(n), quantity(1) {}

    BookInfo(string book, int id, int num):
        name(book), identifier(id), quantity(num) {}

    BookInfo(string book, int id, int num, List<int> users):
        name(book), identifier(id), quantity(num), readersID(users) {}

    ~BookInfo() {}

    friend ostream &operator <<(ostream &output, const BookInfo &book) {
        output << "{\"" << book.name << "\", " << book.identifier << ", "
               << book.quantity << ", " << book.readers;
        output << "}";
        return output;
    }

    friend ostream &operator <<(ostream &output, const BookInfo *&book) {
        output << "{\"" << book->name << "\", " << book->identifier << ", "
               << book->quantity << ", " << book->readers << "}";
        return output;
    }

};

class Library {
public:
    List<BookInfo> books;
    List<UserInfo> users;
    const char *bookPath;
    const char *userPath;
    char DIVIDE_CHAR;

    Library() {
        // 获取csv文件分隔符
        short chartmp;
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, (LPTSTR)&chartmp, sizeof(chartmp));
        DIVIDE_CHAR = (char)chartmp;
    }

    Library(const char *userFile, const char *bookFile) {
        short chartmp;
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, (LPTSTR)&chartmp, sizeof(chartmp));
        DIVIDE_CHAR = (char)chartmp;
        read(userFile, bookFile);
    };

    ~Library() {}
    // 从文件读取数据
    int read(const char *userFile, const char *bookFile) {
        bookPath = bookFile;
        userPath = userFile;
        int userState = userDataReader(userFile);
        int bookState = bookDataReader(bookFile);
        if (userState || bookState) {
            cerr << "未读取到数据。" << endl;
            return 1;
        }
        // 预处理编号数据，添加节点指针到链表中
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            auto readersID = p->elem.readersID;
            for (auto *q = readersID.begin(); q != readersID.end(); q = q->next) {
                p->elem.readers.append(findUser(q->elem));
            }
            readersID.clear();
        }
        for (auto *p = users.begin(); p != users.end(); p = p->next) {
            auto booksID = p->elem.booksID;
            for (auto *q = booksID.begin(); q != booksID.end(); q = q->next) {
                p->elem.books.append(findBook(q->elem));
            }
            booksID.clear();
        }
        return 0;
    }
    // 写入文件信息
    int writeBook(const char *bookFile) {
        ofstream output(bookFile);
        if (!output) {
            cerr << "无法写入文件。请检查文件\"" << bookFile << "\"是否被占用。" << endl;
            return 1;
        }
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            output << p->elem.name << DIVIDE_CHAR << p->elem.identifier << DIVIDE_CHAR
                   << p->elem.quantity;
            auto readers = p->elem.readers;
            for (auto *q = readers.begin(); q != readers.end(); q = q->next) {
                Node<UserInfo> *user = q->elem;
                output << DIVIDE_CHAR << user->elem.identifier;
            }
            output << endl;
        }
        return 0;
    }

    int writeUser(const char *userFile) {
        ofstream output(userFile);
        if (!output) {
            cerr << "无法写入文件。请检查文件\"" << userFile << "\"是否被占用。" << endl;
            return 1;
        }
        for (auto *p = users.begin(); p != users.end(); p = p->next) {
            output << p->elem.name << DIVIDE_CHAR << p->elem.password << DIVIDE_CHAR
                   << p->elem.identifier << DIVIDE_CHAR << p->elem.type;
            auto books = p->elem.books;
            for (auto *q = books.begin(); q != books.end(); q = q->next) {
                Node<BookInfo> *book = q->elem;
                output << DIVIDE_CHAR << book->elem.identifier;
            }
            output << endl;
        }
        return 0;
    }
    // 按编号查找图书
    Node<BookInfo>* findBook(int id) {
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            if (p->elem.identifier == id) return p;
        }
        return nullptr;
    }
    // 按编号查找用户
    Node<UserInfo>* findUser(int id) {
        for (auto *p = users.begin(); p != users.end(); p = p->next) {
            if (p->elem.identifier == id) return p;
        }
        return nullptr;
    }
    // 按名称查找图书
    Node<BookInfo>* findBook(string name) {
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            if (p->elem.name == name) return p;
        }
        return nullptr;
    }
    // 按名称查找图书（模糊查找），返回一个链表，存有目标图书的节点指针
    List<Node<BookInfo>*> fuzzyFindBook(string name) {
        List<Node<BookInfo>*> ret;
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            if (p->elem.name.find(name) != string::npos) {
                ret.append(p);
            }
        }
        return ret;
    }
    // 按名称查找用户
    Node<UserInfo>* findUser(string name) {
        for (auto *p = users.begin(); p != users.end(); p = p->next) {
            if (p->elem.name == name) return p;
        }
        return nullptr;
    }
    // 按名称查找用户（模糊查找），返回一个链表，存有目标用户的节点指针
    List<Node<UserInfo>*> fuzzyFindUser(string name) {
        List<Node<UserInfo>*> ret;
        for (auto *p = users.begin(); p != users.end(); p = p->next) {
            if (p->elem.name.find(name) != string::npos) {
                ret.append(p);
            }
        }
        return ret;
    }
    // 添加图书信息
    Node<BookInfo>* add(BookInfo book) {
        return books.append(book);
    }
    // 添加用户信息
    Node<UserInfo>* add(UserInfo user) {
        return users.append(user);
    }
    // 删除图书节点，force=true 开启强制删除
    Node<BookInfo>* del(Node<BookInfo>* book, bool force = false) {
        if (book == nullptr) {
            cerr << "不存在符合条件的图书。" << endl;
            return nullptr;
        }
        auto readers = book->elem.readers;
        if (!readers.isEmpty()) {
            cerr << "[警告] 现在还有 " << readers.size() << " 名用户未还该书 《"
                 << book->elem.name << "》(" << book->elem.identifier << ")。" << endl;
            if (!force) return nullptr;
        }
        for (auto *p = readers.begin(); p != readers.end(); p = p->next) {
            Node<UserInfo> *user = p->elem;
            // cerr << "[警告] 用户 " << user->elem.name << "(" << user->elem.identifier << ") 未还该书." << endl;
            user->elem.books.delByValue(book);
        }
        return books.del(book);
    }
    // 删除用户节点，force=true 开启强制删除,并强制归还该书
    Node<UserInfo>* del(Node<UserInfo>* user, bool force = false) {
        if (user == nullptr) {
            cerr << "不存在符合条件的用户。" << endl;
            return nullptr;
        }
        auto books = user->elem.books;
        if (!books.isEmpty()) {
            cerr << "[警告] 该用户" << user->elem.name << "(" << user->elem.identifier
                 << ") " << "未还图书 " << books.size() << " 本。";
            if (!force) return nullptr;
        }
        for (auto *p = books.begin(); p != books.end(); p = p->next) {
            Node<BookInfo> *book = p->elem;
            book->elem.readers.delByValue(user);
        }
        return users.del(user);
    }

    Node<BookInfo>* delBook(int id, bool force = false) {
        return del(findBook(id), force);
    }

    Node<UserInfo>* delUser(int id, bool force = false) {
        return del(findUser(id), force);
    }

    Node<BookInfo>* delBook(string name, bool force = false) {
        return del(findBook(name), force);
    }

    Node<UserInfo>* delUser(string name, bool force = false) {
        return del(findUser(name), force);
    }

    Node<BookInfo>* modify(Node<BookInfo>* src, BookInfo target) {
        return books.modify(src, target);
    }

    Node<UserInfo>* modify(Node<UserInfo>* src, UserInfo target) {
        return users.modify(src, target);
    }

    Node<BookInfo>* updateBook(int id, BookInfo target) {
        return books.modify(findBook(id), target);
    }

    Node<UserInfo>* updateUser(int id, UserInfo target) {
        return users.modify(findUser(id), target);
    }

    Node<BookInfo>* updateBook(string name, BookInfo target) {
        return books.modify(findBook(name), target);
    }

    Node<UserInfo>* updateUser(string name, UserInfo target) {
        return users.modify(findUser(name), target);
    }

    int borrowBook(Node<UserInfo>* userNode, Node<BookInfo>* bookNode) {
        if (!userNode || !bookNode) {
            cerr << "不存在符合条件的图书或用户。" << endl;
            return 1;
        }
        BookInfo book = bookNode->elem;
        // 判断书是否还有剩余
        int quantity = book.quantity;
        if (quantity <= book.readers.size()) {
            cerr << "[信息] 该书 《" << book.name << "》(" << book.identifier << ") 已经被借完了。" << endl;
            return 1;
        }
        auto retUser = userNode->elem.books.append(bookNode);
        auto retBook = bookNode->elem.readers.append(userNode);
        // cout << retBook->elem->elem << "借了" << retUser->elem->elem << endl;
        if (!retUser || !retBook) return 1;
        return 0;
    }

    int borrowBook(int userID, int bookID) {
        return borrowBook(findUser(userID), findBook(bookID));
    }

    int borrowBook(string userName, string bookName) {
        return borrowBook(findUser(userName), findBook(bookName));
    }

    int returnBook(Node<UserInfo>* userNode, Node<BookInfo>* bookNode) {
        if (!userNode || !bookNode) {
            cerr << "不存在符合条件的图书或用户。" << endl;
            return 1;
        }
        auto retUser = userNode->elem.books.delByValue(bookNode);
        auto retBook = bookNode->elem.readers.delByValue(userNode);
        if (!retUser || !retBook) return 1;
        return 0;
    }

    int returnBook(int userID, int bookID) {
        return returnBook(findUser(userID), findBook(bookID));
    }

    int returnBook(string userName, string bookName) {
        return returnBook(findUser(userName), findBook(bookName));
    }
    // 判断用户是否为管理员
    bool isAdmin(Node<UserInfo>* user) {
        return user->elem.type == 1;
    }
    // 密码验证，判断是否登录成功
    bool login(string userName, string password) {
        auto result = findUser(userName);
        if (result) {
            return result->elem.password == password;
        } else {
            return false;
        }
    }

protected:
    int bookDataReader(const char *fileName) {
        ifstream input(fileName);
        if (!input) {
            cerr << "数据读取失败。请检查文件\"" << fileName << "\"是否存在。" << endl;
            return 1;
        }
        while (!input.eof()) {
            string line;
            getline(input, line);		// 读入一行
            if (line.empty()) continue;	// 若读到空行则跳过
            line += '\n';

            string buff;		// 临时存储块
            string name;		// 图书的名称
            int cnt = 0;		// 计算由分隔符隔开的块索引号
            int identifier;		// 图书的编号
            int quantity;		// 图书的数量
            List<int> IDs;		// 借阅图书的用户编号
            // 处理该行数据，按分隔符分隔处理
            for (string::iterator i = line.begin(); i != line.end(); i++) {
                if (*i == DIVIDE_CHAR || *i == '\n') {
                    switch (cnt) {
                    case 0:
                        name = buff;
                        buff.clear(); break;
                    case 1:
                        identifier = atoi(buff.data());
                        buff.clear(); break;
                    case 2:
                        quantity = atoi(buff.data());
                        buff.clear(); break;
                    default:
                        int id = atoi(buff.data());
                        if (id) IDs.append(id);
                        buff.clear(); break;
                    }
                    cnt++;
                } else {
                    buff += *i;
                }
            }

            books.append(BookInfo(name, identifier, quantity, IDs));
        }
        input.close();
        return 0;
    }

    int userDataReader(const char *fileName) {
        ifstream input(fileName);
        if (!input) {
            cerr << "数据读取失败。请检查文件\"" << fileName << "\"是否存在。" << endl;
            return 1;
        }
        while (!input.eof()) {
            string line;
            getline(input, line);		// 读入一行
            if (line.empty()) continue;	// 若读到空行则跳过
            line += '\n';

            string buff;		// 临时存储块
            string name;		// 用户的名称
            string password;	// 用户的密码
            int cnt = 0;		// 计算由分隔符隔开的块索引号
            int identifier;		// 用户的编号
            int quantity;		// 用户类型 0：非管理员；1：管理员
            List<int> IDs;		// 用户借阅的图书编号
            // 处理该行数据，按分隔符分隔处理
            for (string::iterator i = line.begin(); i != line.end(); i++) {
                if (*i == DIVIDE_CHAR || *i == '\n') {
                    switch (cnt) {
                    case 0:
                        name = buff;
                        buff.clear(); break;
                    case 1:
                        password = buff;
                        buff.clear(); break;
                    case 2:
                        identifier = atoi(buff.data());
                        buff.clear(); break;
                    case 3:
                        quantity = atoi(buff.data());
                        buff.clear(); break;
                    default:
                        int id = atoi(buff.data());
                        if (id) IDs.append(id);
                        buff.clear(); break;
                    }
                    cnt++;
                } else {
                    buff += *i;
                }
            }

            users.append(UserInfo(name, password, identifier, quantity, IDs));
        }
        input.close();
        return 0;
    }

};

extern Library lib;
extern int loginUserID;
extern bool isLoginAdmin;

#endif // LIBRARYDATA_H
