.PHONY: build run

PROBLEM := BrokenSlotMachines

a.out: ${PROBLEM}.cpp
	g++ -std=c++11 -Wall -O2 -DLOCAL $^
tester/Tester.class: tester/Tester.java
	cd tester ; javac Tester.java

build: a.out
run: a.out tester/Tester.class
	cd tester ; java Tester -exec ../a.out -seed ${SEED} -debug
