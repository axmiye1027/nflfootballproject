/**
* Schema Definition for NFL Databases (basically creates database)
* Contains three tables: 
*       1. distances
*       2. stadiums
*       3. souvenirs
*/

-- Schema for NFL Distances Database
CREATE TABLE distances (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    location_a TEXT NOT NULL,
    location_b TEXT NOT NULL,
    distance_km INTEGER NOT NULL,
    UNIQUE(location_a, location_b)
);

-- Schema for NFL Stadium Information Database (renamed table and fields clarified)
CREATE TABLE stadiums (
    idStadium    INTEGER PRIMARY KEY AUTOINCREMENT,
    team         TEXT NOT NULL,
    stadium_name TEXT NOT NULL,
    capacity     INTEGER,
    location     TEXT,
    roof_type    TEXT,
    surface      TEXT,
    opened_year  INTEGER,
    conference   TEXT,
    division     TEXT
);

-- Schema for Souvenirs Database
CREATE TABLE souvenirs (
    idSouvenir    INTEGER PRIMARY KEY AUTOINCREMENT,
    souvenirName  TEXT    NOT NULL,
    souvenirPrice DOUBLE  NOT NULL DEFAULT 0.0,
    idStadium     INTEGER NOT NULL, -- Link to its stadium

    FOREIGN KEY (idStadium) REFERENCES stadiums(idStadium)
        ON DELETE CASCADE
)