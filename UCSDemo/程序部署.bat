del /s /Q %cd%\depoly
cd %cd%
rd /s /q %cd%\depoly
mkdir %cd%\depoly
copy %cd%\Bin\YzxConfComm.exe %cd%\depoly\
windeployqt.exe %cd%\depoly\YzxConfComm.exe
pause