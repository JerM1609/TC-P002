run:
	g++ main.cpp clases.cpp clases.h -std=c++17 -o main.exe
	./main.exe
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