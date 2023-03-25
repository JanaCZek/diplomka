Rem GCC configure -O0
D:\CMake\bin\cmake.EXE --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING=-O0 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=D:\msys64\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=D:\msys64\mingw64\bin\g++.exe -SD:/Projekty/diplomka/Microbenchmarks -Bd:/Projekty/diplomka/Microbenchmarks/build -G "MinGW Makefiles"

Rem Build
D:\CMake\bin\cmake.EXE --build d:/Projekty/diplomka/Microbenchmarks/build --config Debug --target two-d-array-iteration matrix-multiplication-variations -j 10 --

Rem Clang configure -O0
D:\CMake\bin\cmake.EXE --no-warn-unused-cli -DCMAKE_CXX_FLAGS:STRING=-O0 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=D:\msys64\mingw64\bin\clang.exe -DCMAKE_CXX_COMPILER:FILEPATH=D:\msys64\mingw64\bin\clang++.exe -SD:/Projekty/diplomka/Microbenchmarks -Bd:/Projekty/diplomka/Microbenchmarks/build -G "MinGW Makefiles"

Rem Build
D:\CMake\bin\cmake.EXE --build d:/Projekty/diplomka/Microbenchmarks/build --config Debug --target two-d-array-iteration -j 10 --