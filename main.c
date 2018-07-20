#include "includes.h"

int main()
{
    srand(GetSeed());

    //Set up user struct
    User user;
    strcpy(user.username, getenv("USERNAME"));

    //Check if DB exists
    #if (defined (_WIN32) || defined (_WIN64))
        strcpy(user.homepath, getenv("USERPROFILE"));
        strcat(user.homepath, "\\Documents\\passwordmanager\\");
        strcpy(user.dbFile, ".passwords");
    #elif (defined (LINUX) || defined (__linux__))
        strcpy(user.homepath, getenv("HOME"));
        strcat(user.homepath, "/.passwordmanager/");
        strcpy(user.dbFile, "passwords");
    #endif

    struct stat info;

    if (stat(user.homepath, &info) == 0)
    {
        if (info.st_mode & S_IFDIR)
        {

        }
        else
        {
            printf("%s does not appear to be a folder ! Exiting.\n", user.homepath);
            return 1;
        }
    }
    else
    {
        #if (defined (_WIN32) || defined (_WIN64))
            CreateDirectoryA(user.homepath, NULL);
        #elif (defined (LINUX) || defined (__linux__))
            mkdir(user.homepath, S_IRWXU);
            chmod(user.homepath, S_IRWXU);
        #endif
    }

    user.fullpath[0] = '\0';
    strcat(user.fullpath, user.homepath);
    strcat(user.fullpath, user.dbFile);

    //Connect to DB
    FILE* db = fopen(user.fullpath, "r");   //That actually only checks whether the file exists or not, which we handle below

    if (db == NULL)
    {
        fclose(db);
        fopen(user.fullpath, "w");
        fclose(db);
    }
    int lines = GetNumberOfLines(user.fullpath);

    struct DBRow* document = malloc(100 * sizeof(struct DBRow));
    ReadFromFile(user.fullpath, document);

    //Some useful stuff
    char prompt[5] = ">>> ";        //Looks cool
    char input[50] = {0};           //Gets user input
    char* argbuffer;                //Buffer for splitting
    char* args[10];                 //Stores split user input for deeper parsing
    char looping = 1;               //Tells if the program should keep looping

    //Main program loop
    printf("pwdmanager console v0.9 by Bad64\n");

    while (looping)
    {
        //Empty buffer strings
        input[0] = '\0';
        args[0] = '\0';

        //Print the prompt
        printf("%s", prompt);

        //Get user input
        fgets(input, 100, stdin);
        input[strlen(input)-1] = '\0';

        //Split input into arguments
        argbuffer = strtok(input, " ");
        unsigned char i = 0;

        for (i = 0; i < 10; i++)
        {
            if (argbuffer == NULL)
            {
                args[i] = '\0';
            }
            else
            {
                args[i] = argbuffer;
                argbuffer = strtok(NULL, " ");
            }
        }

        if (strcmp(input, "") == 0) //Tests for an empty string
        {
            continue;   //Just re-print the prompt in this case
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            looping = 0;
        }
        else if ((strcmp(args[0], "help") == 0) || (strcmp(args[0], "?") == 0))
        {
            printf("Available commands:\n");
            printf("    about: Displays technical information about the software\n");
            printf("    new: Generates a new entry in the local records\n");
            printf("    view or ls: Lists all entries in the local record\n");
            printf("    delete <row>: Deletes table entry corresponding to row number\n");
            printf("    get <type> <expression>: Returns all table entries which have at least one term matching with the expression\n");
            printf("    exit: Closes the program\n");
            printf("    help or ?: Displays this help screen\n");
        }
        else if (strcmp(args[0], "about") == 0)
        {
            printf("pwdmanager console v0.9 by Bad64\n");
            printf("Build dated %s %s with gcc %s\n", __DATE__, __TIME__, __VERSION__);
        }
        else if (strcmp(args[0], "new") == 0)
        {
            lines = GetNumberOfLines(user.fullpath);
            AppendToFile(user.fullpath, Generate(user, lines));
        }
        else if ((strcmp(args[0], "view") == 0) || (strcmp(args[0], "ls") == 0))
        {
            lines = GetNumberOfLines(user.fullpath);
            ReadFromFile(user.fullpath, document);  //Reload db
            View(document, lines);
        }
        //deeper parsing
        else if (strcmp(args[0], "delete") == 0)
        {
            if (args[1] == NULL)
            {
                //printf("Missing parameter: row\n");
                printf("Feature not implemented yet\n");
            }
            else
            {
                printf("Feature not implemented yet\n");
            }
        }
        else if (strcmp(args[0], "get") == 0)
        {
            if (args[1] == NULL)
            {
                printf("Missing parameter: type\n");
                printf("Type \"get help\" or \"get ?\" for additional help\n");
            }
            else if (args[2] == NULL)
            {
                printf("Missing parameter: expression\n");
                printf("Type \"get help\" or \"get ?\" for additional help\n");
            }
            else if ((strcmp(args[1], "help") == 0) || (strcmp(args[1], "help") == 0))
            {
                printf("Usage: get <attribute> <expression>\n");
                printf("    <attribute> can be either \"username\" or \"for\"\n");
                printf("    <expression> is a case sensitive string to search for in the database\n");
            }
            else
            {
                lines = GetNumberOfLines(user.fullpath);
                ReadFromFile(user.fullpath, document);
                GetByAttribute(args[1], args[2], document, lines);
            }
        }
        else
        {
            printf("Unknown command; type \"help\" or \"?\" for a list of available commands\n");
        }
    }

    return 0;
}
