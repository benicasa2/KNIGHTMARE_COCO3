@echo off
python "%~dp0tools\run_demo.py" --variant ORCH90 %*
if errorlevel 1 pause
