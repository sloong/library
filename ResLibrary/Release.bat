if exist "%1" ( rd /S /Q "%1" )
md %1
xcopy /e /y "Include" "%1\"
copy ResLibrary\ResLibrary.h %1\
xcopy /e /y "x86" "%1\x86\"
xcopy /e /y "x64" "%1\x64\"
cd %1
del /F /S /Q *.ilk *.exp
cd x86\release\
del /f /s /q *.pdb
cd ..\..\x64\release\
del /f /s /q *.pdb