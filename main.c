#include "includes.h"

int main(int argc, char* argv[])
{
    srand(GetSeed());

    //Set up user struct
    User user;
    #if (defined (_WIN32) || defined (_WIN64))
    	strcpy(user.username, getenv("USERNAME"));
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
	    strcpy(user.username, getenv("USER"));
    #endif

    //Check if DB exists
    #if (defined (_WIN32) || defined (_WIN64))
        strcpy(user.homepath, getenv("USERPROFILE"));
        strcat(user.homepath, "\\Documents\\passwordmanager\\");
        strcpy(user.dbFile, ".passwords");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        strcpy(user.homepath, getenv("HOME"));
        strcat(user.homepath, "/.passwordmanager/");
        strcpy(user.dbFile, "passwords");
    #endif

    struct stat info;

    if (stat(user.homepath, &info) == 0)
    {
        if (info.st_mode & S_IFDIR)
        {
            //Means everything is fine. No need to do anything here.
            //Truth be told, this block only exists because I don't know how bitwise works.
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
        #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
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
        fopen(user.fullpath, "w");  //Open in write mode to create the file if it does not exist
    }
    int lines = GetNumberOfLines(user.fullpath);

    #if (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        chmod(user.fullpath, S_IRWXU);
    #endif

    DBRow* document = malloc(100 * sizeof(DBRow));
    ReadFromFile(user.fullpath, document);

    //Parse command line args
    if (argc > 1)
    {
        int i;
        for (i = 0; i < argc; i++)
        {
            if ((strcmp(argv[i], "help") == 0) || (strcmp(argv[i], "?") == 0))
            {
                printf("Command line options:\n");
                printf("    help or ?: Display this help screen\n");
                printf("    view or ls: Lists all entries in the local record\n");
                printf("    get <type> <expression>: Returns all table entries which have at least one term matching with the expression\n");
                printf("        <attribute> can be either \"id\", \"username\", \"user\", \"for\", or \"password\"\n");
                printf("        <expression> is a case sensitive string to search for in the database\n");
                printf("            -If <expression> should have spaces in it, put it between double quotes\n");
                return 0;
            }
            else if ((strcmp(argv[i], "view") == 0) || (strcmp(argv[i], "ls") == 0))
            {
                lines = GetNumberOfLines(user.fullpath);
                ReadFromFile(user.fullpath, document);  //Reload db
                View(document, lines);

                return 0;
            }
            else if (strcmp(argv[i], "get") == 0)
            {
                if (argv[i+1] == NULL)
                {
                    printf("Missing parameter: type\n");
                    return -1;
                }
                else
                {
                    if (argv[i+2] == NULL)
                    {
                        printf("Missing parameter: expression\n");
                        return -1;
                    }
                    else
                    {
                        lines = GetNumberOfLines(user.fullpath);
                        ReadFromFile(user.fullpath, document);
                        GetByAttribute(argv[i+1], argv[i+2], document, lines);

                        return 0;
                    }
                }
            }
        }
    }

    //Some useful stuff
    char prompt[5] = ">>> ";        //Looks cool
    char input[50] = {0};           //Gets user input
    char* argbuffer;                //Buffer for splitting
    char* args[10];                 //Stores split user input for deeper parsing
    char looping = 1;               //Tells if the program should keep looping

    //Main program loop
    printf("pwdmanager console by Bad64\n");

    while (looping)
    {
        //Empty buffer strings
        input[0] = '\0';
        //args[0] = '\0';

        //Print the prompt
        printf("%s", prompt);

        //Get user input
        fgets(input, 100, stdin);
        input[strlen(input)-1] = '\0';

        //Split input into arguments
        unsigned char i, j = 0;
        int argcount = 0;
        argbuffer = strtok(input, " ");

        for (i = 0; i < 10; i++)
        {
            if (argbuffer == NULL)
            {
                args[i] = '\0';
            }
            else
            {
                argcount++;
                args[i] = argbuffer;
                argbuffer = strtok(NULL, " ");
            }
        }

        for (i = 0; i < argcount; i++)
        {
            if (i+1 == argcount)
            {
                if (args[i][0] == '"')
                {
                    args[i][strlen(args[i])] = '\0';

                    for (j = 0; j < strlen(args[i]); j++)
                    {
                        if (args[i][j] == '\0')
                        {
                            break;
                        }
                        else
                        {
                            args[i][j] = args[i][j+1];
                        }
                    }
                    args[i][strlen(args[i])-1] = '\0';

                }
            }
            else
            {
                if (args[i][0] == '"')
                {
                    for (j = 0; j < strlen(args[i]); j++)
                    {
                        if (args[i][j] == '\0')
                        {
                            break;
                        }
                        else
                        {
                            args[i][j] = args[i][j+1];
                        }
                    }
                    args[i+1][strlen(args[i+1])-1] = '\0';

                    char* buffer = (char *)malloc(strlen(args[i]));
                    strcpy(buffer, args[i]);

                    sprintf(args[i], "%s %s", buffer, args[i+1]);

                    for (j = i+1; j < argcount; j++)
                    {
                        args[j] = args[j+1];
                    }

                    argcount -= 1;
                    free(buffer);
                }
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
            printf("    add: Adds an entry with user-defined (instead of generated) password to the local records\n");
            printf("    view or ls: Lists all entries in the local record\n");
            printf("    copy <row>: Copies the password field in row <row> to the clipboard (Windows Only)\n");
            printf("    delete <row>: Deletes table entry corresponding to row number\n");
            printf("    get <type> <expression>: Returns all table entries which have at least one term matching with the expression\n");
            printf("    exit: Closes the program\n");
            printf("    help or ?: Displays this help screen\n");
        }
        else if (strcmp(args[0], "about") == 0)
        {
            printf("pwdmanager console v%d.%d by Bad64\n", VERSION_MAJOR, VERSION_MINOR);
            printf("Build dated %s %s with ", __DATE__, __TIME__);

            #if (defined (__GNUC__))
                printf("gcc %s\n", __VERSION__);
            #elif (defined (__clang__))
                printf("clang %s\n", __clang_version__);
            #elif (defined (__EMSCRIPTEN__))
                printf("emscripten (?!) %s\n", __clang_version__);  //You brave soul.
            #endif
            printf("\n");
            printf("This is where I'd put a Patreon link if I had one !\n");
            printf("\n");
        }
        else if (strcmp(args[0], "new") == 0)
        {
            lines = GetNumberOfLines(user.fullpath);
            AppendToFile(user.fullpath, Generate(user, lines), lines);
        }
        else if (strcmp(args[0], "add") == 0)
        {
            lines = GetNumberOfLines(user.fullpath);
            AppendToFile(user.fullpath, Add(user, lines), lines);
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
                printf("Missing parameter: row\n");
            }
            else
            {
                int line;
                sscanf(args[1], "%d", &line);
                DeleteRow(user.fullpath, document, line);
            }
        }
        else if (strcmp(args[0], "get") == 0)
        {
            if (args[1] == NULL)
            {
                printf("Missing parameter: type\n");
                printf("Type \"get help\" or \"get ?\" for additional help\n");
            }
            else
            {
                if ((strcmp(args[1], "help") == 0) || (strcmp(args[1], "?") == 0))
                {
                    printf("Usage: get <attribute> <expression>\n");
                    printf("    <attribute> can be either \"id\", \"username\", \"user\", \"for\", or \"password\"\n");
                    printf("    <expression> is a case sensitive string to search for in the database\n");
                    printf("        -If <expression> should have spaces in it, put it between double quotes\n");
                }
                else if (args[2] == NULL)
                {
                    printf("Missing parameter: expression\n");
                    printf("Type \"get help\" or \"get ?\" for additional help\n");
                }
                else
                {
                    lines = GetNumberOfLines(user.fullpath);
                    ReadFromFile(user.fullpath, document);
                    GetByAttribute(args[1], args[2], document, lines);
                }
            }
        }
        else if (strcmp(args[0], "copy") == 0)
        {
            if (args[1] == NULL)
            {
                printf("Missing parameter: line\n");
            }
            else
            {
                int line;
                sscanf(args[1], "%d", &line);
                line -= 1;

                ReadFromFile(user.fullpath, document);
                CopyToClipboard(document, line);
            }
        }
        else
        {
            printf("Unknown command; type \"help\" or \"?\" for a list of available commands\n");
        }
    }

    return 0;
}
