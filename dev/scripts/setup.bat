@echo off
REM -------------------------
REM Windows wrapper to run the cross-platform setup script
REM -------------------------

REM Find the directory of this batch file
SET SCRIPT_DIR=%~dp0

REM Call Git Bash to run the Bash script
REM Adjust path if your team has Git Bash installed elsewhere
"C:\Program Files\Git\bin\bash.exe" "%SCRIPT_DIR%setup"

pause
