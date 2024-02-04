@echo off

set CFLAGS=-Wall -Wextra -Wpedantic -O3 -Ilib/glfw/include -Ilib/glew/include -Ilib/cglm/include -IC:/sdk/assimp/include
set LDFLAGS=-Llib/glfw -Llib/glew -Llib/assimp -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mtd
set OUTPUT=bin\Axiom.exe

mkdir obj

for %%i in (src\*.c src\gfx\*.c src\models\*.c src\ui\*.c src\util\*.c src\ui\imgui\*.cpp) do (
    gcc %CFLAGS% -c -o obj\%%~nxi.obj %%i
)

for %%i in (src\*.cpp src\gfx\*.cpp src\models\*.cpp src\ui\*.cpp src\util\*.cpp src\ui\imgui\*.cpp) do (
    gcc %CFLAGS% -c -o obj\%%~nxi.obj %%i
)

gcc -o %OUTPUT% obj\*.obj %LDFLAGS%

del obj\*.obj
rmdir obj

