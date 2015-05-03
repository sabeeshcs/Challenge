# first name a variable objects for all object files

objects = ConnectionAcceptor.o ConnectionHandler.o main.o


# name a variable sources for all source files

sources = ConnectionAcceptor.cpp ConnectionHandler.cpp main.cpp

# now give target as lab1 with objects as variable dependencies + command line

lab1: $(objects)
	g++ -o stringServer $(objects) -lpthread

# list the dependencies for object files - those header files which help build objects

ConnectionAcceptor.o : ConnectionAcceptor.h
ConnectionHandler.o : ConnectionHandler.h
main.o : main.cpp 

# how to build all object files from all dependent source files


$(objects): $(sources)
	g++ -c $(sources)

clean:
	rm $(objects) stringServer

