server: asynlog.o eventloop.o main.o
	clang++ -o server asynlog.o eventloop.o main.o -lpthread 
asynlog.o:
	clang++ -c asynlog.cpp -std=c++11
eventloop.o:
	clang++ -c eventloop.cpp -std=c++11
main.o:
	clang++ -c main.cpp -std=c++11
clean:
	rm *.o
