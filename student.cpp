#include "student.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
student::student()
{
    student::count = 0;
    student::head = NULL;
}

student::~student()
{

}

bool student::read(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return 0;
    char ch;
    int i = 0;
    bool mode = 0;
    StuInfo *p = new StuInfo;
    p->next = NULL;
    while((ch = fgetc(fp)) != EOF)
    {
        if(mode == 0 && ch != ' ' && ch != '\n')
            p->name[i++]=ch;
        else if(mode == 1 && ch != ' ' && ch != '\n')
            p->id[i++] = ch;
        else if(ch == '\n')
        {
            p->id[i] = '\0';
            i = 0;
            mode = 0;
            StuInfo *point;
            if((point = student::serh(p, 1)) != NULL)
            {
                strcpy(point->name, p->name);
            }
            else
            {
                if(student::head != NULL)
                p->next = student::head;
                student::head = p;
                ++student::count;
            }
            p = new StuInfo;
            p->next = NULL;
        }
        else if(ch == ' ')
        {
            p->name[i] = '\0';
            i = 0;
            mode = 1;
        }
    }
    fclose(fp);
    return 1;
}

/*
//when read Chinese from a unicode file
bool student::read(const char *filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        char *info = line.data();

        if(strcmp("", info) == 0)
            break;
        int flag = 0;
        char name[20], id[20];
        for(int i = 0; i < 20; ++i)
        {
            name[i] = '\0';
            id[i] = '\0';
        }
        for(int i = 0; i < (int)strlen(info); ++i)
            if(flag == 0 && info[i] != ' ' && info[i] != '\n')
                name[i] = info[i];
            else if(flag != 0 && info[i] != ' ' && info[i] != '\n')
                id[i - flag] = info[i];
            else if(info[i] == ' ')
                flag = i+1;
            else if(info[i] == '\n')
                break;
        StuInfo *p = new StuInfo;
        p->next = NULL;
        strcpy(p->id, id);
        strcpy(p->name, name);
        StuInfo *point;
        if((point = student::serh(p, 1)) != NULL)
        {
            strcpy(point->name, p->name);
        }
        else
        {
            if(student::head != NULL)
                p->next = head;
            head = p;
            ++student::count;
        }
    }
    file.close();
    return 1;
}
*/

bool student::save(const char *filename)
{
    StuInfo *p = student::head;
    FILE *fp = fopen(filename, "w");
    while(p != NULL)
    {
        fprintf(fp, p->name);
        fprintf(fp, " ");
        fprintf(fp, p->id);
        fprintf(fp, "\n");
        p = p->next;
    }
    fclose(fp);
    return 1;
}

bool student::add(StuInfo *info)
{
    if(!(student::serh(info, 1) == NULL))
        return 0;
    student::count += 1;
    info->next = student::head;
    student::head = info;
    return 1;
}

bool student::del(StuInfo *info, bool type)
{
    StuInfo *point;
    StuInfo *p = student::head;
    if((point = student::serh(info, type)) == NULL)
        return 0;
    student::count -= 1;
    if(p == point)
    {
        student::head = student::head->next;
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

bool student::chg(StuInfo *info, bool type)
{
    StuInfo *point;
    if((point = student::serh(info, 1)) == NULL)
        return 0;
    if(type == 0)
        strcpy(point->name, info->name);
    else if(type == 1)
        strcpy(point->id, info->id);
    return 1;
}

StuInfo *student::serh(StuInfo *info, int type)
{
    StuInfo *p = student::head;
    while(p != NULL)
    {
        if(type == 0 && strcmp(info->name, p->name) == 0)
            return p;
        else if(type == 1 && strcmp(info->id, p->id) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}
