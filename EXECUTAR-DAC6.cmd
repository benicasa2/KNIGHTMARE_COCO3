@echo off
python "%~dp0tools\run_demo.py" --variant DAC6 %*
if errorlevel 1 pause
