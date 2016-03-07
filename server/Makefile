server: game.SessionMsg.pb.o messagefactory.o asynlog.o eventloop.o main.o
	clang++ -o server game.SessionMsg.pb.o messagefactory.o asynlog.o eventloop.o main.o -lpthread -lprotobuf
game.SessionMsg.pb.o:
	clang++ -c ./protobuf/game.SessionMsg.pb.cc -std=c++11
messagefactory.o:
	clang++ -c messagefactory.cpp -std=c++11
asynlog.o:
	clang++ -c asynlog.cpp -std=c++11
eventloop.o:
	clang++ -c eventloop.cpp -std=c++11
main.o:
	clang++ -c main.cpp -std=c++11
clean:
	rm *.o
