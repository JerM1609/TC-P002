run:
	g++ main.cpp clases.h clases.cpp -std=c++17 -o main.out
	./main.out
test:
	gcc test.c -o test.exe
	./test.exe
check:
	g++ main.cpp -std=c++17 -o main.out
	./main.out
	gcc test.c -o test.exe
	./test.exe
clean:
	rm -rf *.out *.exe