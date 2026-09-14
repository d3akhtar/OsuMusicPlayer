@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

SET includes=/Isrc /Iinclude
SET links=/link /LIBPATH:lib/windows raylib.lib opengl32.lib gdi32.lib winmm.lib user32.lib shell32.lib /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd

echo "Building..."

cl /EHsc /std:c17 %includes% src/*.c %links% /SUBSYSTEM:CONSOLE
