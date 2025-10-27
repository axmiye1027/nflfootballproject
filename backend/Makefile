# In order for Makefile to work make sure you are in the backend directory. Do that by doing:
# cd backend

# Database Commands
# Mainly need to do these commands:
#  make show-distances  - shows the distances table
#  make show-stadiums   - shows the stadiums table
#  make show-all        - shows all tables
#  make db_clean		- deletes the database to reset it
#  make import			- imports data into the database / create database
DB := database/database.db
SCHEMA := database/schema.sql
IMPORT_SCRIPT := scripts/import_csv.py
PY := python3

# Shows Database Tables
# to only show a limited amount of data add this to the end before the semicolon:
# LIMIT 5
show-distances:
	@echo "Showing NFL Distance Table"
	@sqlite3 -header -column "$(DB)" "SELECT * FROM distances;"

show-stadiums:
	@echo "Showing NFL Stadium Table"
	@sqlite3 -header -column "$(DB)" "SELECT * FROM stadiums;"

show-all: show-distances show-stadiums
	@echo "Showing all tables"

.PHONY: all create verify import clean show-distances show-stadiums show-all

all: create verify

create:
	@echo Creating $(DB) from $(SCHEMA)
	@if [ -f "$(DB)" ]; then \
		echo "$(DB) exists, skipping create"; \
	else \
		sqlite3 "$(DB)" < "$(SCHEMA)"; \
	fi

verify:
	@echo Tables in $(DB):
	@sqlite3 "$(DB)" "SELECT name FROM sqlite_master WHERE type='table';"

import: create
	@echo Running import script: $(IMPORT_SCRIPT)
	@$(PY) "$(IMPORT_SCRIPT)"

# Removes Database - use this to reset the database
db_clean:
	@echo Removing $(DB)
	@rm -f "$(DB)"