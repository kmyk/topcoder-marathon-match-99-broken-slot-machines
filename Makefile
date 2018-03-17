.PHONY: build run

PROBLEM := BrokenSlotMachines
CXXFLAGS := -std=c++11 -Wall -O2 -DLOCAL

a.out: ${PROBLEM}.cpp
	g++ ${CXXFLAGS} $^
Tester: Tester.cpp
	g++ ${CXXFLAGS} -o $@ $^ -lpthread -lboost_system

build: a.out
run: a.out Tester
	./Tester -seed ${SEED}

tester/Tester.class: tester/Tester.java
	cd tester ; javac Tester.java
run/java: a.out tester/Tester.class
	cd tester ; java Tester -exec ../a.out -seed ${SEED} -debug
