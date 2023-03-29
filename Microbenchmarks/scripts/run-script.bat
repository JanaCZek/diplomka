Rem Run this script from the scripts folder
Rem Usage: run-script.bat gcc/clang O0/O2

..\build\%1%2\two-d-array-iteration\two-d-array-iteration.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%11.txt"
..\build\%1%2\matrix-multiplication-variations\matrix-multiplication-variations.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%12.txt"
..\build\%1%2\loop-carried-dependence\loop-carried-dependence.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%13.txt"
..\build\%1%2\loop-unrolling\loop-unrolling.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%14.txt"
..\build\%1%2\cache-associativity-limit\cache-associativity-limit.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%15.txt"
..\build\%1%2\array-index-patterns\array-index-patterns.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%16.txt"
..\build\%1%2\hot-cold\hot-cold.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%17.txt"
..\build\%1%2\aos-soa\aos-soa.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%18.txt"
..\build\%1%2\simd\simd.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%19.txt"
..\build\%1%2\structure-size\structure-size.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%110.txt"
..\build\%1%2\predictability\predictability.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%111.txt"
..\build\%1%2\false-sharing\false-sharing.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%112.txt"
..\build\%1%2\memory-aliasing\memory-aliasing.exe --benchmark_out_format=console --benchmark_out="%1%2\bench%113.txt"