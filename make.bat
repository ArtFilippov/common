set workspaceDirectory=%1

if exist %workspaceDirectory%\common\ (
    RD /S "%workspaceDirectory%\common"
)

if exist %cd%\build\ (
    RD /S "%cd%\build\"
)

cmake -B ./build -G "MinGW Makefiles" -DOS=arm -DCMAKE_PREFIX_PATH="%workspaceDirectory%"
cmake --build ./build 
cmake --install ./build --prefix %workspaceDirectory%
