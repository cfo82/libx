@echo off
set sourcedir=%1%
set destdir=%2%
echo Installing header files from "%sourcedir%" into "%destdir%"
if not exist %destdir% mkdir %destdir%
copy "%sourcedir%zlib.h" "%destdir%" /Y
copy "%sourcedir%zconf.h" "%destdir%" /Y