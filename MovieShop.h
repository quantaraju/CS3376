#ifndef __MOVIESHOP_H__
#define __MOVIESHOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Person {
  public:
    Person(string& lname, string& fname, int id);
    string& getLName();
    string& getFName();
    void setLName(string& lname);
    void setFName(string& fname);
    int getId();
  private:
    string lname; //Last Name
    string fname; //First Name
    int id;       //Unique ID for the person
  friend ostream& operator<<(ostream& os, const Person& p);
};

class Movie {
  public:
    Movie(string& title, Person& director, int yearMade, int id);
    string& getTitle();
    Person& getDirector();
    int getYearMade();
    int getId();
  private:
    string title; //Name of the Movie
    Person director; //ID of director referenced into Persons ID
    int yearMade; //Year when the movie was made
    int id;       //Unique ID for the movie
  friend ostream& operator<<(ostream& os, const Movie& m);
};

class ActIn {
  public:
    ActIn(Person& actorId, Movie& movieId);
    Person& getActorId();
    Movie& getMovieId();
  private:
    Person actorId; //Unique ID referenced into the Persons ID
    Movie movieId; //Unique ID referenced into the Movies ID
  friend ostream& operator<<(ostream& os, const ActIn& ai);
};

class Persons {
  public:
    Persons();
    ~Persons();
    void addPerson(Person& person);
    vector<Person>& getPersons();
    void print();
    static vector<Person>::iterator it;
  private:
    vector<Person> persons;
};

class Movies {
  public:
    Movies();
    ~Movies();
    void addMovie(Movie& movie);
    vector<Movie>& getMovies();
    void print();
    static vector<Movie>::iterator it;
  private:
    vector<Movie> movies;
};

class ActsIn {
  public:
    ActsIn();
    ~ActsIn();
    void addActIn(ActIn& actIn);
    vector<ActIn>& getActsIn();
    void print();
    void populateMovieActorMultiMap();
    void printMovieActorMultiMap(Movie searchMovie);
    
    multimap<Movie, Person> movieActorMultiMap;

    static vector<ActIn>::iterator it;
  public:
    vector<ActIn> actsIn;
};
#endif
