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

#include "protocol.h"

/* declated in errno.h, and this variable is in one of system libraries */
extern int errno;

int port;

void clientSession(int);

int main(int argc, char *argv[])
{
    int sd;			// socket descriptor
    struct sockaddr_in server;	// structura folosita pentru conectare 

    if (argc != 3)
    {
        printf("Syntax: %s <server_address> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("Cannot create socket ");
        return errno;
    }

    bzero(&server, sizeof (server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr =  inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
        perror("Cannot connect to server ");
        return errno;
    }

    clientSession(sd);

    close(sd);
    return 0l;
}

void cut_line(char* name)
{
    int l = strlen(name);
    if (name[l-1]=='\n')
       name[l-1] = 0;
}

void clientSession(int sd)
{
    /* Gets hello message from server */
    char* line = readLine(sd);
    int e = 0;  // variabila pentru anumite erori
    int answerIndex = 0; // raspunsuri

    if (line)
    {       
        char name[128];

        free(line);
        printf("Enter user name to register: "); // inregistrarea userului
        fgets(name, sizeof(name), stdin);
        cut_line(name);

        writeLine(sd, REGISTER_USER_COMMAND); // socket-ul va sti ca user-ul se inregistreaza
        writeLine(sd, name);
        writeLine(sd, "\n");

        line = readLine(sd);
        if (line)
        {
            if (startsWith(line, SUCCESS_USER_REGISTER)) // daca clientul s-a inregistrat cu succes
            {
                printf("Registered...\n\n");

                while(line)
                {
                    writeLine(sd, GET_QUESTION_COMMAND); // avem nevoie de intrebari
                    writeLine(sd, "\n");
                    free(line); // elibram memorie de fiecare data
                    line = readLine(sd); // citim din nou linia de comanda
                    if (line)
                    {
                        if (startsWith(line, QUESTION_TEXT_COMMAND)) // primirea intrebarilor
                        {
                            printf("Received question:...\n\n");
                            printf("%s", &line[strlen(QUESTION_TEXT_COMMAND)]); // intrebarea efectiv

                            free(line);
                            line = readLine(sd);
                            answerIndex = 0;
                            while(line)
                            {
                                if (startsWith(line, ANSWER_OPTION_COMMAND)) // primim raspunsuri
                                {
                                    answerIndex++;            
                                    printf("%d) %s", answerIndex, &line[strlen(ANSWER_OPTION_COMMAND)]);
                                    free(line);
                                    line = readLine(sd);
                                }
                                else
                                {                              
                                    char buffer[128];
                                    printf("Pick answer: ");
                                    fgets(buffer, sizeof(buffer), stdin);
                                    cut_line(buffer);
                                    sprintf(buffer, "%d", atoi(buffer)); // 1,2,3,4
                                    writeLine(sd, ANSWER_COMMAND);
                                    writeLine(sd, buffer);
                                    writeLine(sd, "\n");
                                    printf("\n");
                                    break;
                                }
                            }
                        }
                        else if (startsWith(line, NO_QUESTIONS_COMMAND)) // toate intrebarile au fost adresate
                        {
                            char fixedName[256];
                            sprintf(fixedName, "%s\n", name); // 

                            printf("Answered all questions...\n\n");
                            writeLine(sd, WHOIS_WINNER_COMMAND);
                            writeLine(sd, "\n");
                            free(line);
                            line = readLine(sd);
                            printf("Winner is : %s", line);
                            if (!strcmp(line, fixedName)) 
                            {
                                printf("You is winner!\n\n"); // CASTIGATORUL
                            }
                            break;
                        }
                        else
                        {
                            printf("Something wrong...\n\n");
                            break;                        
                        }
                    }
                    else
                        e++;
                }
            }
            else
            {
                free(line);
                printf("Failed to register user\n\n");
            }

        }
        else
            e++; /* indicates read socket error */        
    }
    else
        e++; /* indicates read socket error */


    if (e)
    {
        printf("Error: Cannot read response from server");
    } 
}
