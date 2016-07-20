#include "driver.h"
#include "database.h"
#include <string>

Driver::Driver(char * s)
{
    db = new Database(s);
    // std::cout << "database done" << std::endl;
    controller = Leap::Controller();
    // std::cout << "Leap created" << std::endl;
    if (!controller.isServiceConnected()) {
        std::cout << "Service NOT connected" << std::endl;
    }
    vec = Leap::Vector(0, 0, 0);
    // std::cout << "Vector done" << std::endl;
    dbname = (char *)malloc(sizeof(s+1));
    strcpy(dbname, s);
    std::string tableName = "";
    std::cout << "enter your preferred table name" << std::endl;
    getline(std::cin, tableName);
    char* ptr = (char *) malloc(sizeof(tableName)+1);
    strcpy(ptr, tableName.c_str());
    dbTableName =  (char *) malloc(sizeof(ptr+1));
    strcpy(dbTableName, ptr);
    db->createTable(dbTableName);

    // std::cout << "string copied done" << std::endl;
    up = 4;
    forward = 4;
    right = 4;
    oldTime = 0.0;
}

bool Driver::seeChange(Leap::Vector v) {
    if (up == 4) {
        up = isUp(v.y);
        right = isRight(v.z);
        forward = isForward(v.x);
        return true;

    }
    int newup = isUp(v.y);
    int newright = isRight(v.z);
    int newforward = isForward(v.x);

    if (newup != up || newforward != forward || newright != right) {
        up = newup;
        right = newright;
        forward = newforward;
        return true;
    }
    return false;

}

int Driver::isUp(float k) {
    float r = vec.y - k;
    if (r < 0) return 1;
    else if (r ==0.0) return 0;
    else return -1;
}

int Driver::isRight(float k) {
    float r = vec.z - k;
    if (r < 0) return 1;
    else if (r == 0.0) return 0;
    else return -1;

}

int Driver::isForward(float k) {
    float r = vec.x - k;
    if (r < 0) return 1;
    else if (r == 0.0) return 0;
    else return -1;
}

int Driver::print()
{
    std::cout <<"about to print" << std::endl;
    int value = db->getData(dbTableName);
    return value;
}

std::vector<float> Driver::requestData() {
    std::vector<float> fltArr;
    Leap::Frame frame = controller.frame(); // controller is a Controller object
    // std::cout << "made controller" << std::endl;
    Leap::HandList hands = frame.hands();
    // std::cout << "made hand" << std::endl;

    Leap::Hand firstHand = frame.hands()[0];
    // std::cout << "made firstHand" << std::endl;

    newTime = frame.timestamp();
    if (oldTime == newTime) {
        fltArr.push_back(vec.x);
        fltArr.push_back(vec.y);
        fltArr.push_back(vec.z);
        fltArr.push_back(vec.pitch());
        fltArr.push_back(vec.roll());
        fltArr.push_back(vec.yaw());

        return fltArr;
    }
    oldTime = newTime;
    // std::stringstream ss (std::stringstream::in | std::stringstream::out);
    // ss << time;
    // std::string strTime(ss.str());
    // std::cout << "made time" << std::endl;
    // int newTime = (int) time;
    Leap::Vector v = firstHand.palmPosition();
    // std::cout << "made firstHand palm" << std::endl;
    

    fltArr.push_back(v.x);
    fltArr.push_back(v.y);
    fltArr.push_back(v.z);
    fltArr.push_back(v.pitch());
    fltArr.push_back(v.roll());
    fltArr.push_back(v.yaw());

    if(seeChange(v)) {
        // std::cout << "saw change" << std::endl;
        // std::cout << "database name: " << dbname << std::endl;
        //
        // std::cout << "values" << std::endl;
        // std::cout << "time: " << newTime << std::endl;
        // std::cout << "v.x: " << v.x << std::endl;
        // std::cout << "v.y: " << v.y << std::endl;
        // std::cout << "v.z: " << v.z << std::endl;
        db->insert(dbTableName, newTime, v.x, v.y, v.z, v.pitch(), v.roll(), v.yaw());
        // std::cout << "inserted change" << std::endl;
    }
    vec = v;
    return fltArr;


}
