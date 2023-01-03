-- psql -U dbs141 -h 194.94.121.224 -p 15434 dbs141
-- \i ~/arman/a1.4.sql
\echo
\echo
\echo
\echo
\echo ####
\echo 1.4
\echo ####
CREATE TABLE abteilung(
nr VARCHAR(2) PRIMARY KEY, -- == abeteilungnr in person
name VARCHAR NOT NULL -- == abteilungname in import
);
CREATE TABLE person(
nr INT PRIMARY KEY,
geburt DATE NOT NULL,
name VARCHAR NOT NULL,
vorname VARCHAR NOT NULL,
abteilungnr VARCHAR(2) NOT NULL,
gehalt DECIMAL NOT NULL,
ort VARCHAR NOT NULL,
FOREIGN KEY(abteilungnr) REFERENCES abteilung(nr)
);

CREATE TABLE import(
nr VARCHAR,
ort VARCHAR,
geburt VARCHAR,
name VARCHAR,
vorname VARCHAR,
abteilungname VARCHAR,
abteilungnr VARCHAR,
gehalt VARCHAR
);

\echo Importiere aus .csv in import table:
\encoding utf8
\copy import from '~/Downloads/dbsnam-utf8.csv' delimiter ';'
SELECT COUNT(nr) FROM import;
SELECT * FROM import WHERE nr='0002'; -- Umlaut von Mönchengladbach checken

INSERT INTO abteilung(nr, name)
SELECT DISTINCT abteilungnr, abteilungname
FROM import;
SELECT COUNT(nr) FROM abteilung;

\echo Setzte daten aus import table in person und abteilung tables:
INSERT INTO person (nr,geburt,name,vorname,abteilungnr,gehalt,ort)
SELECT CAST(nr AS INT), TO_DATE(geburt, 'DD.MM.YYYY'),name,vorname,abteilungnr,CAST(gehalt AS DECIMAL),ort
FROM import;
SELECT COUNT(nr) FROM person;
SELECT * FROM person WHERE nr='0002';


\echo ####
\echo 1.5
\echo ####
INSERT INTO person
VALUES (1001, TO_DATE('16.03.1988', 'DD.MM.YYYY'), 'Beckers', 'Melina', 'C3', 3850.0, 'Viersen'); 
\echo Entlasse Duisburg Entwicklung Mitarbeiter
DELETE FROM person WHERE ort='Duisburg' AND abteilungnr='C1'; -- C1 ==Entwicklung
\echo Erhöhe Gehalt um 10% für alle mit Gehalt unter 3k
UPDATE person
SET gehalt = gehalt + (gehalt/100)*10
WHERE gehalt < 3000.0; 


\echo Mitarbeiter geboren nach 1970:
SELECT COUNT(nr) FROM person WHERE geburt > TO_DATE('31.12.1970', 'DD.MM.YYYY');


\echo Abteilung von Michael Schubert:
SELECT person.name, abteilung.name
FROM person
INNER JOIN abteilung
ON abteilung.nr=person.abteilungnr AND person.name='Schubert' AND person.vorname='Michael';


\echo Min,Max und Durchschnitssgehalt je Abteilung:
SELECT 
abteilung.name, 
ROUND(MIN(person.gehalt), 2) AS mein_min, 
ROUND(MAX(person.gehalt), 2) AS mein_max,
ROUND(AVG(person.gehalt),2) AS mein_avg
FROM person
INNER JOIN abteilung
ON abteilung.nr=person.abteilungnr
GROUP BY abteilung.name
;


DROP TABLE person,abteilung,import;


