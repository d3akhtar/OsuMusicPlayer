@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

SET includes=/Isrc /Iinclude
SET links=/link /LIBPATH:lib/windows raylib.lib opengl32.lib gdi32.lib winmm.lib user32.lib shell32.lib ole32.lib comdlg32.lib  /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd

echo "Building tests..."

cl /Fe omp_tests.exe /EHsc /std:c17 %includes% tests/*.c ./src/*.c external/*.c %links% /SUBSYSTEM:CONSOLE
