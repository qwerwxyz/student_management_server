#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <course.h>
#include <user.h>
#include <vector>
#include <QMap>
#include <QThread>
#define forcom(type, it, com) for (vector<type>::iterator it = com.begin(); it != com.end(); it++)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void new_connect();
    void Read();
    void Cinput();
    void Oinput();
    void StudentIn();
    void TeacherIn();
    void Update1();
    void SaveCom();
    void SaveOpt();
    void SaveStudent();
    void SaveTeacher();

public:
    vector<C_Student> vec_stu;
private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket*> sockets;
    vector<C_Teacher> vec_tea;
    vector<C_Compulsory>vec_compulsory1;//必修课库
    vector<C_Optional>vec_optional1;//选修课库
    QMap<QString, bool> stumap;
    QMap<QString, bool> teamap;
};




#endif // MAINWINDOW_H
