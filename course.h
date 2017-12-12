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
    bool read(const char *);
    bool save(const char *, CourseInfo *head);
    CourseInfo *serh(CourseInfo *, int, CourseInfo *head);
    bool add(CourseInfo *);
    bool del(CourseInfo *, bool);
};
#endif // COURSE_H
