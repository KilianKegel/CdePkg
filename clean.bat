@echo off
rem
rem Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
rem SPDX-License-Identifier: BSD-2-Clause-Patent
rem
echo clean up...

if exist %~p0Debug rd /s /q %~p0Debug
if exist %~p0x64 rd /s /q %~p0x64

rem for /F  %%d in ('dir /b /AD') do if exist %%d\doxygen.tmp rd /s /q %%d\doxygen.tmp
rem for /F  %%d in ('dir /b /AD') do if exist %%d\Release rd /s /q %%d\Release
rem 
rem for  %%a in (Win32 x64) do (
rem     for /F %%b in ('@dir %%a /s /AD /b ^| find /V /I "CdeLib\x64"') do echo %%b & rd /s /q %%b
rem )

pushd ..
for /F "tokens=1-4 delims==," %%a in ('type CdePkg.sln ^| find ".vcxproj"') do (
    for  %%o in (%%c) do (
        echo %%~po
        if exist %%~po\x64 rd /s /q %%~po\x64
        if exist %%~po\win32 rd /s /q %%~po\win32
        if exist %%~po\doxygen.tmp rd /s /q %%~po\doxygen.tmp
        if exist %%~po\SHELL rd /s /q %%~po\SHELL
        if exist %%~po\POST rd /s /q %%~po\POST

    )
)
popd
for /F  %%d in ('dir /b /AD') do if exist %%d\doxygen.tmp rd /s /q %%d\doxygen.tmp

if exist build rd /s /q build

ping 127.0.0.0 -n 2 > nul
