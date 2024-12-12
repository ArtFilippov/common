set workspaceDirectory=%1

if exist %workspaceDirectory%\common\ (
    RD /S "%workspaceDirectory%\common"
)

if exist %~dp0\build\ (
    RD /S "%~dp0\build\"
)

cmake -B ./build -G "MinGW Makefiles" -DOS=arm -DCMAKE_PREFIX_PATH="%workspaceDirectory%"
cmake --build ./build 
cmake --install ./build --prefix %workspaceDirectory%
