//
// Created by arman on 07.04.2022.
//

#include "Sale.h"
#include "iostream"
#include "fstream"

Sale::Sale() {

}

Sale::Sale(string *dateFormat, string *carName, string *countCarsSold) {
    this->dateFormat = dateFormat;
    this->carName = carName;
    this->countCarsSold = countCarsSold;
}

// Util method:
string fileContent;
vector<string> splitString(string str, string delimiter = " ")
{
    vector<string> l;
    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        l.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    l.push_back(str.substr(start, end - start));
    return l;
}

void Sale::loadFile(string filePath) {
    cout << "Loading file " << filePath << "\n";
    fileContent.clear();
    sales.clear();

    ifstream fileIn;
    fileIn.open(filePath);
    string line;
    while ( getline (fileIn,line) )
    {
        fileContent.append(line).append("\n");
        vector<string> l = splitString(line);
        Sale sale;
        sale.dateFormat = &l.at(0);
        sale.carName = &l.at(1);
        sale.countCarsSold = &l.at(2);
        cout << *sale.dateFormat << "\n";
        sales.push_back(&sale);
    }
    cout << fileContent << "  -> EOF / Loaded file! \n"; // pipe stream's content to standard output
    fileIn.close();
}

void Sale::print() {
    // TODO FIX THIS. KINDA PRINTS WEIRD RESULTS.
    if (sales.empty())
        cout << *dateFormat << " " << *carName << " " << *countCarsSold << "\n";
    else
        for (Sale* s : sales) // access by reference to avoid copying
        {
            s->print();
        }
}
