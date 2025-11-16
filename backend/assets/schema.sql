/**
* Schema Definition for NFL Databases (basically creates database)
* Contains two tables: 
*       1. distances
*       2. stadiums
*/

-- Schema for NFL Distances Database
CREATE TABLE distances (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    location_a TEXT NOT NULL,
    location_b TEXT NOT NULL,
    distance_km REAL NOT NULL,
    UNIQUE(location_a, location_b)
);

-- Schema for NFL Stadium Information Database (renamed table and fields clarified)
CREATE TABLE stadiums (
    idStadium INTEGER PRIMARY KEY AUTOINCREMENT,
    team TEXT NOT NULL,
    stadium_name TEXT NOT NULL,
    capacity INTEGER,
    location TEXT,
    roof_type TEXT,
    surface TEXT,
    opened_year INTEGER,
    conference TEXT,
    division TEXT
);