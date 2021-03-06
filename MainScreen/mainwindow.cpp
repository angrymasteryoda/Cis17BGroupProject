#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "afterloginmenu.h"
#include "forgot.h"
#include "newaccount.h"
#include <QPainter>
#include <QGraphicsScene>
#include <Qwidget>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "sha1.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    //setWindowTitle(tr("Analog Clock"));
    //resize(200, 200);

    ui->setupUi(this);
    QPixmap bkgnd(":/rccTiger.jpg");
    //bkgnd=bkgnd.scaled(this->size(), Qt::KeepAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    /*
    QPainter painter(this);
    QPalette thing;
    thing.setBrush(QPalette::Window, Qt::black);
    painter.drawConvexPolygon(point,3);
    */
    //! [0] //! [2]
    check1=false;
    check2=false;
    checke=0;
    checkp=0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Check if theres @ and .com (email verification)
    email=ui->lineEmail->text();
    //Find @
    QString string1="@";
    if(email.contains(string1)){
        check1=true;

    }
    //If didnt find @
    else{
        check1=false;
    }
    //Find .com
    QString string2=".com";
    if(email.contains(string2)){
        check2=true;
    }
    //If didnt find .com
    else{
        check2=false;
    }
    //Valid email
    if(check1==true&&check2==true){
        QMessageBox test;
        test.setText("Valid Email");
        test.exec();
        checkee=true;
    }
    //Not Valid email
    else{
        QMessageBox test;
        test.setText("Not a valid Email");
        test.exec();
        checkee=false;
    }

    //If email input is empty
    if(ui->lineEmail->text()==NULL){
        QMessageBox msgBox;
        msgBox.setText("Write an email.");
        msgBox.exec();
        checke=0;
    }
    //If email input is not empty and has a email format (@, .com)
    if(ui->lineEmail->text()!=NULL&&checkee==true){
        email=ui->lineEmail->text();
        checke=1;
    }
    //If password input is empty
    if(ui->linePassword->text()==NULL){
        QMessageBox msgBox;
        msgBox.setText("Write a password.");
        msgBox.exec();
        checkp=0;
    }
    //If password input is not empty
    if(ui->linePassword->text()!=NULL){
        password=ui->linePassword->text();
        checkp=1;
    }
    //If both email and password is filled in do this
    if(checke==1&&checkp==1){
        //Hash email and password
        int eSize=email.size();
        int pSize=password.size();
        unsigned char ehash[20],phash[20];
        char ehexstring[41],phexstring[41];
        const char *emailc=email.toStdString().c_str();
        const char *passc=password.toStdString().c_str();
        sha1::calc(emailc,eSize,ehash);
        sha1::toHexString(ehash,ehexstring);
        sha1::calc(passc,pSize,phash);
        sha1::toHexString(phash,phexstring);
        //Save email hash to text
        QString filename1="email.txt";
        QFile eFile(filename1);
        //Doesn't overwrite when writing to file
        if(eFile.open(QFile::WriteOnly|QFile::Append)){
            QTextStream out(&eFile);
            out<<ehexstring<<endl;
        }
        eFile.close();
        //Save password hash to text
        QString filename2="password.txt";
        QFile pFile(filename2);
        //Doesn't overwrite when writing to file
        if(pFile.open(QFile::WriteOnly|QFile::Append)){
            QTextStream out(&pFile);
            out<<phexstring<<endl;
        }
        pFile.close();
        //Open Next Window
        AfterLoginMenu *k=new AfterLoginMenu;
        k->show();
        this->close();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    NewAccount *k=new NewAccount;
    k->show();
    this->close();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
void MainWindow::paintEvent(QPaintEvent *)
{

    QColor hourColor(249, 141, 32);
    QColor minuteColor(Qt::darkGray);

    QTime time = QTime::currentTime();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    //painter.scale(side / 200.0, side / 200.0);
    /*
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    */
    painter.translate(width() / 2 -10 , height() * .25 + 30);
    painter.scale(1.52, 1.52);
    painter.setPen(hourColor);
    painter.rotate(time.second());
    QPen k=painter.pen();
    k.setWidth(2);
    painter.setPen(k);

    for (int i = 0; i < 60; ++i) {
        painter.drawLine(105, 0, 90, 0);
        painter.rotate(30.0);
    }
    /*
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    */
    painter.rotate(time.second());
    painter.setPen(minuteColor);
    k=painter.pen();
    k.setWidth(2);
    painter.setPen(k);

    for (int j = 0; j < 60; ++j) {
        if ((j % 2) != 0)
            painter.drawLine(100, 0, 90, 0);
            painter.rotate(15.0);
    }
}

