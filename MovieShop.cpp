#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include "MovieShop.h"

Person::Person(string& lname, string& fname, int id) {
  this->lname = lname;
  this->fname = fname;
  this->id = id;
}
string& Person::getLName() {
  return lname;
}
string& Person::getFName() {
  return fname;
}
void Person::setLName(string& lname) {
  this->lname = lname;
}
void Person::setFName(string& fname) {
  this->fname = fname;
}
int Person::getId() {
  return id;
}

Movie::Movie(string& title, Person& director, int yearMade, int id):title(title), director(director), yearMade(yearMade), id(id) {
/*
  this->title = title;
  this->director = director;
  this->yearMade = yearMade;
  this->id = id;
*/
}
string& Movie::getTitle() {
  return title;
}
Person& Movie::getDirector() {
  return director;
}
int Movie::getYearMade() {
  return yearMade;
}
int Movie::getId() {
  return id;
}

ActIn::ActIn(Person& actorId, Movie& movieId):actorId(actorId), movieId(movieId) {
}
Person& ActIn::getActorId() {
  return actorId;
}
Movie& ActIn::getMovieId() {
  return movieId;
}

ostream& operator<<(ostream& os, const Person& p) {
  os << "\t\tLast name : " << p.lname << endl;
  os << "\t\tFirst name: " << p.fname << endl;
  os << "\t\tPerson ID : " << p.id << endl;

  return os;
}

ostream& operator<<(ostream& os, const Movie& m) {
  os << "\t\tMovie Title  : " << m.title << endl;
  Person p = m.director;
  os << "\t\tDirector Name: " << p.getId() << endl;
  os << "\t\tYear Made    : " << m.yearMade << endl;
  os << "\t\tMovie ID     : " << m.id << endl;

  return os;
}

ostream& operator<<(ostream& os, const ActIn& ai) {
  Person p = ai.actorId;
  os << "\t\tActor ID: " << p.getId() << endl;
  Movie m = ai.movieId;
  os << "\t\tMovie ID: " << m.getId() << endl;

  return os;
}


Persons::Persons() {}
Persons::~Persons() {}
void Persons::addPerson(Person& person) {
  persons.push_back(person);
}
vector<Person>& Persons::getPersons() {
  return persons;
}
vector<Person>::iterator Persons::it = vector<Person>::iterator();
void Persons::print() {
  cout << "Persons vector:" << endl;
  unsigned int i = 1;
  for (Persons::it = persons.begin(); Persons::it != persons.end(); ++i,++Persons::it)
    cout << "Record: " << i << "\n" << *it << endl;
}

Movies::Movies() {}
Movies::~Movies() {}
void Movies::addMovie(Movie& movie) {
  movies.push_back(movie);
}
vector<Movie>& Movies::getMovies() {
  return movies;
}
vector<Movie>::iterator Movies::it = vector<Movie>::iterator();
void Movies::print() {
  cout << "Movies vector:" << endl;
  unsigned int i = 1;
  for (Movies::it = movies.begin(); Movies::it != movies.end(); i++,++Movies::it)
    cout << "Record: " << i << "\n" << *it << endl;
}

ActsIn::ActsIn() {}
ActsIn::~ActsIn() {}
void ActsIn::addActIn(ActIn& actIn) {
  actsIn.push_back(actIn);
}
vector<ActIn>& ActsIn::getActsIn() {
  return actsIn;
}
vector<ActIn>::iterator ActsIn::it = vector<ActIn>::iterator();
void ActsIn::print() {
  cout <<"ActsIn vector:" << endl;
  unsigned int i = 1;
  for (ActsIn::it = actsIn.begin(); ActsIn::it != actsIn.end(); i++,++ActsIn::it)
    cout << "Record: " << i << "\n" << *it << endl;
}

bool operator<(Movie a, Movie b)
{
   return strcmp(a.getTitle().c_str(), b.getTitle().c_str()) < 0;
}


void ActsIn::populateMovieActorMultiMap() {
  cout << "Populating Multi-Map...." << endl;
  string s = "UNAVAIL";
  Person tempPerson(s, s, 0);
  Movie tempMovie(s, tempPerson, 0, 0);
  for (ActsIn::it = actsIn.begin(); ActsIn::it != actsIn.end(); ++ActsIn::it) {
    tempPerson = it->getActorId();
    cout << tempPerson << endl;
    tempMovie = it->getMovieId();
    cout << tempMovie << endl;
    movieActorMultiMap.insert(pair<Movie, Person>(tempMovie, tempPerson));
  }
}

void ActsIn::printMovieActorMultiMap(Movie searchMovie) {
  cout << "Printing Multi-Map..." << endl;
  pair<multimap<Movie, Person>::iterator, multimap<Movie, Person>::iterator> p;
  p = movieActorMultiMap.equal_range(searchMovie);
  cout << endl << "Key					Value" << endl;
  for (multimap<Movie, Person>::iterator it = p.first; it != p.second; ++it) {
    Movie tempMovie = it->first;
    Person tempPerson = it->second;
    //string tempName = (*it).second.getFName();
    cout << tempMovie.getTitle() << "\t" << tempPerson.getLName() << " " << tempPerson.getFName() << endl;
  }
}

