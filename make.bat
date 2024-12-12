set workspaceDirectory=%1

if exist %workspaceDirectory%\common\ (
    RD /S "%workspaceDirectory%\common"
)

if exist %~dp0\build\ (
    RD /S "%~dp0\build\"
)

cmake -B %~dp0/build -G "MinGW Makefiles" -DOS=arm -DCMAKE_PREFIX_PATH="%workspaceDirectory%"
cmake --build %~dp0/build
cmake --install %~dp0/build --prefix %workspaceDirectory%
