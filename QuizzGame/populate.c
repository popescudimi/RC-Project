#include <stdio.h>
#include <sqlite3.h> 

int main(void) {
  sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("QuizzGame.db", &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");

      char *sql =
                "DROP TABLE IF EXISTS Answers;"
                "DROP TABLE IF EXISTS Questions;"
                "CREATE TABLE Questions(id INT, text TEXT, timeToAnswer INT);"
                "INSERT INTO Questions VALUES(1, 'In ce an a murit Albert Einstein?', 12);"
                "INSERT INTO Questions VALUES(2, 'In ce an s-a nascut Albert Einstein?', 12);"
                "INSERT INTO Questions VALUES(3, 'Ce drog se gaseste in ceai si cafea?', 12);"
                "INSERT INTO Questions VALUES(4, 'Care este elementul de baza in compozitia stelelor?', 12);"
                "INSERT INTO Questions VALUES(5, 'Ce sport se practica in NBA?', 10);"
                "INSERT INTO Questions VALUES(6, 'Care este capitala Japoniei?', 8);"
                "INSERT INTO Questions VALUES(7, 'Care este capitala Portugaliei?', 8);"
                "INSERT INTO Questions VALUES(8, 'Care este capitala Suediei?', 8);"
                "INSERT INTO Questions VALUES(9, 'In ce tara a fost inventat bumerangul?', 10);"
                "INSERT INTO Questions VALUES(10, 'Care este capitala Belgiei?', 8);"
                "INSERT INTO Questions VALUES(11, 'Care este simbolul ceriului, element chimic?', 12);"
                "INSERT INTO Questions VALUES(12, 'Care dintre urmatoarele nu este vecin al Ungariei?', 10);"
                "INSERT INTO Questions VALUES(13, 'Care este a cincea litera din alfabetul latin?', 6);"
                "INSERT INTO Questions VALUES(14, 'Care este bautura marinarilor?', 10);"
                "INSERT INTO Questions VALUES(15, 'Care este capitala Bulgariei?', 8);"
                "INSERT INTO Questions VALUES(16, 'Care este numarul de sateliti ai planetei Venus?', 12);"
                "CREATE TABLE Answers(id INT, question INT, text TEXT, score INT, FOREIGN KEY(question) REFERENCES Questions(id));"
                "INSERT INTO Answers VALUES(101, 1, '1955', 5);"
                "INSERT INTO Answers VALUES(102, 1, '1962', 0);"
                "INSERT INTO Answers VALUES(103, 1, '1950', 0);"
                "INSERT INTO Answers VALUES(104, 1, '1870', 0);"
                "INSERT INTO Answers VALUES(201, 2, '1900', 0);"
                "INSERT INTO Answers VALUES(202, 2, '1879', 5);"
                "INSERT INTO Answers VALUES(203, 2, '2000', 0);"
                "INSERT INTO Answers VALUES(204, 2, '1850', 0);"
                "INSERT INTO Answers VALUES(301, 3, 'heroina', 0);"
                "INSERT INTO Answers VALUES(302, 3, 'cocaina', 5);"
                "INSERT INTO Answers VALUES(303, 3, 'zahar', 0);"
                "INSERT INTO Answers VALUES(304, 3, 'nimic', 0);"
                "INSERT INTO Answers VALUES(401, 4, 'Oxigen', 0);"
                "INSERT INTO Answers VALUES(402, 4, 'Acid sulfuric', 0);"
                "INSERT INTO Answers VALUES(403, 4, 'Apa', 0);"
                "INSERT INTO Answers VALUES(404, 4, 'Hidrogen', 5);"
                "INSERT INTO Answers VALUES(405, 4, 'Arsenic', 0);"
                "INSERT INTO Answers VALUES(501, 5, 'Biliard', 0);"
                "INSERT INTO Answers VALUES(502, 5, 'Biatlon', 0);"
                "INSERT INTO Answers VALUES(503, 5, 'Baschet', 5);"
                "INSERT INTO Answers VALUES(504, 5, 'Baseball', 0);"
                "INSERT INTO Answers VALUES(601, 6, 'Yokohama', 0);"
                "INSERT INTO Answers VALUES(602, 6, 'Yamashina', 0);"
                "INSERT INTO Answers VALUES(603, 6, 'Tokyo', 5);"
                "INSERT INTO Answers VALUES(604, 6, 'Nagoya', 0);"
                "INSERT INTO Answers VALUES(701, 7, 'Madrid', 0);"
                "INSERT INTO Answers VALUES(702, 7, 'Porto', 0);"
                "INSERT INTO Answers VALUES(703, 7, 'Ohio', 0);"
                "INSERT INTO Answers VALUES(704, 7, 'Lisabona', 5);"
                "INSERT INTO Answers VALUES(801, 8, 'Alegria', 0);"
                "INSERT INTO Answers VALUES(802, 8, 'Copenhaga', 0);"
                "INSERT INTO Answers VALUES(803, 8, 'Olso', 0);"
                "INSERT INTO Answers VALUES(804, 8, 'Stokholm', 5);"
                "INSERT INTO Answers VALUES(901, 9, 'Anglia', 0);"
                "INSERT INTO Answers VALUES(902, 9, 'Australia', 5);"
                "INSERT INTO Answers VALUES(903, 9, 'Italia', 0);"
                "INSERT INTO Answers VALUES(904, 9, 'Spania', 0);"
                "INSERT INTO Answers VALUES(1001, 10, 'Bruxelles', 5);"
                "INSERT INTO Answers VALUES(1002, 10, 'Berna', 0);"
                "INSERT INTO Answers VALUES(1003, 10, 'Haga', 0);"
                "INSERT INTO Answers VALUES(1004, 10, 'Geneva', 0);"
                "INSERT INTO Answers VALUES(1101, 11, 'Ce', 5);"
                "INSERT INTO Answers VALUES(1102, 11, 'Cu', 0);"
                "INSERT INTO Answers VALUES(1103, 11, 'Cr', 0);"
                "INSERT INTO Answers VALUES(1104, 11, 'Cl', 0);"
                "INSERT INTO Answers VALUES(1201, 12, 'Romania', 0);"
                "INSERT INTO Answers VALUES(1202, 12, 'Austria', 0);"
                "INSERT INTO Answers VALUES(1203, 12, 'Croatia', 0);"
                "INSERT INTO Answers VALUES(1204, 12, 'Germania', 5);"
                "INSERT INTO Answers VALUES(1301, 13, 'd', 0);"
                "INSERT INTO Answers VALUES(1302, 13, 'f', 0);"
                "INSERT INTO Answers VALUES(1303, 13, 'g', 0);"
                "INSERT INTO Answers VALUES(1304, 13, 'e', 5);"
                "INSERT INTO Answers VALUES(1401, 14, 'bere', 0);"
                "INSERT INTO Answers VALUES(1402, 14, 'votca', 0);"
                "INSERT INTO Answers VALUES(1403, 14, 'vin', 0);"
                "INSERT INTO Answers VALUES(1404, 14, 'rom', 5);"
                "INSERT INTO Answers VALUES(1501, 15, 'Bratislava', 0);"
                "INSERT INTO Answers VALUES(1502, 15, 'Bucuresti', 0);"
                "INSERT INTO Answers VALUES(1503, 15, 'Budapesta', 0);"
                "INSERT INTO Answers VALUES(1504, 15, 'Sofia', 5);"
                "INSERT INTO Answers VALUES(1601, 16, '1', 0);"
                "INSERT INTO Answers VALUES(1602, 16, '0', 5);"
                "INSERT INTO Answers VALUES(1603, 16, '5', 0);"
                "INSERT INTO Answers VALUES(1604, 16, '10', 0);";

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      } else {
        fprintf(stdout, "Table created successfully\n");
      }
   }
   sqlite3_close(db);
}