#ifndef COURSE_H
#define COURSE_H
#define COURSE_NAME_SIZE 20

struct CourseInfo{
    char name[COURSE_NAME_SIZE];
    int score;
    int type;
    CourseInfo *next;
};

class course{
public:
    course();
    ~course();
    int count;
    CourseInfo *head;
    void init();
    bool read(const char *);
    bool save(const char *, CourseInfo *head);
    CourseInfo *serh(CourseInfo *, int, CourseInfo *head);
    bool add(CourseInfo *);
    bool del(CourseInfo *, bool);
};

class course_ac: public course{
public:
    course_ac(){}
    ~course_ac(){}
};

class course_lv: public course{
public:
    course_lv(){}
    ~course_lv(){}
};

class course_sc: public course{
public:
    course_sc(){}
    ~course_sc(){}
};
#endif // COURSE_H
