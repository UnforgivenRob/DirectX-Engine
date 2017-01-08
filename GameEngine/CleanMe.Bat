rmdir /S /Q Debug
rmdir /S /Q Release
rmdir /S /Q ipch
del /F /S *.sdf
del /F /S /A:H *.suo
del /F /S "Game Engine.VC"

rmdir /S /Q GameEngine\Debug
rmdir /S /Q MathEngine\Debug
rmdir /S /Q Time\Debug
rmdir /S /Q File\Debug
rmdir /S /Q src\Object\Debug