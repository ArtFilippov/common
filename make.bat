set workspaceDirectory=%1

if exist %workspaceDirectory%\common\ (
    RD /S "%workspaceDirectory%\common"
)

if exist %~dp0build\ (
    RD /S "%~dp0build\"
)

cmake %~dp0 -B %~dp0build -G "MinGW Makefiles" -DOS=arm -DCMAKE_PREFIX_PATH="%workspaceDirectory%"
cmake %~dp0 --build %~dp0build
cmake %~dp0 --install %~dp0build --prefix %workspaceDirectory%
