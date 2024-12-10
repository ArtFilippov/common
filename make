# bash

prefix="/home/kali/"

buildDir="~/common_build"

cmake -B "$buildDir" -DOS=linux -DCMAKE_PREFIX_PATH="$prefix"
cmake --build "$buildDir"
cmake --install "$buildDir" --prefix "$prefix"