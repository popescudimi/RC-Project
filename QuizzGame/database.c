#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"
#include <sqlite3.h> 


static int countCallback(void *count, int argc, char **argv, char **azColName) {
    int *c = count;
    *c = atoi(argv[0]);
    return 0;
}

static int textCallback(void *text, int argc, char **argv, char **azColName) {
    char **t = text;
    strcpy(*t, argv[0]);
    return 0;
}

int getQuestionsCount() // aflam numarul de intrebari din baza de date
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

   rc = sqlite3_open("QuizzGame.db", &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return (0) ;
   }else{
      char *sql = "SELECT Count(*) FROM Questions";
      int count = 0;
      rc = sqlite3_exec(db, sql, countCallback, &count, &zErrMsg);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
      sqlite3_close(db);
      return count;
   }
}

void getQuestionText(int id, char* buffer) // extragem intrebarea
{
    char query[256];
    sprintf(query, "SELECT text, id FROM Questions WHERE id=%d", id);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("QuizzGame.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
       rc = sqlite3_exec(db, query, textCallback, &buffer, &zErrMsg);
       //printf("buffer");
       //printf("%s\n", buffer);
       if (rc != SQLITE_OK) {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
       }
       sqlite3_close(db);
    }
}

int getQuestionAnswers(int id) // extragem raspunsurile
{
    char query[256];
    sprintf(query, "SELECT Count(*) FROM Answers WHERE question=%d", id);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("QuizzGame.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return 0;
    } else {
      int count = 0;
      rc = sqlite3_exec(db, query, countCallback, &count, &zErrMsg);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
      sqlite3_close(db);
      return count;
    }
}

int getQuestionTimeout(int id) // verificam timpul
{
    char query[256];
    sprintf(query, "SELECT timeToAnswer FROM Questions WHERE id=%d", id);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("QuizzGame.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return 0;
    } else {
      int count = 0;
      rc = sqlite3_exec(db, query, countCallback, &count, &zErrMsg);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
      sqlite3_close(db);
      return count;
    }
}

void getQuestionAnswerText(int id, int answer, char* buffer) // variantele de raspuns, le luam dupa id 
{
    char query[256];
    sprintf(query, "SELECT text, id FROM Answers WHERE id=%d", id*100 + (answer+1));

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("QuizzGame.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
       rc = sqlite3_exec(db, query, textCallback, &buffer, &zErrMsg);
       //printf("buffer");
       //printf("%s\n", buffer);
       if (rc != SQLITE_OK) {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
       }
       sqlite3_close(db);
    }
}

int getScoreAnswer(int question, int answer, const char* answerText) // vedem care este scorul 
{
    char query[256];
    sprintf(query, "SELECT score,question,id FROM Answers WHERE question=%d AND id=%d", question, question*100+answer);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("QuizzGame.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return 0;
    } else {
      int count = 0;
      rc = sqlite3_exec(db, query, countCallback, &count, &zErrMsg);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
      sqlite3_close(db);
      return count;
    }
}

