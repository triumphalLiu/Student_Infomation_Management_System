#ifndef CHOOSE_H
#define CHOOSE_H

struct StuCrsInfo{
    char stu[20];
    char crs[20];
    int grade;
    StuCrsInfo *next;
};

class choose{
public:
    choose();
    ~choose();
    int count;
    StuCrsInfo *head;
    bool read(const char *);
    bool save(const char *);
    StuCrsInfo *serh(StuCrsInfo *, int);
    bool add(StuCrsInfo *);
    bool del(StuCrsInfo *, int);
    bool addgrade(StuCrsInfo *);
    bool delgrade(StuCrsInfo *);
};
#endif // CHOOSE_H
