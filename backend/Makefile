# Compiling main.cpp
# To compile, run:


# In order for Makefile to work make sure you are in the backend directory. Do that by doing:
# cd backend

# Database Commands
# Mainly need to do these commands (recommend order):
#  make db_clean		- deletes the database to reset it
#  make import			- imports data into the database / create database
#  make show-distances  - shows the distances table
#  make show-stadiums   - shows the stadiums table
#  make show-all        - shows all tables
DB := assets/database.db
SCHEMA := assets/schema.sql
IMPORT_SCRIPT := scripts/import_csv.py
PY := python3

.PHONY: all create verify import db_clean show-distances show-stadiums show-all

.PHONY: api-make api-restart api-redeploy api-configure

all: create verify api-make

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
	@echo Running import script: $(q)
	@$(PY) "$(IMPORT_SCRIPT)"

# Removes Database - use this to reset the database
db_clean:
	@echo Removing $(DB)
	@rm -f "$(DB)"

# Run API
api:
	curl -v http://localhost:18080/

api-configure:
	cmake -S . -B build-wsl -DCMAKE_TOOLCHAIN_FILE=../dev/vcpkg/scripts/buildsystems/vcpkg.cmake
	touch build-wsl/config

build-wsl/config: api-configure

api-make: build-wsl/config
	cmake --build build-wsl -- -j4

# restart server
api-restart:
	@echo "Restarting backend server..."
	@# If server.pid exists and references a running process, try graceful stop then force
	@if [ -f server.pid ]; then \
		pid=`cat server.pid 2>/dev/null` ; \
		if [ -n "$$pid" ] && kill -0 $$pid 2>/dev/null; then \
			echo "Stopping pid $$pid" ; \
			kill $$pid 2>/dev/null || true ; \
			sleep 1 ; \
			if kill -0 $$pid 2>/dev/null; then \
				echo "Forcing kill $$pid" ; \
				kill -9 $$pid 2>/dev/null || true ; \
			fi ; \
		fi ; \
		rm -f server.pid ; \
	fi
	@# Start the server in background and save pid
	@nohup ./build-wsl/NFL_PROJECT_NAME > server.log 2>&1 & echo $$! > server.pid ; \
		echo "Started pid:`cat server.pid`" ;

api-redeploy:
	@echo "Rebuild and restart backend"
	@$(MAKE) api-make
	@$(MAKE) api-restart