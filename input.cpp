#include"course.h"
#include"user.h"
#include "mainwindow.h"
#include<fstream>

istream& operator>>(istream& FILE, C_Compulsory &A) {//重载运算符
    FILE >> A.name >> A.num >> A.stu_num >> A.credit;
    while (A.name.length() < 15) A.name += " ";
    return FILE;
}

istream& operator>>(istream& FILE, CStu_data &A) {//重载运算符
    FILE >> A.flag >> A.name >> A.num >> A.classnum >> A.college >> A.grade >> A.gpa;
    return FILE;
}

string root = "C:/Users/1wxyw/Desktop/kkk/";
void MainWindow::Cinput() {  //必修课的读取
    vec_compulsory1.clear();
    int i = 0, j = 0;
    ifstream file(root + "compulsory2.txt", ios::in);
    if (!file)
    {
        throw 0;
    }
    int num;
    file >> num;
    while (i < num) {
        j = 0;
        C_Compulsory Q;
        file >> Q;
        while (j < Q.stu_num) {
            CStu_data P;
            file >> P;
            Q.data.push_back(P);
            j++;
        }
        i++;
        vec_compulsory1.push_back(Q);
    }
}
istream& operator>>(istream& FILE, C_Optional &A) {
    FILE >> A.name >> A.num >> A.stu_num >> A.credit;
    while (A.name.length() < 15) A.name += " ";
    return FILE;
}
void MainWindow::Oinput() {//选修课的读取
    vec_optional1.clear();
    int i = 0, j = 0;
    ifstream file(root + "optional2.txt", ios::in);
    if (!file)
    {
        throw 0;
    }
    int num;
    file >> num;
    while (i < num) {
        j =  0;
        C_Optional Q;
        file >> Q;
        while (j < Q.stu_num) {
            CStu_data P;
            file >> P;
            Q.data.push_back(P);
            j++;
        }
        i++;
        vec_optional1.push_back(Q);
    }
}
istream& operator>>(istream& FILE, C_Teacher &A) {
    FILE >> A.name >> A.num >> A.password >> A.college >> A.clen>>A.olen;
    return FILE;
}
istream& operator>>(istream& FILE, vector<int> &A) {
    int a;
    FILE >> a;
    A.push_back(a);
    return FILE;
}
void MainWindow::TeacherIn() {//老师的读取
    vec_tea.clear();
    int i = 0, j = 0;
    ifstream file(root + "teacher2.txt", ios::in);
    if (!file)
    {
        throw 0;
    }
    int num;
    file >> num;
    while (i < num) {
        C_Teacher Q;
        file >> Q;
        for (int j = 0; j < Q.clen; j++) {
            file >> Q.t_compulsory;
        }
        for (int j = 0; j < Q.olen; j++) {
            file >> Q.t_optional;
        }
        vec_tea.push_back(Q);
        i++;
    }
}
istream& operator>>(istream& FILE, C_Student &A) {
    FILE >> A.name >> A.num >> A.password>> A.college >>A.classnum>> A.olen;
    return FILE;
}
void MainWindow::StudentIn() { //课程的读取
    vec_stu.clear();
    int i = 0, j = 0;
    ifstream file(root + "student2.txt", ios::in);
    if (!file)
    {
        throw 0;
    }
    int num;
    file>>num;
    while (i < num) {
        C_Student Q;
        for(int j = 0; j < vec_compulsory1.size(); j++){
             Q.s_compulsory.push_back(vec_compulsory1[j].num);
        }
        file >> Q;
        for (int j = 0; j < Q.olen; j++) {
            file >> Q.s_optional;
        }
        vec_stu.push_back(Q);
        i++;
    }
}
ostream& operator<<(ostream& FILE, C_Compulsory &A) {//重载运算符
    FILE << A.name<< " " << A.num <<" "<< A.stu_num <<" "<< A.credit<<endl;
    return FILE;
}
ostream& operator<<(ostream& FILE, CStu_data &A) {
    FILE << A.flag <<" "<< A.name<< " " << A.num<< " " << A.classnum <<" "<< A.college<<" " << A.grade <<" "<< A.gpa<<endl;
    return FILE;
}
void MainWindow::SaveCom() {//必修课的保存
    int i = 0, j = 0;
    ofstream file(root + "compulsory2.txt", ios::out);
    if (!file)
    {
        throw 0;
    }
    file<< vec_compulsory1.size() << endl;
    while (i < vec_compulsory1.size()) {
        j = 0;
        file << vec_compulsory1[i];
        while (j < vec_compulsory1[i].stu_num) {
            file << vec_compulsory1[i].data[j];
            j++;
        }
        i++;
    }
}
ostream& operator<<(ostream& FILE, C_Optional &A) {
    FILE << A.name<< " " << A.num<< " " << A.stu_num<< " " << A.credit<<endl;
    return FILE;
}
void MainWindow::SaveOpt() { //选修课的保存
    int i = 0, j = 0;
    ofstream file(root + "optional2.txt", ios::out);
    if (!file)
    {
        throw 0;
    }
    file<< vec_optional1.size() << endl;
    while (i < vec_optional1.size()) {
        j = 0;
        file << vec_optional1[i];
        while (j < vec_optional1[i].data.size()) {
            file << vec_optional1[i].data[j];
            j++;
        }
        i++;
    }
}
ostream& operator<<(ostream& FILE, C_Teacher &A) {
    FILE << A.name<<" "<< A.num <<" "<<A.password <<" "<< A.college<<" "<< A.clen<<" " <<A.olen;
    return FILE;
}
ostream& operator<<(ostream& FILE, vector<int> &A) {
    for (int i = 0; i < A.size(); i++) {
        FILE <<" "<< A[i];
    }
    return FILE;
}
void MainWindow::SaveTeacher() {//老师的保存
    int i = 0, j = 0;
    ofstream file(root + "teacher2.txt", ios::out);
    if (!file)
    {
        cout << "不可以打开文件" << endl;
        exit(1);
    }
    file<<vec_tea.size()<<endl;
    while (i < vec_tea.size()) {
        file << vec_tea[i];
        for (int j = 0; j < vec_tea[i].clen; j++) {
            file << vec_tea[i].t_compulsory;
        }
        for (int j = 0; j < vec_tea[i].olen; j++) {
            file <<vec_tea[i].t_optional;
        }
        file << endl;
        i++;
    }
}
ostream& operator<<(ostream& FILE, C_Student &A) {
    FILE << A.name<<" " << A.num <<" "<<A.password<<" "<< A.college<<" " << A.classnum<<" "<< A.olen;
    return FILE;
}
void MainWindow::SaveStudent() {//学生的保存
    int i = 0, j = 0;
    ofstream file(root + "student2.txt", ios::out);
    if (!file)
    {
        throw 0;
    }
    file<<vec_stu.size()<<endl;
    while (i < vec_stu.size()) {
        file << vec_stu[i];
        for (int j = 0; j < vec_stu[i].olen; j++) {
            file <<" "<< vec_stu[i].s_optional;
        }
        i++;
        file << endl;
    }

}
