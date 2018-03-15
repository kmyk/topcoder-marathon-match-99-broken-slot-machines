.PHONY: build run

a.out: answer.cpp tester.cpp
	g++ -std=c++11 -Wall -O2 -DLOCAL $^

build: a.out
run: a.out
	./a.out -seed ${SEED}
