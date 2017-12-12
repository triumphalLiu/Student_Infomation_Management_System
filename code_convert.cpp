/*
 * Refer to : http://blog.csdn.net/denny97104/article/details/45443055
 */
#include <cstdio>
#include <windows.h>
#include <QByteArray>

QByteArray AnsiToUtf8(QByteArray &ansi)
{
    int len;
    QByteArray result;
    //ANSI转UNICODE
    len = MultiByteToWideChar(CP_ACP, NULL, ansi.data(), -1, NULL, 0);
    WCHAR * unicode = new WCHAR[len+1];
    memset(unicode, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, NULL, ansi.data(), -1, unicode, len);
    //UNICODE转utf8
    len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
    char *utf8=new char[len + 1];
    memset(utf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, unicode, -1, utf8, len, NULL,NULL);
    result = utf8;
    delete[] unicode;
    delete[] utf8;
    return result;
}
