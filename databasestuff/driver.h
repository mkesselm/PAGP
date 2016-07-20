#ifndef DRIVER_H
#define DRIVER_H
#include "Leap.h"
#include "database.h"


class Driver
{
public:
    Driver(char * s);
    ~Driver();
    std::vector<float> requestData();
    Database * getDB();
    int print();



private:
    Database * db;
    Leap::Controller controller;
    Leap::Vector vec;
    float oldTime;
    float newTime;
    bool seeChange(Leap::Vector);
    int isUp(float k);
    int isRight(float k);
    int isForward(float k);
    char * dbname;
    char * dbTableName;
    int up, forward, right;

};

#endif // DRIVER_H
