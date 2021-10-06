#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "headers.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT
public:
    QSqlDatabase myDB;
    void connClose()
        {
            myDB.close();
            myDB.removeDatabase(QSqlDatabase::defaultConnection);
        }

    bool connOpen()
        {
        //DATABASE
        QSqlDatabase myDB=QSqlDatabase::addDatabase("QSQLITE");
        myDB.setDatabaseName("/sqlite/database.db");


        if(!myDB.open()){
            qDebug()<< ("Failed to connect");
            return false;
        }
        else{
            qDebug()<< ("Database Connected");
            return true;
            }
        }
public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
