@echo off
echo Running Windows setup...

REM Execute PowerShell script with unrestricted permission just for this run
powershell -ExecutionPolicy Bypass -File "%~dp0setup.ps1"

pause
