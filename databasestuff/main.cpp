#include "database.h"
#include "driver.h"

using namespace std;

int main(int argc, char const *argv[]) {
    std::string dbName = "";
    std::cout << "Enter your preferred database name" << std::endl;
    getline(cin, dbName);

    char* ptr = (char *) malloc(sizeof(dbName)+1);
    strcpy(ptr, dbName.c_str());
    Driver* drvr = new Driver(ptr);
    std::vector<float> tempArr;
    long long count = 0;
    while (true) {
        // std::cout << "made it to loop" << endl;
        count++;
        tempArr = drvr->requestData();
        std::cout << "x = " << tempArr[0] << endl;
        std::cout << "y = " << tempArr[1] << endl;
        std::cout << "z = " << tempArr[2] << endl;
        std::cout << "pitch = " << tempArr[3] << endl;
        std::cout << "roll = " << tempArr[4] << endl;
        std::cout << "yaw = " << tempArr[5] << endl;
        // std::cout << "count = " << count << endl;

        if (count >300000 && tempArr[0] == 0 && tempArr[1] == 0 && tempArr[2] == 0) {
            break;
        }
    }
    drvr->print();

    return 0;
}
