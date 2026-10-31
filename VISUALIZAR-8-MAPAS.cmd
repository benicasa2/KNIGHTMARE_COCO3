@echo off
python "%~dp0tools\run_campaign_viewer.py" %*
if errorlevel 1 pause
