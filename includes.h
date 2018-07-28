#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 13

#if (defined (_WIN32) || defined (_WIN64))
    #include <conio.h>
    #include <io.h>
    #include <windows.h>
#elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
    #include <unistd.h>
#endif

typedef struct User User;
struct User
{
    char username[100];
    char homepath[100];
    char fullpath[110];
    char dbFile[10];
};

typedef struct DBRow DBRow;
struct DBRow
{
    unsigned int id;
    char login[100];
    char purpose[100];
    char password[100];
};

//seed.c
int randint(int minvalue, int maxvalue);
unsigned long GetSeed();

//random.c
DBRow Generate(User user, int lines);
DBRow Add(User user, int lines);

//database.c
void PrettyPrint(DBRow* array, int matches);
void ReadFromFile(char* pathtofile, DBRow *document);
int GetNumberOfLines(char* pathtofile);
void View(DBRow *document, int lines);
void DeleteRow(char* pathtofile, DBRow *document, int line);
void GetByAttribute(char *type, char *attr, DBRow *document, int lines);
void WriteToFile(char* pathtofile, DBRow *document, int lines);
void AppendToFile(char* pathtofile, DBRow info, int lines);

//clipboard.c
void CopyToClipboard(DBRow* document, int line);

#endif // INCLUDES_H
