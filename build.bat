
@echo off

set DEBUG_CFLAGS=-Wall -g -IC:/sdk/glfw-64/include -IC:/sdk/glew-2.1.0/include -IC:/sdk/cglm/include -IC:/sdk/assimp/include
set RELEASE_CFLAGS=-O3
set LDFLAGS=-LC:/sdk/glfw-64/lib-mingw-w64 -LC:/sdk/glew-2.1.0/lib/Release/x64 -LC:/sdk/Assimp/lib/Debug -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mtd
set OUTPUT=bin\main.exe

REM Create a directory to store object files
mkdir obj

REM Compile C files
for %%i in (src\*.c src\gfx\*.c src\models\*.c src\ui\*.c src\util\*.c src\ui\imgui\*.cpp) do (
    g++ %DEBUG_CFLAGS% -c -o obj\%%~nxi.obj %%i
)

REM Compile C++ files
for %%i in (src\*.cpp src\gfx\*.cpp src\models\*.cpp src\ui\*.cpp src\util\*.cpp src\ui\imgui\*.cpp) do (
    g++ %DEBUG_CFLAGS% -c -o obj\%%~nxi.obj %%i
)

REM Link object files
g++ -o %OUTPUT% obj\*.obj %LDFLAGS%

REM Clean up object files
del obj\*.obj
rmdir obj

