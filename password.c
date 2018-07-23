#include "includes.h"

struct DBRow Generate(User user, int lines)
{
    char lowercase[27] = "abcdefghijklmnopqrstuvwxyz\0";
    char uppercase[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
    char numeric[11] = "0123456789\0";
    char special[9] = "@&§!€ù$*\0";

    char requireUppercase = -1, requireNumeric = -1, requireSpecial = -1;
    char responsebuffer[50] = {0};
    char username[50] = {0};
    char purpose[50] = {0};
    char password[100] = {0};
    int length = 10;
    int i;

    //Username ?
    printf("Please enter the username that will be associated with this password: [%s] ", user.username);
    fgets(responsebuffer, 50, stdin);

    if (responsebuffer[0] == 10)
        strcpy(username, user.username);
    else
        strcpy(username, responsebuffer);

    for (i = 0; i < strlen(username); i++)
    {
        if (username[i] == '\n')
        {
            username[i] = '\0';
        }
    }

    responsebuffer[0] = '\0';

    //What will this password be used for ?
    do
    {
        printf("Please enter the entity associated with the credentials pair that will be created (e.g. website, application...): ");
        fgets(responsebuffer, 50, stdin);
    }
    while(responsebuffer[0] == 10);

    for (i = 0; i < strlen(responsebuffer); i++)
    {
        if (responsebuffer[i] == '\n')
        {
            responsebuffer[i] = '\0';
        }
    }

    strcpy(purpose, responsebuffer);
    purpose[strlen(purpose)] = '\0';
    responsebuffer[0] = '\0';

    //How long ?
    printf("How long should the password be ? [10] ");
    fgets(responsebuffer, 50, stdin);

    if (responsebuffer[0] == 10)
        length = 10;
    else
        sscanf(responsebuffer, "%d", &length);

    //Uppercase requirement
    printf("Does your password require uppercase letters ? [Y/n] ");
    do
    {
        fgets(responsebuffer, 10, stdin);

        if ((responsebuffer[0] == 10) || (responsebuffer[0] == 'Y') || (responsebuffer[0] == 'y'))
        {
            requireUppercase = 1;
        }
        else if ((responsebuffer[0] == 'N') || (responsebuffer[0] == 'n'))
        {
            requireUppercase = 0;
        }
        else
        {
            printf("Please answer with Y or N\n");
        }
        responsebuffer[0] = '\0';
    }
    while (requireUppercase == -1);


    //Numeric requirement
    printf("Does your password require numerals ? [Y/n] ");
    do
    {
        fgets(responsebuffer, 10, stdin);

        if ((responsebuffer[0] == 10) || (responsebuffer[0] == 'Y') || (responsebuffer[0] == 'y'))
        {
            requireNumeric = 1;
        }
        else if ((responsebuffer[0] == 'N') || (responsebuffer[0] == 'n'))
        {
            requireNumeric = 0;
        }
        else
        {
            printf("Please answer with Y or N\n");
        }
        responsebuffer[0] = '\0';
    }
    while (requireUppercase == -1);

    //Special requirement
    printf("Does your password require special characters ? [y/N] ");
    do
    {
        fgets(responsebuffer, 10, stdin);

        if ((responsebuffer[0] == 'Y') || (responsebuffer[0] == 'y'))
        {
            requireSpecial = 1;
        }
        else if ((responsebuffer[0] == 10) || (responsebuffer[0] == 'N') || (responsebuffer[0] == 'n'))
        {
            requireSpecial = 0;
        }
        else
        {
            printf("Please answer with Y or N\n");
        }
        responsebuffer[0] = '\0';
    }
    while (requireUppercase == -1);

    //Now we actually generate the password
    for (i = 0; i < length; i++)
    {
        unsigned char listindex = randint(0, 3);

        switch (listindex)
        {
        case 0:
            password[i] = lowercase[randint(0, strlen(lowercase)-1)];
            break;
        case 1:
            if (requireUppercase)
                password[i] = uppercase[randint(0, strlen(uppercase)-1)];
            else
                password[i] = lowercase[randint(0, strlen(lowercase)-1)];
            break;
        case 2:
            if (requireNumeric)
                password[i] = numeric[randint(0, strlen(numeric)-1)];
            else
                password[i] = lowercase[randint(0, strlen(lowercase)-1)];
            break;
        case 3:
            if (requireSpecial)
                password[i] = special[randint(0, strlen(special)-1)];
            else
                password[i] = lowercase[randint(0, strlen(lowercase)-1)];
            break;
        }
    }
    password[i] = '\0';

    printf("User %s will have password %s for %s\n", username, password, purpose);

    struct DBRow info;
    info.id = lines+1;
    printf("Stored on line %d\n", info.id);
    strcpy(info.login, username);
    strcpy(info.purpose, purpose);

    strcpy(info.password, password);

    return info;
}

struct DBRow Add(User user, int lines)
{
    char responsebuffer[50] = {0};
    char username[50] = {0};
    char purpose[50] = {0};
    char password[100] = {0};
    int i;

    //Username ?
    printf("Please enter the username that will be associated with this password: [%s] ", user.username);
    fgets(responsebuffer, 50, stdin);

    if (responsebuffer[0] == 10)
        strcpy(username, user.username);
    else
        strcpy(username, responsebuffer);

    for (i = 0; i < strlen(username); i++)
    {
        if (username[i] == '\n')
        {
            username[i] = '\0';
        }
    }

    responsebuffer[0] = '\0';

    //What will this password be used for ?
    do
    {
        printf("Please enter the entity associated with the credentials pair that will be created (e.g. website, application...): ");
        fgets(responsebuffer, 50, stdin);
    }
    while(responsebuffer[0] == 10);

    for (i = 0; i < strlen(responsebuffer); i++)
    {
        if (responsebuffer[i] == '\n')
        {
            responsebuffer[i] = '\0';
        }
    }

    strcpy(purpose, responsebuffer);
    purpose[strlen(purpose)] = '\0';
    responsebuffer[0] = '\0';

    //The actual password
    do
    {
        printf("Please enter the password to store: ");
        fgets(responsebuffer, 50, stdin);
    }
    while(responsebuffer[0] == 10);

    for (i = 0; i < strlen(responsebuffer); i++)
    {
        if (responsebuffer[i] == '\n')
        {
            responsebuffer[i] = '\0';
        }
    }

    strcpy(password, responsebuffer);
    password[strlen(password)] = '\0';
    responsebuffer[0] = '\0';

    printf("User %s will have password %s for %s\n", username, password, purpose);

    struct DBRow info;
    info.id = lines+1;
    printf("Stored on line %d\n", info.id);
    strcpy(info.login, username);
    strcpy(info.purpose, purpose);

    strcpy(info.password, password);

    return info;
}
