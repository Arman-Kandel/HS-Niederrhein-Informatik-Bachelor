#include <iostream>
#include <list>
#include "string"
#include "iostream"
#include "iomanip"
#include "fstream"
#include "filesystem"
#include "Sale.h"

int main(int argc, char *argv[]) {
    std::__fs::filesystem::path cwd = std::__fs::filesystem::current_path() / "sales.txt"; // Current working dir
    std::ofstream fileOut;
    fileOut.open(cwd.string());
    fileOut << "20220106 Ford 19\n"
               "20220106 VW 21\n"
               "20220105 Ford 15\n"
               "20220105 Opel 21\n"
               "20220104 BMW 8\n"
               "20220104 VW 14\n"
               "20220104 Ford 9\n"
               "20220103 BMW 17\n";
    fileOut.close();
    Sale sale;
    sale.loadFile(cwd.string());
    sale.print();
    return 0;
}