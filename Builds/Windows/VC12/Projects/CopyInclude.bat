@echo off
::
:: This batch file is used to copy files from a project into the 
:: Include directory. It also marks those files read only.
::

:: FIND THE INCLUDE DIRECTORY 
:: Prefer the most local include dir, 
:: so order these checks such that local dirs set the variable last.
set INCLUDE_DIR=not_found
if exist ..\..\..\..\Library\%~1\%~1       set INCLUDE_DIR=..\..\..\..\Library\%~1\%~1
if (%INCLUDE_DIR%)==(not_found) goto :ERROR_NO_INCLUDE

echo (only copying new or modified files)

:: COPY STANDARD FILES
if exist ..\..\..\..\Source\Common\%~1\*.h   xcopy /D /F /I /R /Y ..\..\..\..\Source\Common\%~1\*.h   %INCLUDE_DIR%\
if exist ..\..\..\..\Source\Common\%~1\*.inl xcopy /D /F /I /R /Y ..\..\..\..\Source\Common\%~1\*.inl %INCLUDE_DIR%\
if exist ..\..\..\..\Source\Common\%~1\*.hpp xcopy /D /F /I /R /Y ..\..\..\..\Source\Common\%~1\*.hpp %INCLUDE_DIR%\

:: MARK FILES READ ONLY
if exist %INCLUDE_DIR%\ attrib +R %INCLUDE_DIR%\*

exit /b

:==ERROR_NO_INCLUDE
echo %0: fatal error: Cannot locate Include directory.
exit 1
