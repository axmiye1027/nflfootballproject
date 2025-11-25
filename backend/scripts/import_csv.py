#!/usr/bin/env python3
# Allows importing CSV files into the existing SQLite database
# Basically hooks up csv files to the database
"""Import CSV files into the existing SQLite database.

Usage: run from project root (WSL):
    python3 backend/scripts/import_csv.py

This script is intentionally defensive: it cleans numbers, trims whitespace,
ignores bad rows, uses transactions, and uses INSERT OR IGNORE to avoid
violating UNIQUE constraints.
"""
from __future__ import annotations
import csv
import sqlite3
import os
import sys
from pathlib import Path

BASE = Path(__file__).resolve().parent.parent
DB_PATH = BASE / 'assets' / 'database.db'
DIST_CSV = BASE / 'assets' / 'csv' / 'NFL Distances.csv'
INFO_CSV = BASE / 'assets' / 'csv' / 'NFL Information.csv'


def clean_int(s: str | None) -> int | None:
    if s is None:
        return None
    s2 = str(s).strip().replace('"', '').replace(',', '')
    if s2 == '':
        return None
    try:
        return int(float(s2))
    except Exception:
        return None


def clean_float(s: str | None) -> float | None:
    if s is None:
        return None
    s2 = str(s).strip().replace('"', '').replace(',', '')
    if s2 == '':
        return None
    try:
        return float(s2)
    except Exception:
        return None


def clean_text(s: str | None) -> str | None:
    if s is None:
        return None
    # collapse whitespace and remove non-breaking spaces
    return ' '.join(str(s).replace('\u00A0', ' ').split()).strip() or None


def import_distances(conn: sqlite3.Connection, csv_path: Path) -> int:
    cur = conn.cursor()
    inserted = 0
    with csv_path.open(newline='', encoding='utf-8') as fh:
        reader = csv.DictReader(fh)
        rows = []
        for r in reader:
            a = clean_text(r.get('Beginning Stadium') or r.get('Beginning') or r.get('Beginning_Stadium'))
            b = clean_text(r.get('Ending Stadium') or r.get('Ending') or r.get('Ending_Stadium'))
            d = clean_float(r.get('Distance') or r.get('Distance_km') or r.get('distance'))
            if not a or not b or d is None:
                # skip rows missing required data
                continue
            rows.append((a, b, d))
    if rows:
        cur.executemany(
            'INSERT OR IGNORE INTO distances (location_a, location_b, distance_km) VALUES (?,?,?)',
            rows,
        )
        inserted = cur.rowcount if cur.rowcount is not None else len(rows)
        conn.commit()
    return inserted


def import_stadiums(conn: sqlite3.Connection, csv_path: Path) -> int:
    cur = conn.cursor()
    rows = []
    with csv_path.open(newline='', encoding='utf-8') as fh:
        reader = csv.DictReader(fh)
        # normalize field names (strip whitespace and BOM) per-row to be robust
        for r in reader:
            normalized = {}
            for k, v in r.items():
                if k is None:
                    continue
                nk = k.strip().lstrip('\ufeff')
                normalized[nk] = v
            # prefer normalized keys
            team = clean_text(normalized.get('Team(s)') or normalized.get('Team') or normalized.get('Team Name'))
            stadium = clean_text(normalized.get('Name') or normalized.get('Stadium') or normalized.get('stadium_name'))
            capacity = clean_int(r.get('Capacity'))
            location = clean_text(r.get('Location'))
            roof = clean_text(r.get('Roof Type') or r.get('Roof'))
            surface = clean_text(r.get('Surface'))
            opened = clean_int(r.get('Opened') or r.get('Opened Year') or r.get('Opened_Year'))
            conference = clean_text(r.get('Conference'))
            division = clean_text(r.get('Division'))
            if not stadium:
                # stadium name required for stadiums table
                continue
            # ensure team is not NULL (schema requires NOT NULL); fallback to stadium's name if missing
            if not team:
                team = stadium
            rows.append((team, stadium, capacity, location, roof, surface, opened, conference, division))
    inserted = 0
    if rows:
        cur.executemany(
            '''INSERT OR IGNORE INTO stadiums
            (team, stadium_name, capacity, location, roof_type, surface, opened_year, conference, division)
            VALUES (?,?,?,?,?,?,?,?,?)''',
            rows,
        )
        inserted = cur.rowcount if cur.rowcount is not None else len(rows)
        conn.commit()
    return inserted


def main() -> None:
    if not DB_PATH.exists():
        print('ERROR: database file not found at', DB_PATH)
        sys.exit(2)

    conn = sqlite3.connect(str(DB_PATH))
    conn.execute('PRAGMA foreign_keys = ON;')

    print('Importing distances from', DIST_CSV)
    d_count = import_distances(conn, DIST_CSV)
    print(f'Inserted (attempted) {d_count} distance rows')

    print('Importing stadiums from', INFO_CSV)
    s_count = import_stadiums(conn, INFO_CSV)
    print(f'Inserted (attempted) {s_count} stadium rows')

    cur = conn.cursor()
    for t in ('distances', 'stadiums'):
        cur.execute(f'SELECT COUNT(*) FROM {t};')
        print(f'{t}:', cur.fetchone()[0])

    conn.close()


if __name__ == '__main__':
    main()
