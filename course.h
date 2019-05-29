#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
using namespace std;

class CStu_data {//课程类中的学生信息
public:
    bool flag;//是否提交
    string name;//学生姓名
    string num;//学生学号
    string classnum;//学生班级
    string college;//学院
    double grade;//成绩
    double gpa;//绩点
    CStu_data(bool flag, string name, string num,string classnum,string college, double grade, double gpa) {//构造函数
        this->flag = flag;
        this->gpa = gpa;
        this->grade = grade;
        this->num = num;
        this->name = name;
        this->classnum = classnum;
        this->college = college;
    }
    CStu_data() {

    }
    CStu_data(CStu_data const &a){//拷贝构造
        flag = a.flag;
        name = a.name;
        num = a.num;
        grade = a.grade;
        gpa = a.gpa;
        classnum = a.classnum;
        college = a.college;
    }
};
class FactoryCourse {//课程类
public:
    string name;//课程名字
    int stu_num;//学生人数
    int num;//课程号
    double credit;//学分
    virtual double getgpa(double a)=0;//虚函数
    vector<CStu_data>data;//学生数据
};

class C_Compulsory: public FactoryCourse {//必修课
public:
    virtual double getgpa(double a) {//实例化
        return (a / 10) * this->credit;
    }
    vector<CStu_data>data;//学生库
    //void input();
    C_Compulsory(string name, int num, double credit, int stu_num) {//构造函数
        this->stu_num = stu_num;
        this->name = name;
        this->num = num;
        this->credit = credit;
    }
    C_Compulsory(){}
    C_Compulsory(const C_Compulsory &A) {//拷贝构造
        name = A.name;
        num = A.num;
        stu_num = A.stu_num;
        credit = A.credit;
        data = A.data;
    }
};
class C_Optional : public FactoryCourse {//选修课
public:
    virtual double getgpa(double a) {
        return sqrt(a) / 10 * this->credit;
    }
    C_Optional(string name, int num, double credit, int stu_num) {//构造函数
        this->name = name;
        this->num = num;
        this->credit = credit;
        this->stu_num = stu_num;
    }
    C_Optional(){}
    vector<CStu_data>data;//选修课的学生库
    C_Optional(const C_Optional &A) {//拷贝构造
        name = A.name;
        num = A.num;
        stu_num = A.stu_num;
        credit = A.credit;
        data = A.data;
    }
};

extern vector<C_Compulsory>vec_compulsory1;//必修课库
extern vector<C_Optional>vec_optional1;//选修课库

