# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use 
# "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections



# include what ever source code *.h files pathfinder relies on 
# (these are merely the ones that were used in the solution)

pathfinder: ActorGraph.o Movie.o ActorEdge.o ActorNode.o

actorconnections: ActorGraph.o Movie.o ActorEdge.o ActorNode.o UnionFind.hpp


# include what ever source code *.h files ActorGraph relies on i
# (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h 
# file that is already included with class/method headers

UnionFind.hpp: Movie.o ActorNode.o

ActorGraph.o: Movie.o ActorNode.o ActorEdge.o ActorGraph.h

Movie.o: ActorNode.o ActorEdge.o Movie.h

ActorEdge.o: ActorEdge.h

ActorNode.o: ActorEdge.o ActorNode.h


clean:
	rm -f pathfinder actorconnections *.o core*

