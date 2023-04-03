Rem Run this script from the scripts folder
Rem Usage: run-msvc-script.bat O0/O2

..\out\build\x64-Debug-%1\two-d-array-iteration\two-d-array-iteration.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench1.csv"
..\out\build\x64-Debug-%1\matrix-multiplication-variations\matrix-multiplication-variations.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench2.csv"
..\out\build\x64-Debug-%1\loop-carried-dependence\loop-carried-dependence.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench3.csv"
..\out\build\x64-Debug-%1\loop-unrolling\loop-unrolling.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench4.csv"
..\out\build\x64-Debug-%1\cache-associativity-limit\cache-associativity-limit.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench5.csv"
..\out\build\x64-Debug-%1\array-index-patterns\array-index-patterns.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench6.csv"
..\out\build\x64-Debug-%1\hot-cold\hot-cold.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench7.csv"
..\out\build\x64-Debug-%1\aos-soa\aos-soa.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench8.csv"
..\out\build\x64-Debug-%1\simd\simd.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench9.csv"
..\out\build\x64-Debug-%1\structure-size\structure-size.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench10.csv"
..\out\build\x64-Debug-%1\predictability\predictability.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench11.csv"
..\out\build\x64-Debug-%1\false-sharing\false-sharing.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench12.csv"
..\out\build\x64-Debug-%1\memory-aliasing\memory-aliasing.exe --benchmark_out_format=csv --benchmark_out="msvc%1\bench13.csv"