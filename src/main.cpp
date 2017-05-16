#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    int ret = -1;
    do
    {
        LoginDialog login;
        if (login.exec() == QDialog::Accepted)
        {
            MainWindow w;
            w.show();
            ret = a.exec();
        }
        else
            ret = 0;
    } while (ret != 0);

    return ret;
}
