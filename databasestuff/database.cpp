#include "database.h"
#include <sqlite3.h>
#include <iostream>



Database::Database(char* name)
{
  errMsg = 0;
  rc = sqlite3_open(name, &db);
  if (rc) {
      std::cerr << "can't open database " << name
              << ", please check the name and try again" << std::endl;
      sqlite3_close(db);
  } else {
      std::cout << "database " << name <<  " created successfully" << std::endl;
  }
  // sqlite3_close(db);
  // this->createTable(ptr);


}


Database::~Database() {
    sqlite3_close(db);
    delete dbName;
    delete sql;
    delete errMsg;

}

void Database::insert(char* tableName, float time1, float x, float y, float z, float pitch, float roll, float yaw)
{

    // std::cout << "inside insert" << std::endl;
    std::string name(tableName);

    std::string varTime = std::to_string(time1);
    std::string varX = std::to_string(x);
    std::string varY = std::to_string(y);
    std::string varZ = std::to_string(z);
    std::string varP = std::to_string(pitch);
    std::string varR = std::to_string(roll);
    std::string varYaw = std::to_string(yaw);

    sqlStatement = "INSERT INTO " + name + "(TIME, X, Y, Z, PITCH, ROLL, YAW)" " VALUES(" + varTime + ", " + varX + ", " + varY + ", " + varZ +  ", " +
                varP + ", " + varR + ", " + varYaw + ");";

     sql = sqlStatement.c_str();
    //  std::cout << "statement made" << std::endl;
    //  std::cout << sql << std::endl;

     rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    //  std::cout << "sqlite executed" << std::endl;
     if (rc != SQLITE_OK) {
        //  std::cout << "we iz not ok" << std::endl;
         std::cout << "SQL error: "<< errMsg << std::endl;
     } else {
       std::cout << "insert successfully" << std::endl;
     }
}


int Database::openConnection(char *name)
{
  rc = sqlite3_open_v2(name, &db, SQLITE_OPEN_READWRITE, NULL);
  if (rc) {
      std::cerr << "can't open database " << name
              << ", please check the name and try again" << std::endl;
      sqlite3_close(db);
      return(1);
  } else {
      std::cout << "database opened successfully" << std::endl;
      return (0);
  }


}



void Database::closeConnection()
{
  sqlite3_close(db);
  // std::cout << "probs closed" << std::endl;
}

int Database::getData(char * name)
{
    std::string str(name);
    sqlStatement = "SELECT * FROM " + str + ";";
    sql = sqlStatement.c_str();
    rc = sqlite3_exec(db, sql, callback, NULL, &errMsg);
      if( rc != SQLITE_OK ){
         fprintf(stderr, "SQL error: %s\n", errMsg);
         sqlite3_free(errMsg);
      }else{
         fprintf(stdout, "Operation done successfully\n");
      }
      sqlite3_close(db);
      return 0;
}



void Database::createTable(char* dbName)
{

  std::string name(dbName);
  this->dbName = dbName;

    /** SQL STATEMENT */
    sqlStatement = "CREATE TABLE " + name + " ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "TIME   REAL             NOT NULL ,"
                  "X      REAL             NOT NULL,"
                  "Y      REAL             NOT NULL,"
                  "Z      REAL             NOT NULL,"
                  "PITCH  REAL             NOT NULL,"
                  "ROLL   REAL             NOT NULL,"
                  "YAW    REAL             NOT NULL);";

     sql = sqlStatement.c_str();

    // rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    while((rc = sqlite3_exec(db, sql, callback, 0, &errMsg)) != SQLITE_OK) {
        std::cout << "SQL error: "<< errMsg << std::endl;
        std::cout << "enter new table name" << std::endl;
        std::string str = "";
        getline(std::cin, str);
        sqlStatement = "CREATE TABLE " + str + " ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "TIME   REAL             NOT NULL ,"
                      "X      REAL             NOT NULL,"
                      "Y      REAL             NOT NULL,"
                      "Z      REAL             NOT NULL,"
                      "PITCH  REAL             NOT NULL,"
                      "ROLL   REAL             NOT NULL,"
                      "YAW    REAL             NOT NULL);";

         sql = sqlStatement.c_str();
    }
    std::cout << "Table created successfully" << std::endl;



}
