#include "course.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
course::course()
{
    course::count = 0;
    course::head = NULL;
}

course::~course()
{

}

bool course::read(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return 0;
    char ch;
    int i = 0;
    int mode = 0;
    char score_temp[20] = {'\0'};
    char type_temp;
    CourseInfo *p = new CourseInfo;
    p->next = NULL;
    while((ch = fgetc(fp)) != EOF)
    {
        if(mode == 0 && ch != ' ' && ch != '\n')
            p->name[i++]=ch;
        else if(mode == 1 && ch != ' ' && ch != '\n')
            score_temp[i++] = ch;
        else if(mode == 2 && ch != ' ' && ch != '\n')
            type_temp = ch;
        else if(ch == '\n')
        {
            score_temp[i] = '\0';
            p->score = atoi(score_temp);
            p->type = type_temp - '0';
            i = 0;
            mode = 0;
            CourseInfo *point;
            if((point = course::serh(p, 0, course::head)) != NULL)
            {
                strcpy(point->name, p->name);
            }
            else
            {
                if(course::head != NULL)
                p->next = course::head;
                course::head = p;
                ++course::count;
            }
            p = new CourseInfo;
            p->next = NULL;
        }
        else if(ch == ' ')
        {
            if(mode == 0)
            {
                p->name[i] = '\0';
                i = 0;
            }
            mode++;
        }
    }
    fclose(fp);
    return 1;
}

bool course::save(const char *filename, CourseInfo *head)
{
    CourseInfo *p = head;
    FILE *fp = fopen(filename, "w");
    while(p != NULL)
    {
        fprintf(fp, p->name);
        fprintf(fp, " ");
        char buffer[20];
        itoa(p->score, buffer, 10);
        fprintf(fp, buffer);
        fprintf(fp, " ");
        itoa(p->type, buffer, 10);
        fprintf(fp, buffer);
        fprintf(fp, "\n");
        p = p->next;
    }
    fclose(fp);
    return 1;
}

bool course::add(CourseInfo *info)
{
    if(!(course::serh(info, 0, head) == NULL))
        return 0;
    course::count += 1;
    info->next = course::head;
    course::head = info;
    return 1;
}

bool course::del(CourseInfo *info, bool type)
{
    CourseInfo *point;
    CourseInfo *p = course::head;
    if((point = course::serh(info, type, course::head)) == NULL)
        return 0;
    course::count -= 1;
    if(p == point)
    {
        course::head = course::head->next;
        return 1;
    }
    while(p->next != NULL)
        if(p->next == point)
            break;
        else
            p = p->next;
    p->next = p->next->next;
    return 1;
}

CourseInfo *course::serh(CourseInfo *info, int type, CourseInfo *head)
{
    CourseInfo *p = head;
    while(p != NULL)
    {
        if(type == 0 && strcmp(info->name, p->name) == 0)
            return p;
        else if(type == 1 && info->score == p->score)
            return p;
        p = p->next;
    }
    return NULL;
}
