#include "includes.h"

void ReadFromFile(char* pathtofile, struct DBRow *document)
{
    FILE* file = fopen(pathtofile, "r");

    if (file == NULL)
    {
        printf("ERROR: Cannot read from .password file !!\n");
        exit(1);
    }
    else
    {
        char buffer[350];
        int index = 0, col = 0;
        char* ptr;

        struct DBRow rowbuffer;

        while (fgets(buffer, 350, file) != NULL)
        {
            col = 0;
            ptr = strtok(buffer, ",");
            while (ptr != NULL)
            {
                switch (col)
                {
                case 0:
                    rowbuffer.id = atoi(ptr);
                    break;
                case 1:
                    strcpy(rowbuffer.login, ptr);
                    break;
                case 2:
                    strcpy(rowbuffer.purpose, ptr);
                    break;
                case 3:
                    strcpy(rowbuffer.password, ptr);
                    break;
                }
                col++;

                ptr = strtok(NULL, ",");
            }

            document[index] = rowbuffer;
            index++;
        }
        fclose(file);
    }

}

int GetNumberOfLines(char* pathtofile)
{
    FILE* file = fopen(pathtofile, "r");

    if (file == NULL)
    {
        printf("ERROR: Cannot read number of lines from .password file !!\n");
        exit(1);
    }
    else
    {
        char buffer;
        int lines = 0;
        while(!feof(file))
        {
            buffer = fgetc(file);
            if (buffer == '\n')
            {
                lines++;
            }
        }

        fclose(file);
        return lines;
    }
}

void View(struct DBRow *document, int lines)
{
    //printf("%d\n", lines);
    if (lines-1 < 0)
    {
        printf("Nothing to read\n");
    }
    else
    {
        int i, j;
        char header[200];
        header[0] = '\0';
        int maxlogin, maxpurpose, maxpassword;
        maxlogin = 15;
        maxpurpose = 20;
        maxpassword = 30;

        //Determining the longest fields
        for (i = 0; i < lines; i++)
        {
            if (strlen(document[i].login) > maxlogin)
                maxlogin = strlen(document[i].login);

            if (strlen(document[i].purpose) > maxpurpose)
                maxpurpose = strlen(document[i].purpose);

            if (strlen(document[i].password) > maxpassword)
                maxpassword = strlen(document[i].password);
        }

        //Just making a nice header here
        strcat(header, "|ID   ");
        strcat(header, "|USERNAME");
        for (i = 0; i < maxlogin-strlen("USERNAME"); i++)
            strcat(header, " ");
        strcat(header, "|FOR");
        for (i = 0; i < maxpurpose-strlen("FOR"); i++)
            strcat(header, " ");
        strcat(header, "|PASSWORD");
        for (i = 0; i < maxpassword-strlen("PASSWORD"); i++)
            strcat(header, " ");
        strcat(header, "|\n");
        printf("%s", header);

        for (i = 0; header[i] != '\n'; i++)
            printf("-");
        printf("\n");

        for (i = 0; i < lines; i++)
        {
            if (document[i].id != 0)
            {
                printf("|%-5d", document[i].id);

                printf("|%s", document[i].login);
                for (j = 0; j < maxlogin - strlen(document[i].login); j++)
                    printf(" ");

                printf("|%s", document[i].purpose);
                for (j = 0; j < maxpurpose - strlen(document[i].purpose); j++)
                    printf(" ");

                printf("|%s", document[i].password);
                for (j = 0; j < maxpassword - strlen(document[i].password); j++)
                    printf(" ");

                printf("|\n");
            }
        }

    }
}

void DeleteRow(char* pathtofile, struct DBRow *document, int line)
{
    line -= 1;  //Lines are 0-based, the 1st line has index 0, etc, you get the idea
    int lines = GetNumberOfLines(pathtofile);
    lines++;
    int i;

    if (lines == 1)
    {
        printf("Nothing to delete\n");
    }
    else if ((line >= lines-1) || (line < 0))
    {
        printf("Invalid index\n");
    }
    else
    {
        for (i = 0; i < lines; i++)
        {
            if (i == line)
            {
                document[line].id = 0;
                strcpy(document[line].login, "");
                strcpy(document[line].purpose, "");
                strcpy(document[line].password, "");
            }
        }

        for (i = line; i < lines; i++)
        {
            struct DBRow buffer = document[i];
            document[i] = document[i+1];
            document[i].id = i;
            document[i+1] = buffer;
        }

        for (i = 0; i < lines; i++)
        {
            document[i].id = i+1;
        }

        for (i = 0; i < 100; i++)
        {
            if (strcmp(document[i].login, "") == 0)
            {
                document[i].id = 0;
            }
        }

        if (lines-2 != 0)
            View(document, lines);
        else
            printf("Database is now empty\n");

        WriteToFile(pathtofile, document, lines-2);
        ReadFromFile(pathtofile, document);
    }
}

void GetByAttribute(char *type, char *attr, struct DBRow *document, int lines)
{
    int i;

    for (i = 0; i < strlen(attr); i++)
    {
        if (attr[i] == '+')
            attr[i] = ' ';
    }

    if (strcmp(type, "id") == 0)
    {
        int buf;
        sscanf(attr, "%d", &buf);
        buf -= 1;

        printf("|%-5d|%-15s|%-20s|%-30s|\n", document[buf].id, document[buf].login, document[buf].purpose, document[buf].password);
    }
    else if (strcmp(type, "username") == 0)
    {
        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].login, attr) == 0)
                printf("|%-5d|%-15s|%-20s|%-30s|\n", document[i].id, document[i].login, document[i].purpose, document[i].password);
        }
    }
    else if (strcmp(type, "for") == 0)
    {
        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].purpose, attr) == 0)
                printf("|%-5d|%-15s|%-20s|%-30s|\n", document[i].id, document[i].login, document[i].purpose, document[i].password);
        }
    }
    else if (strcmp(type, "password") == 0)
    {
        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].password, attr) == 0)
                printf("|%-5d|%-15s|%-20s|%-30s|\n", document[i].id, document[i].login, document[i].purpose, document[i].password);
        }
    }
    else
    {
        printf("Unrecognized format\n");
    }
}

void WriteToFile(char* pathtofile, struct DBRow *document, int lines)
{
    FILE* file = fopen(pathtofile, "w");

    if (file == NULL)
    {
        printf("ERROR: Cannot write to .password file !!\n");   //That should not happen ?
        exit(1);
    }
    else
    {
        int i;
        for (i = 0; i < lines; i++)
        {
            fprintf(file, "%d,%s,%s,%s,\n", document[i].id, document[i].login, document[i].purpose, document[i].password);
        }
        fclose(file);
    }
}

void AppendToFile(char* pathtofile, struct DBRow info, int lines)
{
    FILE* file = fopen(pathtofile, "a");

    if (file == NULL)
    {
        printf("ERROR: Cannot write to .password file !!\n");
        exit(1);
    }
    else
    {
        fprintf(file, "%d,%s,%s,%s,\n", info.id, info.login, info.purpose, info.password);
        fclose(file);
    }
}
