@echo off
if not exist proj\ (
	echo Creating proj folder
	mkdir proj
)
cd proj
cmake ../
cd ..
pause