#ifndef _DATABASE_
#define _DATABASE_

struct Question
{
    int id;
    char text[128];
    char answer1[32];
    char answer2[32];
    char answer3[32];
    char answer4[32];
    char answer5[32];
    int answers;
    int timeToAnswer;
    int correctAnswer;
};

void init();
int getQuestionsCount();
void getQuestionText(int id, char* buffer);
int getQuestionAnswers(int id);
void getQuestionAnswerText(int id, int answer, char* buffer);
int getScoreAnswer(int question, int answer, const char* answerText);
int getQuestionTimeout(int id);

#endif
