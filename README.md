Please make sure you are in the correct folder or else things like the Makefile will not work!

    cd backend      - makes
    cd -            - takes you back to previous folder

"Ctrl" + "C"    - quit task, use if you are unable to stop something from running or cant exit

- - -

Create Database Tables

1. make db_clean        - removes database file (only do if errors with database or want to restart)
2. make import          - creates databse file and imports our data to do


To See Database:
database.db file must exist in order to see tables

make show-all           - shows all tables 
make show-distances     - shows distance table
make show-stadiums      - shows stadium information

- - -

Frontend Notes
    
    - must insteall Node.js to use and run

Run Frontend:
    npm run dev
    
- - -


BACKEND TABLE OF CONTENTS

    database Folder

        database.db
    Our database

        schema.sql
    creates tables for our database

        scripts folder -> import_csv.py
    connects the csv folders to our tables (schema.sql)

- - -


FRONT END TABLE OF CONTENTS

    App.js
    main page, basically index.html but for react

    App.css
    main styles, mostly global styles or styles 

    componenets folder
    where all reusable functions are stored
