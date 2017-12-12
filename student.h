#ifndef STUDENT_H
#define STUDENT_H

#define STU_INFO_SIZE 20
struct StuInfo{
    char name[STU_INFO_SIZE];
    char id[STU_INFO_SIZE];
    StuInfo *next;
};

class student{
public:
    student();
    ~student();
    int count;
    StuInfo *head;
    bool read(const char *);
    bool save(const char *);
    StuInfo *serh(StuInfo *, int);
    bool add(StuInfo *);
    bool del(StuInfo *, bool);
    bool chg(StuInfo *, bool);
};

#endif // STUDENT_H
