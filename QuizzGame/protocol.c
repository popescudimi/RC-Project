#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

//verificam linia cu comanda specifica 
int startsWith(const char* line, const char* substring)
{
   return strncmp(line, substring, strlen(substring))==0;
}


// verificam cazul atunci cand avem null
char* concatenate(char* line, char* buffer)
{
    if (!line)
    {
        return strdup(buffer);
    }
    else
    {
        int l1 = strlen(line);
        int l2 = strlen(buffer);
        char* newline = malloc(l1 + l2 + 1);
        bzero(newline, l1 + l2 + 1);
        strcpy(newline, line);
        strcat(newline, buffer);
        free(line);

        return newline;
    }
}

// verificam daca am ajuns la final adica la \n
int finished(char* line)
{
    if (line)
    {
        int l = strlen(line);
        if (l>0)
        {
            if (line[l-1] == '\n')
                return 1;
        }
        return 0;
    }
    return 1;
}

// citim din socket pana cand avem \n sau socketul e inchis
char* readLine(int sd)
{
    char buffer[1024];
    char* line = NULL;

    do
    {   
        bzero(buffer, sizeof(buffer));
        if (read(sd, buffer, 1)>0)
        {
            line = concatenate(line, buffer);
        }
        else
        {
            break;
        }
    }while(!finished(line));

    return line;
} 

// scrie catre socket
int writeLine(int sd, const char* line)
{
    return write(sd, line, strlen(line));
}
