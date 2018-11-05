@echo off
set sourcedir=%1%
set destdir=%2%
echo Installing shader files from "%sourcedir%" into "%destdir%"
if not exist %destdir% mkdir %destdir%
copy "%sourcedir%*.cg" "%destdir%" /Y
