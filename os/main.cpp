#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
using namespace std;
#define ENV_SEP '='
extern char **environ;

bool file_exists(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void removeSpaces(char *str)
{
    int count = 0;
    int i = 0;
    while (str[i])
    {
        if (str[i] != ' ')
        {
            str[count++] = str[i];
            i++;
        }
        else
        {
            while (str[i] == ' ')
            {
                if (str[i] == ' ' && str[i + 1] != ' ' && str[count - 1] != '|' && str[count - 1] != '>')
                    str[count++] = ' ';
                i++;
            }
        }
    }
    str[count] = '\0';
}

int unset_env_var(const char *var)
{

    char **e, **s;

    for (e = environ; *e;)
    {
        if ((*e)[strlen(var)] == ENV_SEP && strncmp(*e, var, strlen(var)) == 0)
        {
            s = e;
            while (*s)
            {
                *s = *(s + 1);
                s++;
            }
        }
        else
        {
            e++;
        }
    }

    return 0;
}

vector<string> split(const string &str, char delim = ' ')
{
    vector<string> v;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
    {
        v.push_back(token);
    }
    return v;
}

void catHelper()
{
    cout << "Usage: cat [OPTION]... [FILE]..." << endl;

    cout << "Concatenate FILE(s) to standard output." << endl;

    cout << "With no FILE, read standard input." << endl;
    cout << "-b, --number-nonblank    number nonempty output lines" << endl;
    cout << "-E, --show-ends          display $ at end of each line" << endl;
    cout << "-n, --number             number all output lines" << endl;
    cout << "-s, --squeeze-blank      suppress repeated empty output lines" << endl;
}

void headHelper()
{
    cout << "Usage: head [OPTION]... [FILE]..." << endl;
    cout << "With more than one FILE, precede each with a header giving the file name." << endl;

    cout << "With no FILE, or when FILE is -, read standard input." << endl;

    cout << "Mandatory arguments to long options are mandatory for short options too." << endl;
    cout << "-c, --bytes=[-]NUM       print the first NUM bytes of each file; " << endl;
    cout << "                         with the leading '-'," << endl;
    cout << "                          print all but the last NUM bytes of each file" << endl;
    cout << "-n, --lines=[-]NUM       print the first NUM lines instead of the first 10;" << endl;
    cout << "                           with the leading '-'," << endl;
    cout << "                           print all but the last NUM lines of each file" << endl;
    cout << "-q, --quiet, --silent    never print headers giving file names" << endl;
    cout << "-v, --verbose            always print headers giving file names" << endl;
}

void envHelper()
{
    cout << "Usage: env [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...]" << endl;
    cout << "  -u, --unset=NAME     remove variable from the environment" << endl;
}

void Version()
{
    cout << endl;
    cout << "******************" << endl;
    cout << "author: Simona Tuns" << endl;
    char *username = getenv("USER");
    cout << "User: " << username << endl;
    cout << "******************" << endl;
    cout << endl;
    sleep(1);
}

void openHelp()
{
    cout << "List of implemented commands:" << endl;
    cout << "$> cat -b" << endl;
    cout << "$> cat -E" << endl;
    cout << "$> cat -n" << endl;
    cout << "$> cat -s" << endl;
    cout << "$> head -c" << endl;
    cout << "$> head -n" << endl;
    cout << "$> head -q" << endl;
    cout << "$> head -v" << endl;
    cout << "$> env -u" << endl;
    cout << "$> exit" << endl;
    cout << endl;
    cout << "Use up and down arrows to go through the previous commands" << endl;
    cout << endl;
    cout << "TYPE s_help TO CHECK THE SYNTAX FOR EACH COMMAND" << endl;
    cout << endl;
    cout << "This shell also supports piping and redirection" << endl;
    cout << endl;

    sleep(1);
}

void syntaxHelp()
{
    cout << "cat -b syntax format: $> cat -b file-name" << endl;
    cout << "cat -E syntax format: $> cat -E file-name" << endl;
    cout << "cat -n syntax format: $> cat -n file-name" << endl;
    cout << "cat -s syntax format: $> cat -s file-name" << endl;
    cout << "head -c syntax format: $> cat -c number-of-bytes file-name" << endl;
    cout << "head -n syntax format: $> cat -n number-of-lines file-name" << endl;
    cout << "head -q syntax format: $> cat -q file1-name file2-name" << endl;
    cout << "head -v syntax format: $> cat -v file-name" << endl;
    cout << "env -u syntax format: $> cat -u variable-name" << endl;
}

int cat(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1)
    {
        string line;
        while (getline(cin, line))
        {
            if (line.compare("exit") == 0)
            {
                return EXIT_SUCCESS;
            }
            cout << line << endl;
        }
    }
    //fp = fopen(argv[1], "r");
    string line;
    if (argc >= 2 && file_exists(argv[1]))
    {
        int i = 1;
        while (i < argc)
        {
            ifstream f(argv[i]);
            while (!f.eof())
            {

                getline(f, line);
                cout << line;
                fflush(stdout);
                if (!f.eof())
                    cout << endl;
            }

            f.close();
            i++;
        }
        //fclose(fp);
        return EXIT_SUCCESS;
    }

    // -n numbers output lines
    if (strcmp(argv[1], "-n") == 0)
    {
        string line;
        string s = "\n";

        int k = 0;
        if (argc == 2)
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                cout << "     " << ++k << "  " << line << endl;
            }
        else
        {
            int count = 0;
            int i = 2;
            while (i < argc)
            {
                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                    while (!f.eof())
                    {
                        count++;
                        getline(f, line);
                        if (!f.eof())
                            cout << "     " << count << "	";
                        cout << line;
                        fflush(stdout);
                        if (!f.eof())
                            cout << endl;
                    }

                f.close();
                i++;
            }
            return EXIT_SUCCESS;
        }
        return EXIT_SUCCESS;
    }
    else
        //-b numbers non-empty output lines
        if (strcmp(argv[1], "-b") == 0)
    {
        string line;

        fp = fopen(argv[2], "r");
        int k = 0;
        if (argc == 2)
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                if (!line.empty())
                {
                    cout << "     " << ++k << "  " << line << endl;
                }
            }
        else
        {
            int count = 0;
            int i = 2;
            while (i < argc)
            {
                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                    while (!f.eof())
                    {

                        getline(f, line);
                        if (!f.eof())
                        {
                            if (!line.empty())
                            {
                                count++;
                                cout << "     " << count << "	";
                                cout << line;
                                fflush(stdout);
                                cout << endl;
                            }
                            else
                                cout << line << endl;
                        }
                    }

                f.close();
                i++;
            }
        }
        return EXIT_SUCCESS;
    }
    else
        // -E displays $ at the end of the line
        if (strcmp(argv[1], "-E") == 0)
    {
        string line;

        fp = fopen(argv[2], "r");

        int k = 0;
        if (argc == 2)
        {
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                cout << line;
                cout << "$";
                cout << endl;
            }
        }
        else
        {
            int i = 2;
            while (i < argc)
            {
                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                    while (!f.eof())
                    {
                        getline(f, line);
                        if (!f.eof())
                            cout << line << "$";

                        fflush(stdout);
                        if (!f.eof())
                            cout << endl;
                    }

                f.close();
                i++;
            }
        }
        return EXIT_SUCCESS;
    }
    else
        //-s surpresses reapeated empty rows
        if (strcmp(argv[1], "-s") == 0)
    {
        string line;

        fp = fopen(argv[2], "r");
        int k = 0;
        if (argc == 2)
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                cout << line << endl;
            }
        else
        {
            int empt = 0;
            int i = 2;
            while (i < argc)
            {
                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                    while (!f.eof())
                    {
                        getline(f, line);
                        if (!f.eof())
                        {
                            if (line.empty() && !empt)
                            {
                                cout << line << endl;
                                empt = 1;
                            }
                            else if (line.empty() && empt)
                            {
                                continue;
                            }
                            else if (!line.empty())
                            {
                                cout << line << endl;
                                empt = 0;
                            }
                        }
                    }

                f.close();
                i++;
            }
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fp = fopen(argv[2], "r");

        if (argc > 2 && fp != NULL)
        {
            cout << "cat: " << argv[1] << ": No such file or directory" << endl;
            cout << "OR" << endl;
            cout << "THIS COMMAND IS NOT IMPLEMANTED" << endl;
            cout << "CHECK COMMAND LIST" << endl;

            int i = 2;
            while (i < argc)
            {
                ifstream f(argv[i]);
                while (!f.eof())
                {

                    getline(f, line);
                    cout << line;
                    fflush(stdout);
                    if (!f.eof())
                        cout << endl;
                }

                f.close();
                i++;
            }
            fclose(fp);
            return EXIT_SUCCESS;
        }
        else if (fp == NULL)
        {
            int i = 1;
            while (i < argc)
            {
                cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                i++;
            }
        }
    }
    return EXIT_SUCCESS;
}

int head(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1)
    {
        string line;
        while (getline(cin, line))
        {
            if (line.compare("exit") == 0)
            {
                return EXIT_SUCCESS;
            }
            cout << line << endl;
        }
    }

    fp = fopen(argv[1], "r");
    string line;
    if (argc >= 2 && file_exists(argv[1]))
    {
        int i = 1;
        while (i < argc)
        {
            int contor = 0;
            ifstream f(argv[i]);
            while (!f.eof() && contor < 10)
            {
                contor++;
                getline(f, line);
                cout << line;
                fflush(stdout);
                if (!f.eof())
                    cout << endl;
            }

            f.close();
            i++;
        }
        fclose(fp);
        return EXIT_SUCCESS;
    }

    // -v shows data from the file, always proceded by its file name
    if (strcmp(argv[1], "-v") == 0)
    {
        string line;

        fp = fopen(argv[2], "r");
        ifstream f(argv[2]);
        int k = 0;
        if (argc == 2)
        {
            cout << "==> standard input <==" << endl;
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                cout << line << endl;
            }
        }

        else
        {
            int i = 2;
            while (i < argc)
            {

                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                {
                    cout << "==> " << argv[i] << " <==" << endl;
                    cout << endl;
                    while (!f.eof())
                    {
                        getline(f, line);
                        cout << line;
                        if (!f.eof())
                            cout << endl;
                        fflush(stdout);
                    }
                }
                //cout << endl;

                f.close();
                i++;
            }
        }
    }
    else
        // -n prints the first "k" lines
        if (strcmp(argv[1], "-n") == 0)
    {

        string line;

        if (argc == 3)

        {
            char *ptr = argv[2];
            if (*ptr == '-' || atoi(argv[2]))
            {
                int i = 0;
                while (getline(cin, line))
                {
                    if (line.compare("exit") == 0)
                    {
                        return EXIT_SUCCESS;
                    }
                    if (!strstr(argv[2], "-"))
                    {
                        cout << line << endl;
                        if (i >= atoi(argv[2]) - 1)
                        {
                            return EXIT_SUCCESS;
                        }
                        i++;
                    }
                }
                return EXIT_SUCCESS;
            }
            else
            {
                cout << "head: invalid number of lines" << endl;
            }
            return EXIT_SUCCESS;
        }
        else

            if (argc < 3)
        {
            cout << "head: option requires an argument -- 'n'" << endl;
            cout << "Try 'head --help' for more information." << endl;
            return EXIT_FAILURE;
        }
        else if (!strstr(argv[2], "-"))
        {
            int i = 3;
            while (i < argc)
            {
                ifstream f(argv[i]);

                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else

                {
                    if (argc > 4)
                    {
                        cout << endl;
                        cout << "==> " << argv[i] << " <==" << endl;
                    }

                    int cnt = 1;
                    while (!f.eof())
                    {
                        getline(f, line);
                        cnt++;
                        if (cnt > atoi(argv[2]))
                        {
                            if (i < argc)
                            {
                                f.close();
                            }
                            else
                            {
                                return EXIT_SUCCESS;
                            }
                        }
                        cout << line;
                        if (!f.eof())
                            cout << endl;
                        fflush(stdout);
                    }
                }
                i++;
            }
            return EXIT_SUCCESS;
        }
        else
        {

            int cnt = 0;
            char *parts[10];
            int partcount = 0;

            parts[partcount++] = argv[2];

            char *ptr = argv[2];
            while (*ptr)
            {
                if (*ptr == '-')
                {
                    *ptr = 0;
                    parts[partcount++] = ptr + 1;
                }
                ptr++;
            }
            int x = 0;
            for (int i = 1; i < partcount; i++)
            {
                x = x * 10 + atoi(parts[i]);
            }
            int i = 3;
            while (i < argc)
            {
                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                {
                    int cont = 0;
                    while (!f.eof())
                    {
                        getline(f, line);
                        cont++;
                    }
                    f.close();
                    f.open(argv[i]);

                    if (argc > 4)
                    {
                        cout << endl;
                        cout << "==> " << argv[i] << " <==" << endl;
                    }

                    int cnt = 1;
                    while (!f.eof())
                    {
                        getline(f, line);
                        cnt++;
                        if (cnt >= cont - x)
                        {
                            if (i < argc)
                            {
                                f.close();
                            }
                            else
                            {
                                return EXIT_SUCCESS;
                            }
                        }
                        cout << line;
                        if (!f.eof())
                            cout << endl;
                        fflush(stdout);
                    }
                }
                i++;
            }
        }
        return EXIT_SUCCESS;
    }
    else
        // -c prints first "k" bytes
        if (strcmp(argv[1], "-c") == 0)
    {

        char c;

        if (argc == 3)
        {
            char *ptr = argv[2];
            if (*ptr == '-' || atoi(argv[2]))
            {
                int i = 0;
                while (cin >> noskipws >> c)
                {
                    if (!strstr(argv[2], "-"))
                    {
                        if (c == 'C')
                            return EXIT_SUCCESS;
                        cout << c;
                        if (i >= atoi(argv[2]) - 1)
                        {
                            //cout << endl;
                            return EXIT_SUCCESS;
                        }
                        i++;
                    }
                }
                return EXIT_SUCCESS;
            }
            else
            {
                cout << "head: invalid number of bytes" << endl;
            }
            return EXIT_SUCCESS;
        }

        int cnt = 0;

        if (argc < 3)
        {
            cout << "head: option requires an argument -- 'c'" << endl;
            cout << "Try 'head --help' for more information." << endl;
            return EXIT_FAILURE;
        }
        ifstream f(argv[3]);
        int cont = 0;
        while (f >> noskipws >> c && !f.eof())
        {

            cont++;
        }
        if (argc == 4)
        {
            if (!strstr(argv[2], "-"))
            {
                fp = fopen(argv[3], "r");
                ifstream f(argv[3]);
                if (!file_exists(argv[3]))
                {
                    cout << "cat: " << argv[3] << ": No such file or directory" << endl;
                }
                else
                {
                    if (argc == 3)
                    {
                        int k = 0;
                        while (cin >> noskipws >> c)
                        {
                            k++;
                            if (k > atoi(argv[2]))
                            {
                                cout << endl;
                                return EXIT_SUCCESS;
                            }
                            cout << c;
                        }
                    }

                    while (f >> noskipws >> c)
                    {

                        cnt++;
                        if (cnt > atoi(argv[2]))
                        {
                            cout << endl;
                            return EXIT_SUCCESS;
                        }
                        cout << c;
                        fflush(stdout);
                    }
                    cout << endl;

                    f.close();
                    fclose(fp);
                }
            }
            else
            {
                ifstream f(argv[3]);
                if (!file_exists(argv[3]))
                {
                    cout << "cat: " << argv[3] << ": No such file or directory" << endl;
                }
                else
                {
                    char *parts[10];
                    int partcount = 0;

                    parts[partcount++] = argv[2];

                    char *ptr = argv[2];
                    while (*ptr)
                    {
                        if (*ptr == '-')
                        {
                            *ptr = 0;
                            parts[partcount++] = ptr + 1;
                        }
                        ptr++;
                    }
                    int x = 0;
                    for (int i = 1; i < partcount; i++)
                    {
                        x = x * 10 + atoi(parts[i]);
                    }

                    while (f >> noskipws >> c)
                    {

                        cnt++;
                        if (cnt > cont - x)
                        {
                            cout << endl;
                            return EXIT_SUCCESS;
                        }
                        cout << c;
                        fflush(stdout);
                    }
                    //cout << endl;

                    f.close();
                    fclose(fp);
                }
            }
        }
        else if (argc > 4)
        {

            if (!strstr(argv[2], "-"))
            {
                int i = 3;
                int cnt = 0;
                while (i < argc)
                {
                    ifstream f(argv[i]);
                    if (!file_exists(argv[i]))
                    {
                        cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                    }
                    else
                    {
                        cout << endl;
                        cout << "==> " << argv[i] << " <==" << endl;
                        // cout << endl;
                        while (f >> noskipws >> c)
                        {

                            cnt++;
                            if (cnt >= atoi(argv[2]))
                            {
                                if (i <= argc)
                                {
                                    //   cout << endl;
                                    f.close();
                                    cnt = 0;
                                }
                                else
                                {
                                    cout << endl;
                                    return EXIT_SUCCESS;
                                }
                            }
                            cout << c;
                            fflush(stdout);
                        }
                    }
                    //cout << endl;

                    i++;
                }
                f.close();
            }
            else
            {
                //ifstream f(argv[3]);
                char *parts[10];
                int partcount = 0;

                parts[partcount++] = argv[2];

                char *ptr = argv[2];
                while (*ptr)
                {
                    if (*ptr == '-')
                    {
                        *ptr = 0;
                        parts[partcount++] = ptr + 1;
                    }
                    ptr++;
                }
                int x = 0;
                for (int i = 1; i < partcount; i++)
                {
                    x = x * 10 + atoi(parts[i]);
                }
                int i = 3;
                int cnt = 0;
                while (i < argc)
                {
                    //cnt = 0;
                    ifstream f(argv[i]);
                    if (!file_exists(argv[i]))
                    {
                        cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                    }
                    else
                    {
                        int cont = 0;
                        while (f >> noskipws >> c && !f.eof())
                        {

                            cont++;
                        }
                        f.close();
                        f.open(argv[i]);
                        cout << endl;
                        cout << "==> " << argv[i] << " <==" << endl;
                        // cout << endl;
                        while (f >> noskipws >> c)
                        {

                            cnt++;
                            if (cnt > cont - x)
                            {
                                if (i <= argc)
                                {
                                    //   cout << endl;
                                    f.close();
                                    cnt = 0;
                                }
                                else
                                {
                                    cout << endl;
                                    return EXIT_SUCCESS;
                                }
                            }
                            else
                            {
                                cout << c;
                                fflush(stdout);
                            }
                        }
                    }
                    //cout << endl;

                    i++;
                }

                f.close();
            }
        }
    }
    else
        // -q for 2 files prints content not proceded by their file name
        if (strcmp(argv[1], "-q") == 0)
    {
        string line;
        //FILE *fp1 = fopen(argv[2], "r");
        if (argc == 2)
        {
            while (getline(cin, line))
            {
                if (line.compare("exit") == 0)
                {
                    return EXIT_SUCCESS;
                }
                cout << line << endl;
            }
            //fclose(fp1);
        }
        else
        {
            int i = 2;
            while (i < argc)
            {

                ifstream f(argv[i]);
                if (!file_exists(argv[i]))
                {
                    cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                }
                else
                    while (!f.eof())
                    {
                        getline(f, line);
                        cout << line;
                        if (!f.eof())
                            cout << endl;
                        fflush(stdout);
                    }

                f.close();
                i++;
            }
        }
    }
    else
    {
        fp = fopen(argv[2], "r");

        if (argc > 2 && fp != NULL)
        {
            cout << "head: cannot open '" << argv[1] << "' for reading: No such file or directory" << endl;
            cout << "OR" << endl;
            cout << "THIS COMMAND IS NOT IMPLEMANTED" << endl;
            cout << "CHECK COMMAND LIST" << endl;
            int i = 2;
            while (i < argc)
            {
                int contor = 0;
                ifstream f(argv[i]);
                while (!f.eof() && contor < 10)
                {
                    contor++;
                    getline(f, line);
                    cout << line;
                    fflush(stdout);
                    if (!f.eof())
                        cout << endl;
                }

                f.close();
                i++;
            }
            fclose(fp);
            return EXIT_SUCCESS;
        }
        else if (fp == NULL)
        {
            int i = 1;
            while (i < argc)
            {
                cout << "cat: " << argv[i] << ": No such file or directory" << endl;
                i++;
            }
        }
    }

    return EXIT_SUCCESS;
}

int env(int argc, char *argv[])
{
    if (argc == 1)
    {
        char *s1 = *environ;

        for (int i = 1; s1; i++)
        {
            cout << s1 << endl;
            s1 = *(environ + i);
        }
    }
    else if (argc < 3)
    {
        cout << "env: option requires an argument -- 'u'" << endl;
        cout << "Try 'env --help' for more information." << endl;
        return EXIT_FAILURE;
    }
    else if (strcmp(argv[1], "-u") == 0)

    {
        char *s1 = *environ;

        unset_env_var(argv[2]);

        for (int i = 1; s1; i++)
        {
            cout << s1 << endl;
            s1 = *(environ + i);
        }
    }

    else
    {
        cout << "THIS COMMAND IS NOT IMPLEMANTED" << endl;
        cout << "TYPE help TO CHECK COMMAND LIST" << endl;
        cout << "Try 'env --help' for more information." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int simple_redir(char *cmd)
{
    char *parts[10];
    int partcount = 0;

    parts[partcount++] = cmd;

    char *ptr = cmd;
    while (*ptr)
    {
        if (*ptr == '>')
        {
            *ptr = 0;
            parts[partcount++] = ptr + 1;
        }
        ptr++;
    }
    //for (int i = 0; i < partcount; i++)
    // cout << parts[i] << endl;

    vector<string> v1; // = split(cmd, '>');
    v1.push_back(parts[0]);
    vector<string> cm1[20];
    for (int i = 0; i < v1.size(); i++)
    {
        vector<string> aux = split(v1[i], ' ');
        cm1[i] = aux;
    }

    //fopen(parts[1], "w+");
    int fd;
    fd = open(parts[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    char **curr_cmd1 = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < cm1[i].size(); i++)
    {
        int count = 0;

        for (int j = 0; j < cm1[i].size(); j++)
        {
            curr_cmd1[count] = (char *)cm1[i][j].c_str();
            count++;
        }
        curr_cmd1[count + 1] = NULL;
    }
    if (fork() == 0)
    {
        dup2(fd, 1);
        execvp(curr_cmd1[0], curr_cmd1);
        close(1);
    }
    return EXIT_SUCCESS;
}

int pipe_redir(char *cmd)
{
    char *pt[10];
    int partct = 0;

    pt[partct++] = cmd;

    char *Ptr = cmd;
    while (*Ptr)
    {
        if (*Ptr == '>')
        {
            *Ptr = 0;
            pt[partct++] = Ptr + 1;
        }
        Ptr++;
    }

    vector<string> v2 = split(pt[0], '|');
    int nr_com = v2.size();
    vector<string> cmm[20];
    for (int i = 0; i < v2.size(); i++)
    {
        vector<string> aux = split(v2[i], ' ');
        cmm[i] = aux;
    }
    int fd1;
    fd1 = open(pt[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);

    int pipe1[nr_com][2];
    int pipe2[2];

    pid_t pid, pid2;
    int i = 0;

    for (int i = 0; i < nr_com; ++i)
    {
        pipe(pipe1[i]);
        if (pipe(pipe1[i]) == -1)
        {
            perror("bad pipe");
            exit(1);
        }
    }
    pipe(pipe2);
    if (pipe(pipe1[i]) == -1)
    {
        perror("bad pipe");
        exit(1);
    }

    while (i < nr_com)
    {
        int count = 0;
        char **curr_cmd = (char **)malloc(100 * sizeof(char *));
        for (int j = 0; j < cmm[i].size(); j++)
        {
            curr_cmd[count] = (char *)cmm[i][j].c_str();
            count++;
        }
        curr_cmd[count + 1] = NULL;
        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid > 0)
        {
            close(pipe1[i][0]);
            close(pipe1[i][1]);
            i++;
        }

        else

            if (!pid)
        {

            if (i == nr_com - 1)
            {
                if (dup2(pipe2[1], 1) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe2[0]);
                close(pipe2[1]);
            }
            else

            {
                if (dup2(pipe1[i + 1][1], 1) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe1[i + 1][0]);
                close(pipe1[i + 1][1]);
            }
            pid2 = fork();

            if (pid2 == -1)
            {
                perror("fork");
                exit(1);
            }
            if (i)
            {
                if (dup2(pipe1[i][0], 0) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe1[i][0]);
                close(pipe1[i][1]);
            }
            if (execvp(curr_cmd[0], curr_cmd) < 0)
            {
                perror(curr_cmd[0]);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (dup2(fd1, 1) < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    wait(NULL);
    char buffer[256];
    int x = read(pipe2[0], &buffer, 255);
    if (x < 0)
        exit(EXIT_FAILURE);
    buffer[x] = '\0';
    cout << buffer;
    close(fd1);
    return 0;
}

int piping(vector<string> *cm, int nr_com)
{
    int pipe1[nr_com][2];
    int pipe2[2];
    pid_t pid, pid2;
    int i = 0;
    int status;

    for (int i = 0; i < nr_com; ++i)
    {
        pipe(pipe1[i]);
        if (pipe(pipe1[i]) == -1)
        {
            perror("bad pipe");
            exit(1);
        }
    }
    pipe(pipe2);
    if (pipe(pipe2) == -1)
    {
        perror("bad pipe");
        exit(1);
    }

    while (i < nr_com)

    {
        int count = 0;
        char **curr_cmd = (char **)malloc(100 * sizeof(char *));

        for (int j = 0; j < cm[i].size(); j++)
        {
            curr_cmd[count] = (char *)cm[i][j].c_str();
            count++;
        }
        curr_cmd[count + 1] = NULL;

        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid > 0)
        {
            close(pipe1[i][0]);
            close(pipe1[i][1]);
            i++;
        }

        else

            if (!pid)
        {

            if (i == nr_com - 1)
            {
                if (dup2(pipe2[1], 1) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe2[0]);
                close(pipe2[1]);
            }
            else

            {
                if (dup2(pipe1[i + 1][1], 1) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe1[i + 1][0]);
                close(pipe1[i + 1][1]);
            }
            pid2 = fork();

            if (pid == -1)
            {
                perror("fork");
                exit(1);
            }

            if (i)
            {
                if (dup2(pipe1[i][0], 0) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe1[i][0]);
                close(pipe1[i][1]);
            }
            if (execvp(curr_cmd[0], curr_cmd) < 0)
            {
                perror(curr_cmd[0]);
                exit(EXIT_FAILURE);
            }
        }
    }
    wait(&status);
    char buffer[257];
    int x = read(pipe2[0], &buffer, 256);
    if (x < 0)
        exit(EXIT_FAILURE);
    buffer[x] = '\0';
    cout << buffer;
    return EXIT_SUCCESS;
}

int out_check(char *cmd)
{
    char *parts[10];
    int partcount = 0;

    parts[partcount++] = cmd;

    char *ptr = cmd;
    while (*ptr)
    {
        if (*ptr == '>')
        {
            return 1;
        }

        ptr++;
    }
    return 0;
}

int check_pipe_red(char *cmd)
{
    char *pt[10];
    int partct = 0;
    int red = 0, pipe = 0;

    pt[partct++] = cmd;

    char *Ptr = cmd;
    while (*Ptr)
    {
        if (*Ptr == '>')
        {
            red = 1;
        }
        if (*Ptr == '|')
        {
            pipe = 1;
        }
        Ptr++;
        if (pipe && red)
            return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    char *cmd;
    Version();
    openHelp();
    do
    {
        cmd = readline("$> ");

        removeSpaces(cmd);

        if (strlen(cmd) > 0)
            add_history(cmd);
        if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
        {
            free(cmd);
            continue;
        }
        if (strcmp(cmd, "exit") == 0)
        {
            free(cmd);
            break;
        }

        vector<string> v = split(cmd, '|');
        vector<string> cm[20];
        int nr_com = v.size();
        for (int i = 0; i < v.size(); i++)
        {
            vector<string> aux = split(v[i], ' ');
            cm[i] = aux;
        }
        if (check_pipe_red(cmd))
        {
            pipe_redir(cmd);
            break;
        }
        if (out_check(cmd))
        {
            simple_redir(cmd);
        }
        else if (nr_com == 1)
        {
            if (strcmp(cmd, "cat --help") == 0)
            {
                catHelper();
            }
            else if (strcmp(cmd, "head --help") == 0)
            {
                headHelper();
            }
            else if (strcmp(cmd, "env --help") == 0)
            {
                envHelper();
            }
            else
            {
                char *parts[10];
                int partcount = 0;

                parts[partcount++] = cmd;

                char *ptr = cmd;
                while (*ptr)
                {
                    if (*ptr == ' ')
                    {
                        *ptr = 0;
                        parts[partcount++] = ptr + 1;
                    }
                    ptr++;
                }
                if (strcmp(parts[0], "cat") == 0)
                    cat(partcount, parts);
                else if (strcmp(parts[0], "head") == 0)
                    head(partcount, parts);
                else if (strcmp(parts[0], "env") == 0)
                    env(partcount, parts);
                else if (strcmp(cmd, "help") == 0)
                    openHelp();
                else if (strcmp(cmd, "s_help") == 0)
                    syntaxHelp();

                else
                {
                    cout << "COMMAND NOT FOUND!!!" << endl;
                    cout << "TYPE help TO CHECK THE COMMAND LIST!!!" << endl;
                    cout << "TYPE s_help TO CHECK THE SYNTAX FOR EACH COMMAND" << endl;
                    // return EXIT_SUCCESS;
                }
            }
        }
        else if (nr_com > 1)
        {
            vector<string> w = split(cmd, '|');
            vector<string> cmnd[20];
            int nr_com = w.size();
            for (int i = 0; i < w.size(); i++)
            {
                vector<string> aux = split(w[i], ' ');
                cmnd[i] = aux;
            }
            piping(cmnd, nr_com);
            //return EXIT_SUCCESS;
        }

        free(cmd);
    } while (1);
}