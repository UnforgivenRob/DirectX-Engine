rmdir /S /Q Debug
rmdir /S /Q Release
del /F /S *.sdf
del /F /S /A:H *.suo

rmdir /S /Q Exporter\Debug
rmdir /S /Q Exporter\bin
rmdir /S /Q Exporter\obj
rmdir /S /Q src\Object\Debug