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
        printf("ERROR: Cannot write to .password file !!\n");
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
                lines++;
        }
        lines+=1;

        fclose(file);
        return lines;
    }
}

void View(struct DBRow *document, int lines)
{
    if (lines-1 <= 0)
    {
        printf("Nothing to read\n");
    }
    else
    {
        int i;
        char header[100];
        header[0] = '\0';

        //Just making a nice header here
        strcat(header, "|ID   ");
        strcat(header, "|USERNAME");
        for (i = 0; i < 15-strlen("USERNAME"); i++)
            strcat(header, " ");
        strcat(header, "|FOR");
        for (i = 0; i < 20-strlen("FOR"); i++)
            strcat(header, " ");
        strcat(header, "|PASSWORD");
        for (i = 0; i < 30-strlen("PASSWORD"); i++)
            strcat(header, " ");
        strcat(header, "|\n");
        printf("%s", header);

        for (i = 0; header[i] != '\n'; i++)
            printf("-");
        printf("\n");

        for (i = 0; i < lines; i++)
        {
            if (document[i].id != 0)
                printf("|%-5d|%-15s|%-20s|%-30s|\n", document[i].id, document[i].login, document[i].purpose, document[i].password);
        }

    }
}

void DeleteRow(char* pathtofile, struct DBRow *document, int line)
{
    line -= 1;
    int lines = GetNumberOfLines(pathtofile);
    int i;

    if (lines == 1)
    {
        printf("Nothing to delete\n");
    }
    else if (line >= lines-1)
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
    if (strcmp(type, "id") == 0)
    {
        int buf;
        sscanf(attr, "%d", &buf);

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
        printf("ERROR: Cannot write to .password file !!\n");
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

void AppendToFile(char* pathtofile, struct DBRow info)
{
    FILE* file = fopen(pathtofile, "a");

    if (file == NULL)
    {
        printf("ERROR: Cannot write to .password file !!\n");
        exit(1);
    }
    else
    {
        fprintf(file, "%d,%s,%s,%s,", info.id, info.login, info.purpose, info.password);
        fprintf(file, "\n");
        fclose(file);
    }
}
