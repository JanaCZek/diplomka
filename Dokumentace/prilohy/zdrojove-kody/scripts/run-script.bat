Rem Run this script from the scripts folder
Rem Usage: run-script.bat gcc/clang O0/O2

..\build\%1%2\two-d-array-iteration\two-d-array-iteration.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%11.csv"
..\build\%1%2\matrix-multiplication-variations\matrix-multiplication-variations.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%12.csv"
..\build\%1%2\loop-carried-dependence\loop-carried-dependence.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%13.csv"
..\build\%1%2\loop-unrolling\loop-unrolling.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%14.csv"
..\build\%1%2\cache-associativity-limit\cache-associativity-limit.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%15.csv"
..\build\%1%2\array-index-patterns\array-index-patterns.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%16.csv"
..\build\%1%2\hot-cold\hot-cold.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%17.csv"
..\build\%1%2\aos-soa\aos-soa.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%18.csv"
..\build\%1%2\simd\simd.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%19.csv"
..\build\%1%2\structure-size\structure-size.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%110.csv"
..\build\%1%2\predictability\predictability.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%111.csv"
..\build\%1%2\false-sharing\false-sharing.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%112.csv"
..\build\%1%2\memory-aliasing\memory-aliasing.exe --benchmark_out_format=csv --benchmark_out="%1%2\bench%113.csv"