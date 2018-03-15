.PHONY: build run

a.out: answer.cpp tester.cpp
	g++ -std=c++11 -Wall -O2 -DLOCAL $^

build: a.out
run: a.out
	./a.out -seed ${SEED}

score: a.out
	echo 'seed,result'
	for seed in $$(seq 300) ; do echo $$seed,$$(./a.out -seed $$seed | tee /dev/stderr | grep Result: | cut -d ' ' -f 2) ; done
