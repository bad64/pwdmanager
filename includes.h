#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define VERSION 1.1

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
struct DBRow Generate(User user, int lines);

//database.c
void ReadFromFile(char* pathtofile, struct DBRow *document);
int GetNumberOfLines(char* pathtofile);
void View(struct DBRow *document, int lines);
void DeleteRow(char* pathtofile, struct DBRow *document, int line);
void GetByAttribute(char *type, char *attr, struct DBRow *document, int lines);
void WriteToFile(char* pathtofile, struct DBRow *document, int lines);
void AppendToFile(char* pathtofile, struct DBRow info);

#endif // INCLUDES_H
