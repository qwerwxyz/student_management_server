#include "mainwindow.h"

QString C_Student::ShowCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//显示全校课程
    QString str = "全校必修课如下：\n课程名称\t\t课程编号\t学分\n";
    forcom(C_Compulsory, it, vec_compulsory1)
        str += QString::fromStdString(it->name) + "\t\t"
            + QString::number(it->num) + "\t\t" + QString::number(it->credit)
                + "\n";

    str += "全校选修课：\n课程名称\t\t课程编号\t学分\n";
    forcom(C_Optional, it, vec_optional1)
        if(it->name!="demo")
            str += QString::fromStdString(it->name)
            + "\t\t" + QString::number(it->num) + "\t\t" + QString::number(it->credit)
                    + "\n";
    return str;
}
QString C_Student::ShowOwnCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//显示自己的课程
    QString str = "你的必修课如下：\n课程名称\t课程编号\t学分\n";
    for (int i = 0; i < s_compulsory.size(); i++) {
        str += QString::fromStdString(vec_compulsory1[s_compulsory[i]].name)
                + "\t" + QString::number(vec_compulsory1[s_compulsory[i]].num)
                + "\t" + QString::number(vec_compulsory1[s_compulsory[i]].credit)
                + "\n";
    }
    if(s_optional.size()==0){
        str += "你没有选修课\n";
    }
    else{
        str += "你的选修课如下：\n";
        for (int i = 0; i < s_optional.size(); i++) {
        str += QString::fromStdString(vec_optional1[s_optional[i]].name)
                + "\t" + QString::number(vec_optional1[s_optional[i]].num)
                + "\t" + QString::number(vec_optional1[s_optional[i]].credit)
                + "\n";
        }
    }
    return str;
}

QString C_Student::ShowAllGrade(int a, vector<C_Compulsory> &vec_compulsory1, vector<C_Optional> &vec_optional1) {//显示自己的成绩
    GetCourseGrade(vec_compulsory1, vec_optional1);
    if (a == 1) sort(coursedata1.begin(), coursedata1.end(), CompareNumBig);//选择排序方式
    else if (a == 2)sort(coursedata1.begin(), coursedata1.end(), CompareNumSmall);
    else if (a == 3)sort(coursedata1.begin(), coursedata1.end(), CompareGraBig);
    else if (a == 4)sort(coursedata1.begin(), coursedata1.end(), CompareGraSmall);
    QString str = "名称    \t成绩     \t绩点\n";
    for (int i = 0; i < coursedata1.size(); i++) {
        str += QString::fromStdString(coursedata1[i].name) + "\t" +
                QString::number(coursedata1[i].grade) + "\t" + QString::number(coursedata1[i].gpa) + "\n";
    }
    return str;
}
bool C_Student::CompareNumBig(const C_Coursedata &a,const C_Coursedata &b) {//按成绩排序
    if (a.grade > b.grade)return true;
    return false;
}
bool C_Student::CompareNumSmall(const C_Coursedata &a, const C_Coursedata &b) {
    if (a.grade < b.grade)return true;
    return false;
}
bool C_Student::CompareGraBig(const C_Coursedata &a, const C_Coursedata &b) {//按绩点排序
    if (a.gpa > b.gpa)return true;
    return false;
}
bool C_Student::CompareGraSmall(const C_Coursedata &a, const C_Coursedata &b) {
    if (a.gpa < b.gpa)return true;
    return false;
}
bool C_Student::Choose(C_Optional &A, vector<C_Student> &stu) {
    for (vector<CStu_data>::iterator it = A.data.begin(); it != A.data.end(); it++) {//在A课程中找学生
        if (it->name == name) {

            return 0;
        }
    }
    CStu_data a = CStu_data(false, "空", "0","0","0" ,-1, 0);
    forcom (C_Student, itnum, stu) {//将学生放入课程的信息中
        if (itnum->name == name) {
            CStu_data buffer = CStu_data(false, itnum->name,itnum->num,itnum->classnum,itnum->college, -1, 0);
            a = buffer;
            break;
        }
    }
    olen++;
    s_optional.push_back(A.num);//将课程放入学生的信息中
    A.data.push_back(a);
    A.stu_num++;
    return 1;
}

bool C_Student::Del(C_Optional &A, vector<C_Student> &stu) {//将学生从课程的信息中删除
    int flag = 0;
    vector<CStu_data>::iterator it;
    for ( it = A.data.begin(); it != A.data.end(); it++) {
        if (it->flag == true) {

            return 0;
        }
        if (it->num == num) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {//在课程的学生名单中找不到该学生

        return 0;
    }
    A.data.erase(it);
    vector<int>::iterator it2;
    for (it2 = s_optional.begin(); it2 != s_optional.end(); it2++) {
        if ((*it2) == A.num) {//遍历找到课程
            break;
        }
    }
    olen--;
    s_optional.erase(it2);//删除课程信息
    A.stu_num--;
    return 1;
}

void C_Student::GetCourseGrade(vector<C_Compulsory> &vec_compulsory1, vector<C_Optional> &vec_optional1) {//显示成绩
    coursedata1.clear();
    for (int i = 0; i < s_compulsory.size(); i++) {
        if (vec_compulsory1[s_compulsory[i]].data[0].grade == -1) continue;
        else
            for (int j = 0; j < vec_compulsory1[s_compulsory[i]].data.size(); j++) {
                if (vec_compulsory1[s_compulsory[i]].data[j].name == this->name) {
                    C_Coursedata A = C_Coursedata(vec_compulsory1[s_compulsory[i]].name, vec_compulsory1[s_compulsory[i]].num, vec_compulsory1[s_compulsory[i]].data[j].grade, vec_compulsory1[s_compulsory[i]].data[j].gpa);
                    coursedata1.push_back(A);
            }
        }
    }
    for (int i = 0; i < s_optional.size(); i++) {
        if (vec_optional1[s_optional[i]].data[0].grade == -1) continue;
        else
            for (int j = 0; j < vec_optional1[s_optional[i]].data.size(); j++) {
                if (vec_optional1[s_optional[i]].data[j].name == this->name) {
                    C_Coursedata A = C_Coursedata(vec_optional1[s_optional[i]].name, vec_optional1[s_optional[i]].num, vec_optional1[s_optional[i]].data[j].grade, vec_optional1[s_optional[i]].data[j].gpa);
                    coursedata1.push_back(A);
                }
            }
    }
}
