@echo off
cd /d "%~dp0"
python tools\run_ssc.py %*
if errorlevel 1 pause
