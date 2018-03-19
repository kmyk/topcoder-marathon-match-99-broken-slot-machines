.PHONY: build run

PROBLEM := BrokenSlotMachines
CXXFLAGS := -std=c++11 -Wall -O2 -DLOCAL

build: a.out Tester
run: a.out Tester
	./Tester -seed ${SEED}

a.out: ${PROBLEM}.cpp
	g++ ${CXXFLAGS} $^
Tester: Tester.cpp
	g++ ${CXXFLAGS} -o $@ $^ -lpthread -lboost_system -ljsoncpp

tester/Tester.class: tester/Tester.java
	cd tester ; javac Tester.java
run/java: a.out tester/Tester.class
	cd tester ; java Tester -exec ../a.out -seed ${SEED} -debug

URL := https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17092&pm=14853
submit:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open
submit/full:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open --full
