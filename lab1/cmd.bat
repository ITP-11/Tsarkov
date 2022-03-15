@echo off
set /p d="Nazvanie diska: "
set "vrf=bak,tmp"
for %%a in (%vrf%) do for /f "delims= eol=" %%b in ('2^>nul dir /a-d/b/s "%d%:\*.%%a"') do echo del  "%%b"
Pause