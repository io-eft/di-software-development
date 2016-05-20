all: first second clearo

third: third.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o GraphQueries.o mythreads.o myqueue.o myjob.o
	g++ -g3 -w third.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o GraphQueries.o mythreads.o myqueue.o myjob.o -o third -lpthread

second: SD-2014-Part-2.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o GraphQueries.o
	g++ -g3 -w SD-2014-Part-2.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o GraphQueries.o -o second

first: SD-2014-Part-1.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o
	g++ -g3 -w SD-2014-Part-1.o graph.o myfunctions.o tree.o list.o node.o properties.o edge.o edgelist.o GraphLib.o entities.o -o first

third.o: third.cpp
	g++ -g3 -w -c third.cpp
	
SD-2014-Part-1.o: SD-2014-Part-1.cpp
	g++ -g3 -w -c SD-2014-Part-1.cpp

SD-2014-Part-2.o: SD-2014-Part-2.cpp
	g++ -g3 -w -c SD-2014-Part-2.cpp
	
mythreads.o: mythreads.cpp
	g++ -g3 -w -c mythreads.cpp
    
myqueue.o: myqueue.cpp
	g++ -g3 -w -c myqueue.cpp
    
myjob.o: myjob.cpp
	g++ -g3 -w -c myjob.cpp
	
GraphQueries.o: GraphQueries.cpp
	g++ -g3 -w -c GraphQueries.cpp
	
GraphLib.o: GraphLib.cpp
	g++ -g3 -w -c GraphLib.cpp

graph.o: graph.cpp
	g++ -c -w -g3 graph.cpp

myfunctions.o: myfunctions.cpp
	g++ -c -w -g3 myfunctions.cpp

tree.o: tree.cpp
	g++ -c -w -g3 tree.cpp

list.o: list.cpp
	g++ -c -w -g3 list.cpp

entities.o: entities.cpp
	g++ -c -w -g3 entities.cpp

node.o: node.cpp
	g++ -c -w -g3 node.cpp

properties.o: properties.cpp
	g++ -c -w -g3 properties.cpp

edge.o: edge.cpp
	g++ -c -w -g3 edge.cpp

edgelist.o: edgelist.cpp
	g++ -c -w -g3 edgelist.cpp

clearo:
	rm -rf *.o
clean:
	rm -rf *.o test server
