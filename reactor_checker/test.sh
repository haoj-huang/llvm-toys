#/bin/bash
#modified files: 
#clang/include/clang/StaticAnalyzer/Checkers/Checkers.td
#clang/lib/StaticAnalyzer/Checkers/ReactorChecker.cpp

clang --analyze -Xanalyzer -analyzer-checker=alpha.powerplant ./test.c

