copy %cd%\Bin\UCSDemo.exe %cd%\depoly\
copy %cd%\Bin\*.json %cd%\depoly\
copy %cd%\Bin\*.dll %cd%\depoly\
copy %cd%\Bin\*.exp %cd%\depoly\
copy %cd%\Bin\*.lib %cd%\depoly\
windeployqt.exe %cd%\depoly\UCSDemo.exe
pause