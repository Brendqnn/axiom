@echo off

set CFLAGS=-Wall -O3 -Ilib/glfw/include -Ilib/glew/include -Ilib/cglm/include -IC:/sdk/assimp/include
set LDFLAGS=-Llib/glfw -Llib/glew -Llib/assimp -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mtd
set OUTPUT=bin\Axiom.exe

REM Create a directory to store object files
mkdir obj

REM Compile C files
for %%i in (src\*.c src\gfx\*.c src\models\*.c src\ui\*.c src\util\*.c src\ui\imgui\*.cpp) do (
    g++ %CFLAGS% -c -o obj\%%~nxi.obj %%i
)

REM Compile C++ files
for %%i in (src\*.cpp src\gfx\*.cpp src\models\*.cpp src\ui\*.cpp src\util\*.cpp src\ui\imgui\*.cpp) do (
    g++ %CFLAGS% -c -o obj\%%~nxi.obj %%i
)

REM Link object files
g++ -o %OUTPUT% obj\*.obj %LDFLAGS%

REM Clean up object files
del obj\*.obj
rmdir obj

