#include "mainwindow.h"

QString C_Teacher::ShowCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//显示全校课程
    QString str = "全校必修课如下：\n课程名称\t\t课程编号\t\t学分\n";
    forcom(C_Compulsory, it, vec_compulsory1)
        str += QString::fromStdString(it->name) + "\t\t"
            + QString::number(it->num) + "\t" + QString::number(it->credit)
                + "\n";

    str += "全校选修课：\n课程名称\t\t课程编号\t\t学分\n";
    forcom(C_Optional, it, vec_optional1)
        if(it->name!="demo")
            str += QString::fromStdString(it->name)
            + "\t\t" + QString::number(it->num) + "\t" + QString::number(it->credit)
                    + "\n";
    return str;
}
QString C_Teacher::ShowOwnCourse(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//显示自己的课程
    QString str = "您任教的必修课如下：\n课程名称\t\t课程编号\t\t学分\n";
    for (int i = 0; i < t_compulsory.size(); i++) {
        str += QString::fromStdString(vec_compulsory1[t_compulsory[i]].name)
                + "\t\t" + QString::number(vec_compulsory1[t_compulsory[i]].num)
                + "\t" + QString::number(vec_compulsory1[t_compulsory[i]].credit)
                + "\n";
    }
    str += "您任教的选修课如下：\n";
    for (int i = 0; i < t_optional.size(); i++) {
        str += QString::fromStdString(vec_optional1[t_optional[i]].name)
                + "\t\t" + QString::number(vec_optional1[t_optional[i]].num)
                + "\t" + QString::number(vec_optional1[t_optional[i]].credit)
                + "\n";
    }
    return str;
}
QString C_Teacher::ShowStu(vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//显示课程学生
    QString str = "您任教的必修课有：\n";
    for (int i = 0; i < t_compulsory.size(); i++) {
        str += QString::fromStdString(vec_compulsory1[t_compulsory[i]].name)
               +"\n该课程的学生有：\n姓名\t学号\t班级\t学院\n";
        forcom(CStu_data, it2, vec_compulsory1[t_compulsory[i]].data) {
            str += QString::fromStdString(it2->name)+"\t"+ QString::fromStdString(it2->num) + "\t"
                + QString::fromStdString(it2->classnum) + "\t" +  QString::fromStdString(it2->college)+"\n";
        }
    }
    str += "您任教的选修课有：\n";
    for (int i = 0; i < t_optional.size(); i++) {
        str += QString::fromStdString(vec_optional1[t_optional[i]].name)
               +"\n该课程的学生有：\n姓名\t学号\t班级\t学院\n";
        forcom(CStu_data, it2, vec_optional1[t_optional[i]].data) {
            str += QString::fromStdString(it2->name)+"\t"+ QString::fromStdString(it2->num) + "\t"
                + QString::fromStdString(it2->classnum) + "\t" +  QString::fromStdString(it2->college)+"\n";
        }
    }
    return str;
}
void C_Teacher::AskClass(QTcpSocket* sockets,vector<C_Compulsory>vec_compulsory1,vector<C_Optional>vec_optional1){//寻找课程
    int ctype = QString::fromLatin1(sockets->readLine()).toInt();
    QString message = sockets->readLine();//课编号
    int m = message.toInt();//m是课编号

    QString str1;
    bool flag = 0;
    if (ctype == 1)
        str1 += QString::number(vec_compulsory1[m].stu_num)+"\n";
        //sockets.at(ii)->write((QString::number(vec_compulsory1[m].stu_num)+"\n").toLatin1());
    else
        str1 += QString::number(vec_optional1[m].stu_num)+"\n";
        //sockets.at(ii)->write((QString::number(vec_optional1[m].stu_num)+"\n").toLatin1());
    if (ctype == 1) {
        for (int i = 0; i < t_compulsory.size(); i++)
            if (vec_compulsory1[t_compulsory[i]].num == m)
                flag = 1;
    }
    else {
        for (int i = 0; i < t_optional.size(); i++)
            if (vec_optional1[t_optional[i]].num == m)
                flag = 1;
    }
    if (!flag) {
        throw 4;
    }
    if (ctype == 1) forcom(CStu_data, it, vec_compulsory1[m].data) {
            if (it->grade == -1)
                str1 += QString::fromStdString(it->name)+
                        "成绩尚未输入\t"+"\t"+ ":\n";
            else
                str1 +=QString::fromStdString(it->name)+
                        "当前成绩为\t"+QString::number(it->grade)+"\n";
    }
    else forcom(CStu_data, it, vec_optional1[m].data) {
                            if (it->grade == -1)
                                str1 += QString::fromStdString(it->name)+
                                        "成绩尚未输入\t"+"\t"+ ":\n";
                            else
                                str1 +=QString::fromStdString(it->name)+
                                        "当前成绩为\t"+QString::number(it->grade)+"\n";
                        }

    sockets->write(str1.toUtf8());
    sockets->flush();
}
void C_Teacher::InputGrade(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1){//输入成绩
    int ctype = QString::fromLatin1(sockets->readLine()).toInt();
    QString message = sockets->readLine();//课编号
       int m = message.toInt();//m是课程编号
       bool flag = 0;
       if (ctype == 1) {
           for (int i = 0; i < t_compulsory.size(); i++)
               if (vec_compulsory1[t_compulsory[i]].num == m)
                   flag = 1;
       }
       else {
           for (int i = 0; i < t_optional.size(); i++)
               if (vec_optional1[t_optional[i]].num == m)
                   flag = 1;
       }
       QVector<QString> ss;
       int astu = QString::fromLatin1(sockets->readLine()).toInt();
       for (int i = 0; i < astu; i++) ss.push_back(QString::fromLatin1(sockets->readLine()));
       if (!flag) throw 4;
       int realstu = ctype == 1? vec_compulsory1[m].stu_num:vec_optional1[m].stu_num;


       if (astu != realstu) {
           throw 5;
       }
       int i = 0;
       if (ctype == 1) forcom(CStu_data, it, vec_compulsory1[m].data) {
           QString str = ss[i++];
           if (str == "") continue;
           bool ok = 0;
           int mm = str.toInt(&ok);
           if (!ok) throw 7;
           if (it->flag) throw 6;
           it->grade = mm;
           it->gpa = vec_compulsory1[m].getgpa(mm);
       }
       else forcom(CStu_data, it, vec_optional1[m].data){
           message = sockets->readLine();//读学生成绩
           QString str = ss[i++];
           if (str == "") continue;
           bool ok;
           int mm = str.toInt(&ok);
           if (!ok) throw 7;
           if (it->flag) throw 6;
           it->grade = mm;
           it->gpa = vec_optional1[m].getgpa(mm);
       }
}
void C_Teacher::HandGrade(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1){//提交成绩
    int ctype = QString::fromLatin1(sockets->readLine()).toInt();
    QString message = sockets->readLine();//课编号
    int m = message.toInt();
    bool flag = 0;
    if (ctype == 1) {
        for (int i = 0; i < t_compulsory.size(); i++)
            if (vec_compulsory1[t_compulsory[i]].num == m)
                flag = 1;
    }
    else {
        for (int i = 0; i < t_optional.size(); i++)
            if (vec_optional1[t_optional[i]].num == m)
                flag = 1;
    }

    if (ctype == 1) forcom(CStu_data, it, vec_compulsory1[m].data) {
        if (it->grade < 0) throw -1;
        else if(it->flag ==1)throw -1;
        it->flag = 1;
    }
    else forcom(CStu_data, it, vec_optional1[m].data){
        if (it->grade < 0) throw -1;
        else if(it->flag ==1)throw -1;
        it->flag = 1;
    }
    QString str;
    str = "handsuccess\n";
    sockets->write(str.toLatin1());
    sockets->flush();
}
void C_Teacher::AskSort(QTcpSocket* sockets,vector<C_Compulsory>&vec_compulsory1,vector<C_Optional>&vec_optional1){//选择分类
    int ctype = QString::fromLatin1(sockets->readLine()).toInt();
    QString message = sockets->readLine();//课编号
    QString str;
    int m = message.toInt();
    bool flag = 0;
    if (ctype == 1) {
        for (int i = 0; i < t_compulsory.size(); i++)
            if (vec_compulsory1[t_compulsory[i]].num == m)
                flag = 1;
    }
    else {
        for (int i = 0; i <t_optional.size(); i++)
            if (vec_optional1[t_optional[i]].num == m)
                flag = 1;
    }
    if (!flag) throw 4;
    int y = QString::fromLatin1(sockets->readLine()).toInt();
    str = "\nname\tnumber\t\tgrade\tgpa\n";
    if (ctype == 1) {
        if (y == 1) sort(vec_compulsory1[m].data.begin(), vec_compulsory1[m].data.end(), CompareNumBig);
        else if (y == 2)sort(vec_compulsory1[m].data.begin(), vec_compulsory1[m].data.end(), CompareNumSmall);
        else if (y == 3)sort(vec_compulsory1[m].data.begin(), vec_compulsory1[m].data.end(), CompareGraBig);
        else if (y == 4)sort(vec_compulsory1[m].data.begin(), vec_compulsory1[m].data.end(), CompareGraSmall);
        forcom (CStu_data, it, vec_compulsory1[m].data) {
            str += QString::fromStdString(it->name) + "\t"
                    + QString::fromStdString(it->num) + "\t"
                    + QString::number(it->grade) + "\t" +
                    QString::number(it->gpa) + "\n";
        }
    }
    else {
        if (y == 1) sort(vec_optional1[m].data.begin(), vec_optional1[m].data.end(), CompareNumBig);
        else if (y == 2) sort(vec_optional1[m].data.begin(), vec_optional1[m].data.end(), CompareNumSmall);
        else if (y == 3) sort(vec_optional1[m].data.begin(), vec_optional1[m].data.end(), CompareGraBig);
        else if (y == 4) sort(vec_optional1[m].data.begin(), vec_optional1[m].data.end(), CompareGraSmall);
        forcom (CStu_data, it, vec_optional1[m].data) {
            str += QString::fromStdString(it->name) + "\t"
                    + QString::fromStdString(it->num) + "\t"
                    + QString::number(it->grade) + "\t" +
                    QString::number(it->gpa) + "\n";
        }
    }
    sockets->write(str.toLatin1());
    sockets->flush();
}

bool C_Teacher::CompareNumBig(const CStu_data& a, const CStu_data& b) {
    if (a.num > b.num) return true;
    return false;
}
bool C_Teacher::CompareNumSmall(const CStu_data& a,const CStu_data &b) {
    if (b.num > a.num) return true;
    return false;
}
bool C_Teacher::CompareGraBig(const CStu_data& a, const CStu_data& b) {
    if (a.grade > b.grade) return true;
    return false;
}
bool C_Teacher::CompareGraSmall(const CStu_data& a, const CStu_data& b) {
    if (b.grade > a.grade) return true;
    return false;
}

