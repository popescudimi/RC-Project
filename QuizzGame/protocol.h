#ifndef _PROTOCOL_
#define _PROTOCOL_


#define REGISTER_USER_COMMAND  "RegisterUser:" // comanda inregistrare
#define FAILED_USER_REGISTER   "Failed:RegisterUser" // comanda inregistare fail
#define SUCCESS_USER_REGISTER  "Success:RegisterUser" // comanda inregistrare succes

#define GET_QUESTION_COMMAND           "GetQuestion:" // intrebare
#define NO_QUESTIONS_COMMAND           "NoQuestions:" // nici-o intrebare
#define QUESTION_TEXT_COMMAND          "Question:"   // intrebare
#define ANSWER_OPTION_COMMAND          "AnswerOption:" // optiunea de a raspunde
#define DONE_QUESTION_COMMAND          "End:"    // end
#define ANSWER_COMMAND                 "Answer:" // raspunsul
#define WHOIS_WINNER_COMMAND           "WhoIsWinner:" // castigator

int startsWith(const char* line, const char* substring);

char* readLine(int sd);
int writeLine(int sd, const char* line);

#endif /* _PROTOCOL_ */
