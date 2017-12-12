#include "choose.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
choose::choose()
{
    choose::count = 0;
    choose::head = NULL;
}

choose::~choose()
{

}

bool choose::read(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return 0;
    char ch;
    int i = 0;
    int mode = 0;
    StuCrsInfo *p = new StuCrsInfo;
    p->next = NULL;
    char grade_temp[20];
    while((ch = fgetc(fp)) != EOF)
    {
        if(mode == 0 && ch != ' ' && ch != '\n')
            p->stu[i++]=ch;
        else if(mode == 1 && ch != ' ' && ch != '\n')
            p->crs[i++] = ch;
        else if(mode == 2 && ch != ' ' && ch != '\n')
            grade_temp[i++] = ch;
        else if(ch == '\n')
        {
            grade_temp[i] = '\0';
            i = 0;
            mode = 0;
            p->grade = atoi(grade_temp);
            if(choose::serh(p, 2) == NULL)
            {
                if(choose::head != NULL)
                p->next = choose::head;
                choose::head = p;
                ++choose::count;
            }
            p = new StuCrsInfo;
            p->next = NULL;
        }
        else if(ch == ' ')
        {
            if(mode == 0)
            {
                p->stu[i] = '\0';
                i = 0;
            }
            else if(mode == 1)
            {
                p->crs[i] = '\0';
                i = 0;
            }
            mode++;
        }
    }
    fclose(fp);
    return 1;
}

bool choose::save(const char *filename)
{
    StuCrsInfo *p = choose::head;
    FILE *fp = fopen(filename, "w");
    while(p != NULL)
    {
        fprintf(fp, p->stu);
        fprintf(fp, " ");
        fprintf(fp, p->crs);
        fprintf(fp, " ");
        char buffer[20];
        itoa(p->grade, buffer, 10);
        fprintf(fp, buffer);
        fprintf(fp, "\n");
        p = p->next;
    }
    fclose(fp);
    return 1;
}

bool choose::add(StuCrsInfo *info)
{
    if(choose::serh(info, 2) != NULL)
        return 0;
    choose::count += 1;
    info->next = choose::head;
    choose::head = info;
    return 1;
}

bool choose::del(StuCrsInfo *info, int type)
{
    StuCrsInfo *point;
    StuCrsInfo *p = choose::head;
    if((point = choose::serh(info, type)) == NULL)
        return 0;
    choose::count -= 1;
    if(p == point)
    {
        choose::head = choose::head->next;
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

StuCrsInfo *choose::serh(StuCrsInfo *info, int type)
{
    StuCrsInfo *p = choose::head;
    while(p != NULL)
    {
        if(type == 0 && strcmp(info->stu, p->stu) == 0)
            return p;
        else if(type == 1 && strcmp(info->crs, p->crs) == 0)
            return p;
        else if(type == 2 && strcmp(info->crs, p->crs) == 0 && strcmp(info->stu, p->stu) == 0)
            return p;
        else if(type == 3 && strcmp(info->stu, p->stu) == 0)
        {
            StuCrsInfo *temp = new StuCrsInfo;
            strcpy(temp->crs, p->crs);
            strcpy(temp->stu, p->stu);
            temp->grade = p->grade;
            temp->next = info;
            info = temp;
        }
        else if(type == 4 && strcmp(info->crs, p->crs) == 0)
        {
            StuCrsInfo *temp = new StuCrsInfo;
            strcpy(temp->crs, p->crs);
            strcpy(temp->stu, p->stu);
            temp->grade = p->grade;
            temp->next = info;
            info = temp;
        }
        p = p->next;
    }
    return (type < 3) ? NULL : info;
}

bool choose::addgrade(StuCrsInfo *p)
{
    StuCrsInfo *node = choose::serh(p, 2);
    if(node == NULL)
        return 0;
    node->grade = p->grade;
    return 1;
}

bool choose::delgrade(StuCrsInfo *p)
{
    StuCrsInfo *node = choose::serh(p, 2);
    if(node == NULL)
        return 0;
    node->grade = -1;
    return 1;
}
