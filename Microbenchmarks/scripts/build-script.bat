Rem Run this script from the scripts folder
Rem GCC configure -O0
cmake --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING="-O0 -msse" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DBENCHMARK_ENABLE_TESTING:BOOL=FALSE -S.. -B../build/gccO0 -G "MinGW Makefiles"

Rem Clang configure -O0
cmake --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING="-O0 -msse" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DBENCHMARK_ENABLE_TESTING:BOOL=FALSE -S.. -B../build/clangO0 -G "MinGW Makefiles"

Rem GCC configure -O2
cmake --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING="-O2 -msse" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DBENCHMARK_ENABLE_TESTING:BOOL=FALSE -S.. -B../build/gccO2 -G "MinGW Makefiles"

Rem Clang configure -O2
cmake --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING="-O2 -msse" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DBENCHMARK_ENABLE_TESTING:BOOL=FALSE -S.. -B../build/clangO2 -G "MinGW Makefiles"

Rem Build
cmake --build ../build/gccO0 --config Debug --target all -j 10 --
cmake --build ../build/clangO0 --config Debug --target all -j 10 --
cmake --build ../build/gccO2 --config Debug --target all -j 10 --
cmake --build ../build/clangO2 --config Debug --target all -j 10 --

Rem Compiler path options
Rem ... -DCMAKE_C_COMPILER:FILEPATH=D:\msys64\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=D:\msys64\mingw64\bin\g++.exe ...

Rem Cmake path
Rem D:\CMake\bin\cmake.EXE ...