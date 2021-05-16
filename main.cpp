#include "librarymain.h"
#include "logindialog.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

Library lib;
int loginUserID;
bool isLoginAdmin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("微软雅黑", 9));
    // 语言文件读取
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LibraryManage_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // 图书数据读取
    if (lib.read("user.csv",
                 "book.csv")) {
        loginUserID = -1;
        isLoginAdmin = true;
        LibraryMain w;
        w.show();
        return a.exec();
    }

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        LibraryMain w;
        w.show();
        return a.exec();
    }

    return 0;
}
