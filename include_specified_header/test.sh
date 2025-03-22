#/bin/bash
#modified files: 
#clang/include/clang/Driver/Options.td
#clang/lib/Driver/Clang.cpp: Clang::ConstructJob
#clang/lib/Driver/Driver.cpp: Driver::getToolChain
#clang/lib/Driver/ToolChains/Zipline.h(Zipline.cpp)

read -p "Enter a Test No[valid no: 0,1,2,3]: " num

if [ $num -eq 0 ]; then
	mv ./simple_log_bak.h ./simple_log.h
	clang++ -fuse-simple-log ./test.cpp -o ./simple_log
	mv ./simple_log.h ./simple_log_bak.h
elif [ $num -eq 1 ]; then
	clang++ -zipline -fuse-simple-log ./test.cpp -o ./simple_log
elif [ $num -eq 2 ]; then
	mv ./simple_log_bak.h ./simple_log.h
	clang++ -fuse-info-simple-log -fsimple-log-path=./simple_log.h ./test.cpp -o ./simple_log
	mv ./simple_log.h ./simple_log_bak.h
elif [ $num -eq 3 ]; then
	mv ./simple_log_bak.h ./simple_log.h
	clang++ -fuse-simple-log -fno-use-info-simple-log ./test.cpp -o ./simple_log
	mv ./simple_log.h ./simple_log_bak.h
else
	echo "unrecognized Test No!"
fi


if [ -e ./simple_log ]; then
	./simple_log
	rm -f ./simple_log
fi

