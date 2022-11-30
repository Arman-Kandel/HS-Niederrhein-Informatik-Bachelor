#include "db.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h> // c string functions
#include <string>   // cpp string
#include <vector>


using namespace std;

int main(int argc, char *argv[]) {
  //"".c_str();
  string user = "";
  string password = "";
  string host = "";
  string port = "";
  string dbName = "";
  string commandFile = "";

  // connect options aus argv lesen und verarbeiten
  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];
    if (strcmp(arg, "-u") == 0) { // user
      user = argv[i + 1];
    } else if (strcmp(arg, "-c") == 0) { // credential/password
      password = argv[i + 1];
    } else if (strcmp(arg, "-h") == 0) { // host
      host = argv[i + 1];
    } else if (strcmp(arg, "-p") == 0) { // port
      port = argv[i + 1];
    } else if (strcmp(arg, "-d") == 0) { // database name
      dbName = argv[i + 1];
    } else if (strcmp(arg, "-f") == 0) { // command file name
      commandFile = argv[i + 1];
    }
    // else do nothing
  }
  if (user.empty()) {
    cerr << "Missing argument user.\n";
    return 1;
  }
  // Password is allowed to be empty
  // if(password.empty()){cerr << "Missing argument password.\n"; return 1;}
  if (host.empty()) {
    cerr << "Missing argument host.\n";
    return 1;
  }
  if (port.empty()) {
    cerr << "Missing argument port.\n";
    return 1;
  }
  if (dbName.empty()) {
    cerr << "Missing argument dbName.\n";
    return 1;
  }
  if (commandFile.empty()) {
    cerr << "Missing argument commandFile.\n";
    return 1;
  }

  std::cout << "############ FILE: " << commandFile << endl;
  std::cout << "############ FILE: " << commandFile << endl;
  std::cout << "############ FILE: " << commandFile << endl;

  // Pfad des commandfiles steht im letzten übergebenen Kommandozeilenparameter
  ifstream infile(commandFile.c_str());

  if (!infile.good()) {
    cerr << "Failed to open file " << commandFile << "\n";
    return 1;
  }

  // Command Files Zeile für Zeile verarbeiten
  string line;
  vector<string> values;

  // Diese while-Schleife iteriert durch alle Zeilen des command files
  db_login(user, password, host, port, dbName);
  db_begin();
  while (getline(infile, line)) {

    string value;
    istringstream stream(line);

    // Diese while-Schleife zerlegt die aktuelle Zeile in Teilstrings und legt
    // diese in values ab
    int countValues = 0;
    while (stream >> value) {
      values.push_back(value);
      countValues++;
    }

    std::cout << "<<<<<<<< READ: values: " << countValues << " line: " << line
              << endl;
    string firstValue = values[0];
    if (firstValue == "n") {
      // n
      // Erzeugt eine leere Datenbank, indem die Tabellen produkt und
      // hersteller (falls erforderlich) gelöscht und neu kreiert werden.
      db_delete();
      db_create_table_produkt();
      db_create_table_hersteller();
    } else if (firstValue == "ih") {
      // ih hnr name stadt
      // Fügt ein neues Tupel mit den angegebenen Werten in die Tabelle
      // hersteller ein. Falls bereits ein Tupel mit der gegebenen hnr
      // existiert, sollen für dieses Tupel die Attribute name und stadt
      // entsprechend geändert werden.
      if (db_check_hnr(values[1]) == 1)
        db_update_hersteller(values[1], values[2], values[3]);
      else
        db_insert_hersteller(values[1], values[2], values[3]);
    } else if (firstValue == "ip") {
      // ip pnr name preis hnr
      // Fügt ein neues Tupel mit den angegebenen Werten in die Tabelle produkt
      // ein. Falls bereits ein Tupel mit der angegebenen pnr existiert, sollen
      // für dieses Tupel die Attribute name, preis und hnr entsprechend
      // geändert werden.
      if (db_check_pnr(values[1]) == 1)
        db_update_produkt(values[1], values[2], values[3], values[4]);
      else
        db_insert_produkt(values[1], values[2], values[3], values[4]);
    } else if (firstValue == "i") {
      // i hnr hname stadt pnr pname preis
      // Teilt die angegebenen Werte auf die Tabellen hersteller und produkt
      // auf, indem entsprechende Tupel eingefügt bzw. aktualisiert werden.
      if (db_check_hnr(values[1]) == 0)
        db_insert_hersteller(values[1], values[2], values[3]);
      else
        db_update_hersteller(values[1], values[2], values[3]);

      if (db_check_pnr(values[4]) == 0)
        db_insert_produkt(values[4], values[5], values[6], "");
      else
        db_update_produkt(values[4], values[5], values[6], "");
    } else if (firstValue == "dh") {
      // dh hnr
      // Löscht das Tupel mit dem Schlüssel hnr aus der hersteller-Tabelle. Ggf.
      // vorhandene abhängige produkt-Tupel sind ebenfalls zu löschen.
      db_delete_hersteller(values[1]);
    } else if (firstValue == "dp") {
      // dp pnr
      // Löscht das Tupel mit dem Schlüssel pnr aus der produkt-Tabelle.
      db_delete_produkt(values[1]);
    } else if (firstValue == "ch") {
      // dp pnr
      // Gibt die Anzahl der Tupel in der Tabelle hersteller aus.
      cout << db_count("hersteller") << "\n";
    } else if (firstValue == "cp") {
      // dp pnr
      // Gibt die Anzahl der Tupel in der Tabelle produkt aus
      cout << db_count("produkt") << "\n";
    } else {
      cerr << "Unknown command '<<" << firstValue << "'\n";
      return 1;
    }

    /*
    // Die Teilstrings der aktuellen Zeile stehen jetzt in values

    // Debug-Ausgabe, um die Teilstrings anzuzeigen
    std::cout << "line: " << line << endl;
    std::cout << "values.size: " << values.size() << ", values[0]: " <<
    values[0] << endl; ulong  i=0; while (i < values.size()) { std::cout << "
    values[" << i << "]: " << values[i] << endl; i++;
    }
*/

    // Verarbeitung der aktuellen Zeile
    // ...

    // values leer machen für die nächste Zeile
    values.clear();
  }
  db_commit();
  db_logout();
  cout << "Finished!\n";
}
