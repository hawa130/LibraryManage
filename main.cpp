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
    lib.read("D:\\Users\\hawa130\\Documents\\QtProj\\LibraryManage\\user.csv",
             "D:\\Users\\hawa130\\Documents\\QtProj\\LibraryManage\\book.csv");

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        LibraryMain w;
        w.show();
        return a.exec();
    }
    return 0;
}
