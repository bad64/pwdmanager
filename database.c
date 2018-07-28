#include "includes.h"

void PrettyPrint(DBRow* array, int matches)
{
    int i, j = 0;
    int maxlogin, maxpurpose, maxpassword;
    maxlogin = 15;
    maxpurpose = 20;
    maxpassword = 30;

    for (i = 0; i < matches; i++)
    {
        if (strlen(array[i].login) > maxlogin)
            maxlogin = strlen(array[i].login);

        if (strlen(array[i].purpose) > maxpurpose)
            maxpurpose = strlen(array[i].purpose);

        if (strlen(array[i].password) > maxpassword)
            maxpassword = strlen(array[i].password);
    }

    printf("|ID   ");
    printf("|USERNAME");
    for (i = 0; i < maxlogin - strlen("USERNAME"); i++)
        printf(" ");

    printf("|FOR");
    for (i = 0; i < maxpurpose - strlen("FOR"); i++)
        printf(" ");

    printf("|PASSWORD");
    for (i = 0; i < maxpassword - strlen("PASSWORD"); i++)
        printf(" ");

    printf("|\n");

    for (i = 0; i < 5 + maxlogin + maxpurpose + maxpassword + 5; i++)
        printf("-");
    printf("\n");

    for (i = 0; i < matches; i++)
    {
        printf("|%-5d", array[i].id);

        printf("|%s", array[i].login);
        for (j = 0; j < maxlogin - strlen(array[i].login); j++)
            printf(" ");

        printf("|%s", array[i].purpose);
        for (j = 0; j < maxpurpose - strlen(array[i].purpose); j++)
            printf(" ");

        printf("|%s", array[i].password);
        for (j = 0; j < maxpassword - strlen(array[i].password); j++)
            printf(" ");

        printf("|\n");
    }
}

void ReadFromFile(char* pathtofile, DBRow *document)
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

        DBRow rowbuffer;

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

void View(DBRow *document, int lines)
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

void DeleteRow(char* pathtofile, DBRow *document, int line)
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
            DBRow buffer = document[i];
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

void GetByAttribute(char *type, char *attr, DBRow *document, int lines)
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

        if ((buf > lines-1) || (buf < 0))
        {
            printf("No matching records found\n");
        }
        else
        {
            int maxlogin, maxpurpose, maxpassword;
            maxlogin = 15;
            maxpurpose = 20;
            maxpassword = 30;

            if (strlen(document[buf].login) > maxlogin)
                maxlogin = strlen(document[buf].login);

            if (strlen(document[buf].purpose) > maxpurpose)
                maxpurpose = strlen(document[buf].purpose);

            if (strlen(document[buf].password) > maxpassword)
                maxpassword = strlen(document[buf].password);

            printf("|ID   ");
            printf("|USERNAME");
            for (i = 0; i < maxlogin - strlen("USERNAME"); i++)
                printf(" ");

            printf("|FOR");
            for (i = 0; i < maxpurpose - strlen("FOR"); i++)
                printf(" ");

            printf("|PASSWORD");
            for (i = 0; i < maxpassword - strlen("PASSWORD"); i++)
                printf(" ");

            printf("|\n");

            for (i = 0; i < 5 + maxlogin + maxpurpose + maxpassword + 5; i++)
                printf("-");
            printf("\n");

            printf("|%-5d", document[buf].id);

            printf("|%s", document[buf].login);
            if (strlen(document[buf].login) < maxlogin)
            {
                for (i = 0; i < maxlogin-strlen(document[buf].login); i++)
                printf(" ");
            }

            printf("|%s", document[buf].purpose);
            if (strlen(document[buf].purpose) < maxpurpose)
            {
                for (i = 0; i < maxpurpose-strlen(document[buf].purpose); i++)
                printf(" ");
            }

            printf("|%s", document[buf].password);
            if (strlen(document[buf].password) < maxpassword)
            {
                for (i = 0; i < maxpassword-strlen(document[buf].password); i++)
                printf(" ");
            }
            printf("|\n");
        }
    }
    else if ((strcmp(type, "username") == 0) || (strcmp(type, "user") == 0))
    {
        int j = 0;
        int matches = 0;

        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].login, attr) == 0)
            {
                matches++;
            }
        }

        if (matches == 0)
        {
            printf("No matching records found\n");
        }
        else
        {
            DBRow* results = malloc(sizeof(DBRow) * matches);

            for (i = 0; i <= lines; i++)
            {
                if (strcmp(document[i].login, attr) == 0)
                {
                    results[j] = document[i];
                    j++;
                }
            }

            PrettyPrint(results, matches);

            free(results);
        }
    }
    else if (strcmp(type, "for") == 0)
    {
        int j = 0;
        int matches = 0;

        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].purpose, attr) == 0)
            {
                matches++;
            }
        }

        if (matches == 0)
        {
            printf("No matching records found\n");
        }
        else
        {
            DBRow* results = malloc(sizeof(DBRow) * matches);

            for (i = 0; i <= lines; i++)
            {
                if (strcmp(document[i].purpose, attr) == 0)
                {
                    results[j] = document[i];
                    j++;
                }
            }

            PrettyPrint(results, matches);

            free(results);
        }
    }
    else if (strcmp(type, "password") == 0)
    {
        int j = 0;
        int matches = 0;

        for (i = 0; i <= lines; i++)
        {
            if (strcmp(document[i].password, attr) == 0)
            {
                matches++;
            }
        }

        if (matches == 0)
        {
            printf("No matching records found\n");
        }
        else
        {
            DBRow* results = malloc(sizeof(DBRow) * matches);

            for (i = 0; i <= lines; i++)
            {
                if (strcmp(document[i].password, attr) == 0)
                {
                    results[j] = document[i];
                    j++;
                }
            }

            PrettyPrint(results, matches);

            free(results);
        }
    }
    else
    {
        printf("Unrecognized format\n");
    }
}

void WriteToFile(char* pathtofile, DBRow *document, int lines)
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

void AppendToFile(char* pathtofile, DBRow info, int lines)
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
