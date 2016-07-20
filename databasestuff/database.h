#ifndef DATABASE_H
#define DATABASE_H


#include <sqlite3.h>
#include <iostream>
#include <stdio.h>

class Database
{


public:

  Database(char* name);
  ~Database();
  int openConnection(char* name);
  void insert(char* table, float vTime, float x, float y, float z, float pitch
                    ,float roll, float yaw);
  void closeConnection();
  void createTable(char* dbName);
  int getData(char * name);
  static int callback(void *NotUsed, int argc, char **argv, char **azColName)
  {
     int i;
     for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
     }
     printf("\n");
     return 0;
  }




private:
  sqlite3* db;
  sqlite3_stmt *stmt;
  char *errMsg;
  int rc;
  char* dbName;
  std::string sqlStatement;
  const char* sql;
};


#endif // DATABASE_H
