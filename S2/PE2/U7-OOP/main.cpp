#include <iostream>
#include <string>
#include "UtisString.h"
#include "Date.h"
#include "Worker.h"
#include "Worker2.h"
#include "Worker3.h"

int main(int argc, char *argv[]) {
    UtilsString utils{};
    auto vec= utils.split("- This, a sample string.", " ,.-");
    for (const auto &item : vec){
        printf("%s\n", item.data());
    }
    Date::now();

    auto peter = Worker();
    auto frank = Worker2();
    auto john = Worker3();
    peter.standardIncome = 1000;
    frank.standardIncome = 1000;
    john.standardIncome = 1000;
    peter.bonusIncome = 10;
    frank.bonusIncome = 10;
    john.bonusIncome = 10;
    peter.age = 30;
    frank.age = 29;
    john.age = 55;

    cout << peter.actualIncome() << " "
         << frank.actualIncome() << " "
         << john.actualIncome() << "\n";

    return 0;
}