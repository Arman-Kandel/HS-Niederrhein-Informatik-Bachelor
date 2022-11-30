#include <iostream>
#include <libpq-fe.h>
#include <stdio.h>
#include <string>

using namespace std;

// ----------------------
// Datenbank-Login/Logout
// ----------------------
PGconn *conn;
PGresult *res;

// Datenbank-Login
// rc: 0 = ok, -1 = error
int db_login(const string &user, const string &password, const string &host,
             const string &port, const string &dbname) {
  cout << "LOGIN"
       << "\n";
  string strConnection =
      "host=" + host + " port=" + port + " user=" + user + " dbname=" + dbname;
  if (!password.empty())
    strConnection = strConnection + " password=" + password;
  cout << "Connecting with: " << strConnection << "\n";
  char *pcon = const_cast<char *>(strConnection.c_str());
  conn = PQconnectdb(pcon);
  cout << "\n";

  // Check to see that the backend connection was successfully made
  if (PQstatus(conn) != CONNECTION_OK) {
    cout << "Connection to database failed: " << PQerrorMessage(conn) << "\n";
    PQfinish(conn);
    return -1;
  } else {
    cout << "Connected to database.\n";
    return 0;
  }
}

// Datenbank-Logout
void db_logout() {
  cout << "LOGOUT"
       << "\n";
  if (conn == NULL) {
    cout << "Logout failed because there is no connection.\n";
    return;
  }
  PQfinish(conn);
}

// -------------------
// Transaktionsbefehle
// -------------------

// rc: 0 = ok, -1 = error
int db_begin() {
  cout << "BEGIN"
       << "\n";
  if (conn == NULL) {
    cout << "BEGIN transaction failed because there is no connection.\n";
    return -1;
  }
  res = PQexec(conn, "BEGIN");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "BEGIN transaction failed because: " << PQerrorMessage(conn)
         << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}
int db_commit() {
  cout << "COMMIT"
       << "\n";
  res = PQexec(conn, "COMMIT");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}
int db_rollback() {
  res = PQexec(conn, "ROLLBACK");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// ----------------------------------
// DB-Schema anlegen und zurücksetzen
// ----------------------------------

// Hersteller-Tabelle anlegen
// rc: 0 = ok, -1 = error
int db_create_table_hersteller() {
  string query = "CREATE TABLE hersteller (hnr varchar(4) PRIMARY KEY, name "
                 "varchar(30), stadt varchar(30))";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Produkt-Tabelle anlegen
// rc: 0 = ok, -1 = error
int db_create_table_produkt() {
  string query = "CREATE TABLE produkt (pnr varchar(4) PRIMARY KEY, name "
                 "varchar(30), preis numeric(8,2), hnr varchar(4))";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Tabelle tablename löschen und aus DB-Schema entfernen
// rc: 0 = ok, -1 = error
int db_drop_table(const string &tablename) {
  string query = "DROP TABLE " + tablename;
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// -------------------------------------
// Existenz/Anzahl von Tupel zurückgeben
// -------------------------------------

// Prüfen, ob hnr in Hersteller schon vorhanden ist?
// rc: 0 = noch nicht da, 1 = schon da, -1 = error
int db_check_hnr(const string &hnr) {
  string query = "SELECT * FROM hersteller WHERE hnr='" + hnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  int rows = PQntuples(res);
  if (rows > 0)
    return 1;
  else
    return 0;
}

// Prüfen, ob pnr in Produkt schon vorhanden ist?
// rc: 0 = noch nicht da, 1 = schon da, -1 = error
int db_check_pnr(const string &pnr) {
  string query = "SELECT * FROM produkt WHERE pnr='" + pnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  int rows = PQntuples(res);
  if (rows > 0)
    return 1;
  else
    return 0;
}

// Anzahl der Tupel in der Tabelle tablename zurückgeben
// rc: n = Anzahl der Tupel (n>=0), -1 = error
int db_count(const string &tablename) {
  string query = "SELECT COUNT(*) FROM " + tablename;
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  return PQntuples(res);
}

// -------------------------------
// Tupel einfügen, ändern, löschen
// -------------------------------

// Einfuegen Produkt
// rc: 0 = ok, -1 = error
int db_insert_produkt(const string &pnr, const string &name,
                      const string &preis, const string &hnr) {
  string query = "INSERT INTO produkt (pnr, name, preis, hnr) VALUES ('" + pnr +
                 "', '" + name + "', '" + preis + "', '" + hnr + "')";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Einfuegen Hersteller
// rc: 0 = ok, -1 = error
int db_insert_hersteller(const string &hnr, const string &name,
                         const string &stadt) {
  string query = "INSERT INTO hersteller (hnr, name, stadt) VALUES ('" + hnr +
                 "', '" + name + "', '" + stadt + "')";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Ändern Produkt
// rc: 0 = ok, -1 = error
int db_update_produkt(const string &pnr, const string &name,
                      const string &preis, const string &hnr) {
  string query = "UPDATE produkt SET pnr='" + pnr + "', name='" + name +
                 "', preis='" + preis + "', hnr='" + hnr + "' WHERE pnr='" +
                 pnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Ändern Hersteller
// rc: 0 = ok, -1 = error
int db_update_hersteller(const string &hnr, const string &name,
                         const string &stadt) {
  string query = "UPDATE hersteller SET hnr='" + hnr + "', name='" + name +
                 "', stadt='" + stadt + "' WHERE hnr='" + hnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Löschen Produkt
// rc: 0 = ok, -1 = error
int db_delete_produkt(const string &pnr) {
  string query = "DELETE FROM produkt WHERE pnr='" + pnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Löschen Hersteller und aller abhängigen Produkte
// rc: 0 = ok, -1 = error
int db_delete_hersteller(const string &hnr) {
  string query = "DELETE FROM hersteller WHERE hnr='" + hnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  query = "DELETE FROM produkt WHERE hnr='" + hnr + "'";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}

// Loeschen des kompletten Tabelleninhalts beider Tabellen
// rc: 0 = ok, -1 = error
int db_delete() {
  string query = "DROP TABLE IF EXISTS produkt";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  query = "DROP TABLE IF EXISTS hersteller";
  cout << query << "\n";
  res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cout << "FAILED! " << PQerrorMessage(conn) << "\n";
    PQclear(res);
    return -1;
  }
  PQclear(res);
  return 0;
}
