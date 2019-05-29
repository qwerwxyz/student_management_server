#include "student.cpp"
#include "teacher.cpp"
#include "ui_mainwindow.h"
#include <fstream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <qtimer.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer();
    connect(server, &QTcpServer::newConnection, this, &MainWindow::new_connect);
    try {
        if (!server->listen(QHostAddress::Any, 8765)) throw 1;
        Cinput();
        Oinput();
        TeacherIn();
        StudentIn();
    }
    catch (int x) {
        if (x == 1) {
            QMessageBox::warning(this, "警告", "端口错误");
        }
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update1()));
    timer->start(10);


}

void MainWindow::Update1(){
    QString str="world";
    SaveTeacher();
    SaveStudent();
    SaveCom();
    SaveOpt();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::new_connect() {
    socket = server->nextPendingConnection();
    sockets.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read()));
}
void MainWindow::Read() {
    QByteArray message;
    QByteArray usenum;
    for (int ii = 0; ii < sockets.length(); ii++)
    {
        QDataStream in(sockets.at(ii));
        in.setVersion(QDataStream::Qt_5_4);
        message = sockets.at(ii)->readLine();
        if (message.isEmpty()) continue;
        try{
            if (message == "log\n") {
                ui->textEdit->insertPlainText("处理登录请求\n");
                message = sockets.at(ii)->readLine();
                if (message == "1\n") {
                    message = sockets.at(ii)->readLine();
                    if (stumap[message] == 1) throw 1;
                    int flag = 0;
                    for (int i = 0; i < vec_stu.size(); i++) {
                        if (QString::fromStdString(vec_stu[i].num) + "\n"  == QString::fromLatin1(message))
                        {
                            flag = 1;
                            usenum = message;
                            message = sockets.at(ii)->readLine();

                            if (QString::fromStdString(vec_stu[i].password) + "\n" == QString::fromLatin1(message)){
                                flag = 2;
                                break;
                            }
                        }
                    }
                    if (flag == 1) throw 8;
                    if (flag == 0) throw 9;
                    stumap[usenum] = 1;
                    sockets.at(ii)->write(tr("success\n").toLatin1());
                    sockets.at(ii)->write(tr("stu\n").toLatin1());
                    sockets.at(ii)->write(usenum);
                    sockets.at(ii)->flush();
                }
                else if (message == "2\n") {
                    message = sockets.at(ii)->readLine();
                    if (teamap[message] == 1) throw 1;
                    int flag = 0;
                    for (int i = 0; i < vec_tea.size(); i++) {
                        if (QString::fromStdString(vec_tea[i].num) + "\n" == QString::fromLatin1(message))
                        {
                            flag = 1;
                            usenum = message;
                            message = sockets.at(ii)->readLine();
                            if (QString::fromStdString(vec_tea[i].password) + "\n" == QString::fromLatin1(message))
                            {
                                flag = 2;
                                break;
                            }

                        }
                    }
                    if (flag == 1) throw 8;
                    if (flag == 0) throw 1;
                    teamap[usenum] = 1;
                    sockets.at(ii)->write(tr("success\n").toLatin1());
                    sockets.at(ii)->write(tr("tea\n").toLatin1());
                    sockets.at(ii)->write(usenum);
                    sockets.at(ii)->flush();
                }
                else throw 0;
            }
            else if (message == "course\n") {
                ui->textEdit->insertPlainText("处理显示全校课程\n");
                QString str = vec_tea[0].ShowCourse(vec_compulsory1,vec_optional1);
                sockets.at(ii)->write(tr("data\n").toLatin1());
                sockets.at(ii)->write(str.toUtf8());
                sockets.at(ii)->flush();
            }
            else if(message == "self\n") {
                ui->textEdit->insertPlainText("处理显示任教课程请求\n");
                message = sockets.at(ii)->readLine();
                int teanumber = -1;
                for (int i = 0; i < vec_tea.size(); i++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[i].num) + "\n"){
                        teanumber = i;
                        break;
                        }
                QString str = vec_tea[teanumber].ShowOwnCourse(vec_compulsory1,vec_optional1);
                sockets.at(ii)->write(tr("data\n").toLatin1());
                sockets.at(ii)->write(str.toUtf8());
                sockets.at(ii)->flush();
            }
            else if(message == "showstu\n"){
                ui->textEdit->insertPlainText("处理显示所教学生请求\n");
                message = sockets.at(ii)->readLine();
                int teanumber = -1;
                for (int i = 0; i < vec_tea.size(); i++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[i].num) + "\n"){
                        teanumber = i;
                        break;
                    }
                QString str = vec_tea[teanumber].ShowStu(vec_compulsory1,vec_optional1);
                sockets.at(ii)->write(tr("data\n").toLatin1());
                sockets.at(ii)->write(str.toUtf8());
                sockets.at(ii)->flush();

            }
            else if(message == "askstu\n"){
                ui->textEdit->insertPlainText("处理输入成绩预选择请求\n");
                message = sockets.at(ii)->readLine();//读老师的编号
                int k;//k是老师编号
                for (k = 0; k < vec_tea.size(); k++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[k].num) + "\n")
                        break;
                sockets.at(ii)->write(tr("returnstu\n").toLatin1());
                vec_tea[k].AskClass(sockets.at(ii),vec_compulsory1,vec_optional1);
            }

            else if(message == "inputgrade\n"){
                ui->textEdit->insertPlainText("处理输入成绩请求\n");
                message = sockets.at(ii)->readLine();//读老师的编号
                int k;//k是老师编号
                for (k = 0; k < vec_tea.size(); k++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[k].num) + "\n")
                        break;
                vec_tea[k].InputGrade(sockets.at(ii),vec_compulsory1,vec_optional1);

            }
            else if (message == "handgrade\n") {
                ui->textEdit->insertPlainText("处理提交成绩请求\n");
                message = sockets.at(ii)->readLine();//读老师的编号
                int k;
                for (k = 0; k < vec_tea.size(); k++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[k].num) + "\n")
                        break;
                vec_tea[k].HandGrade(sockets.at(ii),vec_compulsory1,vec_optional1);


            }
            else if (message == "logout\n") {
                ui->textEdit->insertPlainText("处理登出请求\n");
                QByteArray tmp = sockets.at(ii)->readLine();
                message = sockets.at(ii)->readLine();
                if (tmp == "1\n") stumap[message] = 0;
                else teamap[message] = 0;
            }
            else if(message == "asksort\n"){
                ui->textEdit->insertPlainText("处理老师排序\n");
                message = sockets.at(ii)->readLine();//读老师的编号
                int k;
                for (k = 0; k < vec_tea.size(); k++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_tea[k].num) + "\n")
                        break;
                sockets.at(ii)->write(tr("data\n").toLatin1());
                vec_tea[k].AskSort(sockets.at(ii),vec_compulsory1,vec_optional1);
            }
            else if(message == "show_own_course\n"){
                ui->textEdit->insertPlainText("处理显示学生课程请求\n");
                message = sockets.at(ii)->readLine();
                int stunumber;
                //vector<int> s_compulsory, s_optional;
                for (int i = 0; i < vec_stu.size(); i++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_stu[i].num) + "\n"){
                        stunumber = i;
                    }
                        //s_compulsory = stu[i].s_compulsory;
                QString str = vec_stu[stunumber].ShowOwnCourse(vec_compulsory1,vec_optional1);
                sockets.at(ii)->write(tr("data\n").toLatin1());
                sockets.at(ii)->write(str.toUtf8());
                sockets.at(ii)->flush();
            }
            else if (message == "choose\n") {
                ui->textEdit->insertPlainText("处理选课\n");
                message = sockets.at(ii)->readLine();
                int x = QString::fromLatin1(sockets.at(ii)->readLine()).toInt();
                if (x >= 0 && x < vec_optional1.size()) {
                    for (int i = 0; i < vec_stu.size(); i++)
                        if (QString::fromLatin1(message) == QString::fromStdString(vec_stu[i].num) + "\n")
                            if (vec_stu[i].Choose(vec_optional1[x], vec_stu)) {
                                sockets.at(ii)->write(tr("tip\n").toLatin1());
                                sockets.at(ii)->write(tr("选课成功\n").toUtf8());
                                sockets.at(ii)->flush();
                            }
                            else {
                                sockets.at(ii)->write(tr("tip\n").toLatin1());
                                sockets.at(ii)->write(tr("选课失败\n").toUtf8());
                                sockets.at(ii)->flush();
                            }
                }
                else throw 4;
            }
            else if (message == "withdraw\n") {
                ui->textEdit->insertPlainText("处理退课\n");
                message = sockets.at(ii)->readLine();
                int x = QString::fromLatin1(sockets.at(ii)->readLine()).toInt();
                if (x >= 0 && x < vec_optional1.size()) {
                    for (int i = 0; i < vec_stu.size(); i++)
                        if (QString::fromLatin1(message) == QString::fromStdString(vec_stu[i].num) + "\n")
                            if (vec_stu[i].Del(vec_optional1[x], vec_stu)) {
                                sockets.at(ii)->write(tr("tip\n").toLatin1());
                                sockets.at(ii)->write(tr("退课成功\n").toUtf8());
                                sockets.at(ii)->flush();
                            }
                            else {
                                sockets.at(ii)->write(tr("tip\n").toLatin1());
                                sockets.at(ii)->write(tr("退课失败\n").toUtf8());
                                sockets.at(ii)->flush();
                            }
                }
                else throw 4;
            }
            else if (message == "showallgrade\n") {
                ui->textEdit->insertPlainText("处理课程排序\n");
                message = sockets.at(ii)->readLine();//读学生的编号
                int k;
                for (k = 0; k < vec_stu.size(); k++)
                    if (QString::fromLatin1(message) == QString::fromStdString(vec_stu[k].num) + "\n")
                        break;
                int a = QString::fromLatin1(sockets.at(ii)->readLine()).toInt();
                sockets.at(ii)->write(tr("data\n").toLatin1());
                sockets.at(ii)->write(vec_stu[k].ShowAllGrade(a, vec_compulsory1, vec_optional1).toUtf8());
                sockets.at(ii)->flush();
            }
            else if (message == "renew\n"){
                Cinput();
                Oinput();
                TeacherIn();
                StudentIn();
            }
        }
        catch (int x) {
            if (x == 1) {
                QString str = "fail\n";//连接失败
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == 4 || x == 5){//输入序号错误
                QString str = "numerror\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == 6) {//不可更改
                QString str = "changeerror\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == 7) {//输入成绩错误
                QString str = "gradeerror\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == -1) {//提交错误
                QString str = "handfail\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == 8 ) {//密码错误
                QString str = "mistake\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }
            else if (x == 9 ) {//没有该用户
                QString str = "usernameerror\n";
                sockets.at(ii)->write(str.toLatin1());
                sockets.at(ii)->flush();
            }

        }
    }
}

