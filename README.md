# common
## сборка

1. Клонировать репозиторий
2. В командной строке выполнить `make.bat`, указав путь к глобальной рабочей директории (в ней должен находиться toolchain.cmake)
```
make.bat C:\path\to\workspace
```
В результате будет создана папка  `C:\path\to\workspace\common` со всеми заголовочными файлами

## интеграциия с Cmake
### способ 1
С помощью `include()`
```CMake
# CMakeLists.txt
include(C:/path/to/workspace/toolchain.cmake)
include(C:/path/to/workspace/common/lib/common/common.cmake)


project(my_project) # важно поместить include до project()

include_directories(C:/path/to/workspace)

add_executable(my_target)
target_link_libraries(my_target PUBLIC common)
``` 
Сборка my_project будет выглядеть так
```
cmake -B path/to/build -DOS=arm
cmake --build path/to/build
```

### способ 2
C помощью `find_package()`
```CMake
# CMakeLists.txt
include(${CMAKE_PREFIX_PATH}/toolchain.cmake)

project(my_project) # важно поместить include до project()

include_directories(${CMAKE_PREFIX_PATH})

find_package(common)

add_executable(my_target)
target_link_libraries(my_target PUBLIC common)
``` 
Сборка my_project будет выглядеть так
```
cmake -B path/to/build -DOS=arm -DCMAKE_PREFIX_PATH=C:/path/to/workspace
cmake --build path/to/build
```