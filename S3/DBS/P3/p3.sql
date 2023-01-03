-- psql -U dbs141 -h 194.94.121.224 -p 15434 dbs141
-- \i ~/arman/p3.sql

\echo ####
\echo AUFGABE 1
\echo ####
CREATE TABLE abteilung(
    anr INT PRIMARY KEY,
    name VARCHAR(30)
    );
CREATE TABLE person(
    pnr INT PRIMARY KEY,
    name VARCHAR(30), 
    vorname VARCHAR(30),
    abteilungsnr INT FOREIGN KEY REFERENCES abteilung.anr,
    gehalt NUMERIC(8,2),
    gueltigAb DATE
    );
CREATE TABLE gehaltshistorie(
    id SERIAL, -- SERIAL is an int primary key with auto increment
    pnr INT FOREIGN KEY REFERENCES person.pnr,
    gehaltAlt NUMERIC(8,2),
    gehaltNeu NUMERIC(8,2),
    gueltigAbAlt DATE,
    gueltigAbNeu DATE,
    changed DATE,
    name VARCHAR(30) -- username of database user that performed this change
);

CREATE OR REPLACE FUNCTION check_change()      
LANGUAGE plpgsql  
AS $variable_name$    
DECLARE    
averageGehalt NUMERIC(8,2),   
maxGehalt NUMERIC(8,2)   
 BEGIN    
-- Regel 1: Das Gehalt einer neu eingefügten oder geänderten Person darf um maximal 50%
-- höher sein als der bisherige Durchschnitt in der Abteilung der Person. (Dies gilt
-- nur, wenn es weitere Personen in der gleichen Abteilung gibt).
averageGehalt = SELECT AVG(gehalt) FROM person WHERE abteilungsnr = NEW.abteilungsnr;
maxGehalt = averageGehalt + averageGehalt / 2; -- 50%
IF (NEW.gehalt > maxGehalt) THEN
  RAISE EXCEPTION 'Regel 1! % > %', NEW.gehalt, maxGehalt;
END IF;
-- Regel 2: Das Gehalt einer Person darf nicht reduziert werden.
IF (NEW.gehalt < OLD.gehalt) THEN
  RAISE EXCEPTION 'Regel 2! % < %' NEW.gehalt, OLD.gehalt;
END IF;
-- Regel 3: Das neue Gehalt einer Person darf höchstens 120% ihres bisherigen Gehalts
-- betragen. Sollte das neue Gehalt höher sein, wird es automatisch auf 120% des
-- bisherigen Gehalts gekappt.
maxGehalt = OLD.gehalt + OLD.gehalt / 5; -- 20%
IF (NEW.gehalt > maxGehalt) THEN
  NEW.gehalt = maxGehalt;
END IF;
-- Regel 4: Es dürfen keine rückwirkenden Gehaltsänderungen vorgenommen werden, d.h.
-- das neue gueltigAb-Datum darf nicht zeitlich früher liegen als das bisherige
-- gueltigAb-Datum.  
IF (NEW.gueltigAb < OLD.gueltigAb) THEN
  RAISE EXCEPTION 'Regel 4! % < %' NEW.gueltigAb, OLD.gueltigAb;
END IF;
INSERT INTO gehaltshistorie VALUES(NEW.pnr, OLD.gehalt, NEW.gehalt, OLD.gueltigAb, NEW.gueltigAb, current_date(), current_user());
END;   
CREATE OR REPLACE TRIGGER check_update
    BEFORE UPDATE OR INSERT ON person
    FOR EACH ROW
    EXECUTE FUNCTION check_change();


/* 
   Die folgenden Testfälle sollten Sie nach und nach überprüfen, indem Sie 
   jeweils ein Statement ausführen und alle anderen auskommentieren
*/
INSERT INTO Abteilung VALUES (1, 'Vertrieb'), (2,'Entwicklung');

INSERT INTO Person VALUES (1, 'Flasche', 'Frank', 1, 2000, to_date('01.01.2019','DD.MM.YYYY'));
-- OK

INSERT INTO Person VALUES (2, 'Buechse', 'Bernd', 2, 3200, to_date('01.01.2019','DD.MM.YYYY'));
-- OK

INSERT INTO Person VALUES (3, 'Dose', 'Doris', 1, 3200, to_date('01.01.2019','DD.MM.YYYY'));
-- Wird zurückgewiesen wegen Regel 1

INSERT INTO Person VALUES (4, 'Schachtel', 'Susi', 1, 3000, to_date('01.01.2019','DD.MM.YYYY'));
-- OK

INSERT INTO Person VALUES (3, 'Dose', 'Doris', 1, 3200, to_date('01.01.2019','DD.MM.YYYY'));
-- Jetzt ok wegen veränderten Durchschnitts

UPDATE Person SET gehalt = 3000, gueltigAb = to_date('01.01.2020','DD.MM.YYYY') WHERE pnr = 3;
-- Wird zurückgewiesen wegen Regel 2

UPDATE Person SET gehalt = 3400, gueltigAb = to_date('01.01.2018','DD.MM.YYYY') WHERE pnr = 3;
-- Wird zurückgewiesen wegen Regel 4

UPDATE Person SET gehalt = 3400, gueltigAb = to_date('01.01.2020','DD.MM.YYYY') WHERE pnr = 3;
-- OK → Update und neuer Eintrag in Gehaltshistorie

UPDATE Person SET gehalt = 4500, gueltigAb = to_date('01.01.2021','DD.MM.YYYY') WHERE pnr = 3;
-- Wird zurückgewiesen wegen Regel 1

UPDATE Person SET gehalt = 4200, gueltigAb = to_date('01.01.2021','DD.MM.YYYY') WHERE pnr = 3;
-- Gehalt wird auf 4080 gekappt wegen Regel 3,
-- OK → Update und neuer Eintrag in Gehaltshistorie

\echo ####
\echo AUFGABE 2
\echo ####


DROP TABLE abteilung;
DROP TABLE person;
DROP TABLE gehaltshistorie;
