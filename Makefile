# target: dependency1 dependency 2
# <tab>	command
#

CC = g++
CPPFLAGS = -o
RM = rm -f
MV = mv

testMovieShop: testMovieShop.o MovieShop.o
	$(CC) -o testMovieShop testMovieShop.o MovieShop.o -l sqlite3

MovieShop.o: MovieShop.cpp MovieShop.h
	$(CC) -c MovieShop.cpp

testMovieShop.o: testMovieShop.cpp
	$(CC) -c testMovieShop.cpp

clean:
	$(RM) MovieShop.o testMovieShop testMovieShop.o movie.db all.txt
