rmdir /s /q $(SolutionDir)Include
mkdir $(SolutionDir)Include
mkdir $(SolutionDir)Include\$(PlatformTarget)
copy "$(ProjectDir)*.h" "$(SolutionDir)Include\*.h"
del "$(SolutionDir)Include\resource.h"
del "$(SolutionDir)Include\stdafx.h"
del "$(SolutionDir)Include\targetver.h"
del "$(SolutionDir)Include\version.h"
copy "$(OutDir)*.lib" "$(SolutionDir)Include\$(PlatformTarget)\*.lib"
copy "$(OutDir)*.dll" "$(SolutionDir)Include\$(PlatformTarget)\*.dll"