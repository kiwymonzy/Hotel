#ifndef REPORT_H
#define REPORT_H

#include <QMainWindow>
#include "headers.h"

namespace Ui {
class Report;
}

class Report : public QMainWindow
{
    Q_OBJECT





public:
    explicit Report(QWidget *parent = nullptr);
    ~Report();


private slots:

    void on_exit_clicked();

    void on_load_clicked();

    void loadReport();

    void on_pushButton_login_clicked();

private:
    Ui::Report *ui;
};

#endif // REPORT_H
