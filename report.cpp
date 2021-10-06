#include "report.h"
#include "ui_report.h"
#include "headers.h"

Report::Report(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Report)
{
    ui->setupUi(this);
    this->setFixedSize(710,500);
    Login conn;
    conn.connOpen();// OPEN CONNECTION

    ui->load->setEnabled(false);
    ui->excel->setEnabled(false);

}

Report::~Report()
{
    delete ui;
}


void Report::on_exit_clicked()
{
    Login conn;
    this->hide();
    ui->tableView_report->clearSpans();
    conn.connClose();

}


void Report::on_load_clicked()
{
    loadReport();
}

void Report::loadReport()
{
    //TABLE VIEW REPORT
    Login conn;
    QSqlQueryModel *modal =new QSqlQueryModel;

    conn.connOpen();
    QSqlQuery *customer=new QSqlQuery(conn.myDB);

    customer->prepare("select * from reportbook");
    customer->exec();
    modal->setQuery(*customer);
    ui->tableView_report->setModel(modal);
    qDebug() << (modal->rowCount());
}

void Report::on_pushButton_login_clicked()
{
    Login conn;
    QString password;

    password=ui->line_password->text();


    conn.connOpen();// OPEN CONNECTION
    QSqlQuery login;
    login.prepare("select * from users where passwords='"+password+"'");
    if(login.exec())
    {
    int count=0;
    while(login.next())
    {
        ++count;
    }
    if(count==1)
    {

        QMessageBox::critical(this,
                              tr("Success!"),
                              tr("Password is correct"));
        loadReport();
        ui->load->setEnabled(true);
        ui->line_password->clear();
        conn.connClose();// CLOSE CONNECTION
    }
    if(count>1)
    {

        QMessageBox::critical(this,
                              tr("Error!"),
                              tr("Duplicated user and password"));
        ui->load->setEnabled(false);
    }
    if(count<1)
    {
        QMessageBox::critical(this,
                              tr("Error!"),
                              tr("Password  incorrect"));
        ui->load->setEnabled(false);
    }
    }
    else
    {
    QMessageBox::critical(this,tr("error;;"),login.lastError().text());

    }
}
