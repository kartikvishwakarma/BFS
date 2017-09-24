all:	kartik

kartik:  	kartik.o
			g++ kartik.o -o kartik

kartik.o:	kartik_vishwakarma.cpp
			g++ -c kartikvishwakarma.cpp -o kartik.o

clean: 
		rm -f *.o 
