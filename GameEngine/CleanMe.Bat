rmdir /S /Q Debug
rmdir /S /Q Release
del /F /S *.sdf
del /F /S /A:H *.suo

rmdir /S /Q GameEngine\Debug
rmdir /S /Q MathEngine\Debug
rmdir /S /Q File\Debug
rmdir /S /Q src\Object\Debug