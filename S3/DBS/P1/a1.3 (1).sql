-- psql -U dbs141 -h 194.94.121.224 -p 15434 dbs141
-- \i ~/arman/a1.3.sql
CREATE TABLE mytable(id INT PRIMARY KEY);
INSERT INTO mytable(id) VALUES (1);
INSERT INTO mytable(id) VALUES (2);
INSERT INTO mytable(id) VALUES (3);
SELECT * FROM mytable;
\dt mytable
DROP TABLE mytable
