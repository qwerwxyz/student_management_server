#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"course.h"
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
using namespace std;
class C_Coursedata {//学生课程数据
public:
        string name;//课程名字
        int num;//序号
        double grade;//课程成绩
        double gpa;
        C_Coursedata(string name, int num, double grade, double gpa) {//构造函数
                this->name = name;
                this->grade = grade;
                this->gpa = gpa;
                this->num = num;
        }
        C_Coursedata(const C_Coursedata &A) {//拷贝构造
                this->name = A.name;
                this->grade = A.grade;
                this->gpa = A.gpa;
                this->num = A.num;
        }
};
class FactoryUser{//父类
public:
    string num;//学号
    string name;//姓名
    string college;//学院
    string password;//密码
    virtual QString ShowCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1) = 0;//显示课程
};
class C_Student : public FactoryUser{//学生类
public:
        string classnum;//班号
        int clen;//必修课数量
        int olen;//选修课数量
        C_Student(string num, string name, string college, string classnum,string password,vector<int>opt) {//构造函数
                this->name = name;
                this->num = num;
                this->college = college;
                this->classnum = classnum;
                this->password = password;
                for(int i = 0; i < vec_compulsory1.size(); i++){
                     s_compulsory[i] = vec_compulsory1[i].num;
                }
        }
        C_Student(){}
        QString ShowCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//显示全校课程
        void GetCourseGrade(vector<C_Compulsory> &vec_compulsory1, vector<C_Optional> &vec_optional1);//显示课程成绩
        QString ShowOwnCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//显示自己的课程
        bool Choose(C_Optional &A, vector<C_Student> &vec_stu);//选课
        void ChooseCourse(QTcpSocket* sockets,vector<C_Optional>vec_optional1);
        bool Del(C_Optional &A, vector<C_Student> &vec_stu);//退课
        QString ShowAllGrade(int a, vector<C_Compulsory> &vec_compulsory1, vector<C_Optional> &vec_optional1);//显示所有成绩
        vector<int>s_compulsory;//学生的必修课
        vector<int>s_optional;//学生的选修课
        vector<C_Coursedata>coursedata1;//课程数据
        static bool CompareNumBig(const C_Coursedata &a,const C_Coursedata &b);//排序函数
        static bool CompareNumSmall(const C_Coursedata &a, const C_Coursedata &b);
        static bool CompareGraBig(const C_Coursedata &a, const C_Coursedata &b);
        static bool CompareGraSmall(const C_Coursedata &a, const C_Coursedata &b);
};


class C_Teacher : public FactoryUser {//老师类
public:
    C_Teacher(string num, string name, string college, vector<int>com,vector<int>opt,string password) {//构造函数
        this->name = name;
        this->num = num;
        this->college = college;
        this->t_compulsory = com;
        this->t_optional = opt;
        this->password = password;
    }
    C_Teacher() {
    }
    C_Teacher(const C_Teacher &A) {//拷贝构造
        name = A.name;
        college = A.college;
        num = A.num;
        t_compulsory =A.t_compulsory;
        t_optional = A.t_optional;
        clen = A.clen;
        olen = A.olen;
        password = A.password;
    }
    int clen;//必修课数
    int olen;//选修课数
    vector<int>t_compulsory;//老师的必修课
    vector<int>t_optional;//老师的选修课
    QString ShowCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//显示课程
    QString ShowOwnCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//显示自身课程
    QString ShowStu(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//显示课程学生
    void AskClass(QTcpSocket* sockets,vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1);//选定课程
    void InputGrade(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1);//输入成绩
    void HandGrade(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1);//提交成绩
    void AskSort(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1);//排序
    static bool CompareNumBig(const CStu_data& a, const CStu_data& b);//排序函数，泛型函数
    static bool CompareNumSmall(const CStu_data& a,const CStu_data &b);
    static bool CompareGraBig(const CStu_data& a, const CStu_data& b);
    static bool CompareGraSmall(const CStu_data& a, const CStu_data& b);
};

extern vector<C_Teacher>vec_tea;//老师名单
extern vector<C_Student>vec_stu;//学生库
