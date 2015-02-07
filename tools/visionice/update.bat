@echo off
set InstallPath=C:\ESTII

if NOT EXIST %InstallPath% goto EST_ERROR

:DIREXISTS
echo Begining Update......
echo Backing up the existing files to be replaced....
if NOT EXIST %InstallPath%\simfiles\bcm4710.dtl goto C1
copy %InstallPath%\simfiles\bcm4710.dtl %InstallPath%\simfiles\bcm4710.dtl.old
:C1
if NOT EXIST %InstallPath%\bitmap\bcm4710.ini goto C2
copy %InstallPath%\bitmap\bcm4710.ini %InstallPath%\bitmap\bcm4710.ini.old
:C2
if NOT EXIST %InstallPath%\regfiles\MIPS\bcm4710.reg goto C3
copy %InstallPath%\regfiles\MIPS\bcm4710.reg  %InstallPath%\regfiles\MIPS\bcm4710.reg.old
:C3
if NOT EXIST %InstallPath%\regfiles\MIPS\bcm4710_r4.reg goto C4
copy %InstallPath%\regfiles\MIPS\bcm4710_r4.reg  %InstallPath%\regfiles\MIPS\bcm4710_r4.reg.old
:C4
if NOT EXIST %InstallPath%\targets.def goto C5
copy %InstallPath%\targets.def   %InstallPath%\targets.def.old
:C5

copy bcm4710.dtl %InstallPath%\simfiles 1>nul
copy bcm4710.ini %InstallPath%\bitmap   1>nul
copy bcm4710.REG %InstallPath%\regfiles\MIPS   1>nul
copy bcm4710_R4.REG %InstallPath%\regfiles\MIPS   1>nul
copy targets.def %InstallPath%  1>nul

echo BCM4710 files updated sucessfully.
echo goodbye!!!
%SystemDrive%
goto TERMINATE

:EST_ERROR
echo The directory %InstallPath% does not exist!!!
echo If you do not have the visionICE software installed
echo please do so now and then run this batch file.  
echo If the software is installed in another location, please
echo modify the line that begins with "set InstallPath=" in
echo the UPDATE.BAT file
goto TERMINATE

:TERMINATE
