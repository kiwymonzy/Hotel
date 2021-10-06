#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QDateEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(875,500);
            report= new Report(this);

            //LOAD DATA
            loadCheckout();
            loadRoomType();
            updateDetails();

            //DATABASE CONNECTION
            Login conn;
            if(!conn.connOpen())
                ui->database->setText("Failed to connect");
            else
                ui->database->setText("Database Connected");


            //DATE UPDATE
            showTime();
            QTimer *timer=new QTimer(this);
            connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
            timer->start();
            ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            ui->dateTimeEdit2->setDateTime(QDateTime::currentDateTime());

            //
            ui->pushButton_update->setEnabled(false);

}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_checkin_clicked()
{
    checkin();
    loadCheckout();
}



void MainWindow::on_pushButton_checkout_clicked()
{
    checkout();
    updateDetails();
}



void MainWindow::on_tableView_room_activated(const QModelIndex &index)
{
            QString val=ui->tableView_room->model()->data(index).toString();


            Login conn;
                    if(!conn.connOpen()){
                qDebug()<< "Failed to open the database";
                return;
            }

            conn.connOpen();// OPEN CONNECTION
            QSqlQuery qry;
            qry.prepare("select * from customer where roomno='"+val+"'");

                 if(qry.exec())
            {
                while(qry.next())
                {
                    ui->line_surname_2->setText(qry.value(1).toString());
                    ui->line_firstname_2->setText(qry.value(0).toString());
                    ui->line_roomno_2->setText(qry.value(5).toString());

                    //ui->pushButton_checkout->setEnabled(true);
                    //ui->pushButton_checkin->setEnabled(false);
                }
            }


}


void MainWindow::on_pushButton_clicked()
{
   loadCheckout();
}


void MainWindow::on_exit_clicked()
{
    QMessageBox messageBox(QMessageBox::Warning, "waring", "Exit the system?", QMessageBox::Yes|QMessageBox::No);
    if(messageBox.exec()==QMessageBox::Yes)
        qApp->exit(0);
}


void MainWindow::on_report_clicked()
{
    report->show();
}


void MainWindow::on_clear_checkin_clicked()
{
    ui->line_surname->clear();
    ui->line_firstname->clear();
    ui->line_nationality->clear();
    ui->line_mobileno->clear();
    ui->line_address->clear();
    ui->line_roomno->clear();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->pushButton_update->setEnabled(false);
    ui->pushButton_checkin->setEnabled(true);

}


void MainWindow::on_clear_checkout_clicked()
{
    ui->line_surname_2->clear();
    ui->line_firstname_2->clear();
    ui->line_roomno_2->clear();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}


void MainWindow::on_pushButton_time_clicked()
{
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}


void MainWindow::showTime()
{
        QTime time=QTime::currentTime();
        QString time_text=time.toString(("HH : mm : ss"));
        ui->Digital_clock->setText(time_text);
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->dateTimeEdit2->setDateTime(QDateTime::currentDateTime());

}




void MainWindow::checkin()
{   
    Login conn;//CHECKIN VARIABLES

    //CHECKIN STATEMENTS
    if(!conn.connOpen())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    checkRoom();
    return;
}

void MainWindow::checkout()
{
    //CHECKOUT VARIABLES
    Login conn;
    QString firstname,surname,roomno,date;
    firstname=ui->line_firstname_2->text();
    surname=ui->line_surname_2->text();
    roomno=ui->line_roomno_2->text();
    date=ui->dateTimeEdit2->dateTime().toString("dd-MM-yyyy  HH:mm:ss ap");


    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }

    conn.connOpen();// OPEN CONNECTION
    QSqlQuery checkout,dateout;

    checkout.prepare("delete from customer where roomno='"+roomno+"'");
    dateout.prepare("update reportbook set dateout='"+date+"' where roomno='"+roomno+"'");
    dateout.exec();
    if(checkout.exec())
      {
        QMessageBox::critical(this,
                              tr("Success!"),
                              tr("ROOM CHECKED-OUT!"));

        ui->line_surname_2->clear();
        ui->line_firstname_2->clear();
        ui->line_roomno_2->clear();
        loadCheckout();
        conn.connClose(); // CLOSE CONNECTION
      }
    else
    {
        QMessageBox::critical(this,tr("error;;"),checkout.lastError().text());

    }
    return;
}

void MainWindow::loadCheckout()
{
    //TABLE VIEW ROOMS
    Login conn;
    QSqlQueryModel *modal =new QSqlQueryModel;

    conn.connOpen();// OPEN CONNECTION
    QSqlQuery *customer=new QSqlQuery(conn.myDB);

    customer->prepare("select roomno from customer");
    customer->exec();
    modal->setQuery(*customer);
    ui->tableView_room->setModel(modal);
    conn.connClose();// CLOSE CONNECTION
    qDebug() << (modal->rowCount());
}

void MainWindow::checkRoom()
{
    Login conn;
    QString surname,firstname,nationality,mobileno,address,roomno,changes,roomtype,datein,dateout;

    surname=ui->line_surname->text();
    firstname=ui->line_firstname->text();
    nationality=ui->line_nationality->text();
    mobileno=ui->line_mobileno->text();
    address=ui->line_address->text();
    roomno=ui->line_roomno->text();
    changes=ui->dateTimeEdit2->dateTime().toString("NILL");
    roomtype=ui->RoomcomboBox->currentText();
    datein=ui->dateTimeEdit->dateTime().toString("dd-MM-yyyy  HH:mm:ss ap");
    dateout=ui->dateTimeEdit2->dateTime().toString("NILL");

    conn.connOpen();// OPEN CONNECTION
    QSqlQuery checkin,report,roomavailable;
    roomavailable.prepare("select roomno from customer where roomno='"+roomno+"'");
    if(roomavailable.exec())
    {
        int count=0;
        while(roomavailable.next())
        {
            ++count;
        }
        if(count==1)
        {

            QMessageBox::critical(this,
                                  tr("Error!"),
                                  tr("ROOM NOT AVAILALE"));
            updateDetails();
            ui->line_roomno->clear();
        }
        if(count>1)
        {

            QMessageBox::critical(this,
                                  tr("Error!"),
                                  tr("ROOM NOT AVAILALE"));
            updateDetails();
            ui->line_roomno->clear();
        }
        if(count<1)
        {

            checkin.prepare("insert into customer(surname,firstname,mobileno,nationality,address,roomno,roomtype,datein,dateout) values ('"+surname+"','"+firstname+"','"+mobileno+"','"+nationality+"','"+address+"','"+roomno+"','"+roomtype+"','"+datein+"','"+dateout+"')");
            report.prepare("insert into reportbook(surname,firstname,mobileno,nationality,address,roomno,changes,roomtype,datein,dateout) values ('"+surname+"','"+firstname+"','"+mobileno+"','"+nationality+"','"+address+"','"+roomno+"','"+changes+"','"+roomtype+"','"+datein+"','"+dateout+"')");
            report.exec();
            checkin.exec();
            QMessageBox::critical(this,
                                  tr("Success!"),
                                  tr("ROOM CHECKED-IN!"));
            ui->line_surname->clear();
            ui->line_firstname->clear();
            ui->line_nationality->clear();
            ui->line_mobileno->clear();
            ui->line_address->clear();
            ui->line_roomno->clear();
            ui->dateTimeEdit->update();
            updateDetails();
            conn.connClose();// CLOSE CONNECTION
        }
    }
    else
    {
        QMessageBox::critical(this,tr("error;;"),roomavailable.lastError().text());

    };
}

void MainWindow::loadRoomType()
{
    //TABLE VIEW REPORT
    Login conn;
    QSqlQueryModel *roomtype =new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery *qry=new QSqlQuery(conn.myDB);

    qry->prepare("select roomtype from room");
    qry->exec();
    roomtype->setQuery(*qry);
    ui->RoomcomboBox->setModel(roomtype);
    qDebug() << (roomtype->rowCount());
}

void MainWindow::updateRoom()
{
    //TABLE VIEW REPORT
    Login conn;
    QSqlQueryModel *update =new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery *qry=new QSqlQuery(conn.myDB);

    qry->prepare("select roomtype from room");
    qry->exec();
    update->setQuery(*qry);
    ui->RoomcomboBox->setModel(update);
    qDebug() << (update->rowCount());
}

void MainWindow::on_listView_activated(const QModelIndex &index)
{
    QString val=ui->listView->model()->data(index).toString();


    Login conn;
            if(!conn.connOpen()){
        qDebug()<< "Failed to open the database";
        return;
    }

    conn.connOpen();// OPEN CONNECTION
    QSqlQuery qry;
    qry.prepare("select * from customer where firstname='"+val+"'");

         if(qry.exec())
    {
        while(qry.next())
        {
            ui->line_firstname->setText(qry.value(0).toString());
            ui->line_surname->setText(qry.value(1).toString());
            ui->line_mobileno->setText(qry.value(2).toString());
            ui->line_nationality->setText(qry.value(3).toString());
            ui->line_address->setText(qry.value(4).toString());
            ui->line_roomno->setText(qry.value(5).toString());




            ui->dateTimeEdit->setEnabled(false);
            ui->pushButton_update->setEnabled(true);
            ui->pushButton_checkin->setEnabled(false);

        }
    }


}

void MainWindow::updateDetails()
{
    //TABLE VIEW ROOMS
    Login conn;
    QSqlQueryModel *modal =new QSqlQueryModel;

    conn.connOpen();// OPEN CONNECTION
    QSqlQuery *qry=new QSqlQuery(conn.myDB);

    qry->prepare("select firstname from customer");
    qry->exec();
    modal->setQuery(*qry);
    ui->listView->setModel(modal);
    conn.connClose();// CLOSE CONNECTION
    qDebug() << (modal->rowCount());
}

void MainWindow::on_pushButton_update_clicked()
{
    UpdateCustomer();
}

void MainWindow::UpdateCustomer()
{
    Login conn;
    QString surname,firstname,nationality,mobileno,address,roomno,roomtype,datein,dateout;

    surname=ui->line_surname->text();
    firstname=ui->line_firstname->text();
    nationality=ui->line_nationality->text();
    mobileno=ui->line_mobileno->text();
    address=ui->line_address->text();
    roomno=ui->line_roomno->text();
    roomtype=ui->RoomcomboBox->currentText();
    conn.connOpen();// OPEN CONNECTION
    QSqlQuery roomavailable;
    roomavailable.prepare("select roomno from customer where roomno='"+roomno+"'");
    if(roomavailable.exec())
    {
        int count=0;
        while(roomavailable.next())
        {
            ++count;
        }
        if(count==1)
        {

            QMessageBox::critical(this,
                                  tr("Error!"),
                                  tr("ROOM NOT AVAILALE"));
            updateDetails();
            ui->line_roomno->clear();
        }
        if(count>1)
        {

            QMessageBox::critical(this,
                                  tr("Error!"),
                                  tr("ROOM NOT AVAILALE"));
            updateDetails();
            ui->line_roomno->clear();
        }
        if(count<1)
        {
            QSqlQuery roomn,roomUP;
            roomUP.prepare("update reportbook set firstname='"+firstname+"', surname='"+surname+"', mobileno='"+mobileno+"',nationality='"+nationality+"',address='"+address+"',changes='"+roomno+"',roomtype='"+roomtype+"' where firstname='"+firstname+"'");
            roomn.prepare("update customer set firstname='"+firstname+"', surname='"+surname+"', mobileno='"+mobileno+"',nationality='"+nationality+"',address='"+address+"',roomno='"+roomno+"',roomtype='"+roomtype+"' where firstname='"+firstname+"'");
            roomUP.exec();
            roomn.exec();
            QMessageBox::critical(this,
                                  tr("Success!"),
                                  tr("ROOM CHECKED-IN!"));
            ui->line_surname->clear();
            ui->line_firstname->clear();
            ui->line_nationality->clear();
            ui->line_mobileno->clear();
            ui->line_address->clear();
            ui->line_roomno->clear();
            ui->dateTimeEdit->update();
            updateDetails();
            ui->pushButton_checkin->setEnabled(true);
            ui->pushButton_update->setEnabled(false);
            conn.connClose();// CLOSE CONNECTION
        }
    }
    else
    {
        QMessageBox::critical(this,tr("error;;"),roomavailable.lastError().text());

    };
}

