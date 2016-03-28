#include <sqlite3.h> 
#include "MovieShop.h"

using namespace std;

ifstream ifs;
ofstream ofs;
Persons personsTable;
Movies moviesTable;
ActsIn actsInTable;
sqlite3 *db;
char *zErrMsg = 0;
int  rc;
const char* createActsInSQL;
const char* createMoviesSQL;
const char* createPersonsSQL;
const char* data = "Callback function called";

static int callback_select(void *data, int argc, char **argv, char **azColName){
   int i;
   ostringstream oss;
   for(i=0; i<argc; i++){
      oss << azColName[i] << " = " << argv[i] << endl;
   }
   oss << endl;
   ofs << oss.str(); 
   return 0;
}
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void parsePersonsFile() {

  string line;
  ifs.open("persons.txt");
  cout << endl;
  cout << "Processing persons.txt file" << endl;
  int recordCount = 1;

  while (getline(ifs, line)) {
    cout <<"\t Record " << recordCount << ": " << line << endl;
    int begin(0);
    int end;

    end = line.find_first_of("|", begin);
    string lastName = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string firstName = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string personId = line.substr(begin, (end == string::npos ? line.size() : end) - begin);

    Person tempPerson(lastName, firstName, atoi(personId.c_str()));
    personsTable.addPerson(tempPerson);

    //cout << tempPerson << endl;
    recordCount++;
  }
  cout << "Persons table size = " << personsTable.getPersons().size();
  ifs.close();
}

Person& getDirector(int director) {
  for (unsigned int i = 0; i < personsTable.getPersons().size(); i++) {
    if (personsTable.getPersons()[i].getId() == director) {
      return personsTable.getPersons()[i];
    }
  }
}

void parseMoviesFile() {

  string line;
  ifs.open("movies.txt");
  cout << endl;
  cout << "Processing movies.txt file" << endl;
  int recordCount = 1;

  while (getline(ifs, line)) {
    cout <<"\t Record " << recordCount << ": " << line << endl;
    int begin(0);
    int end;

    end = line.find_first_of("|", begin);
    string title = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string director = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string yearMade = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string movieId = line.substr(begin, (end == string::npos ? line.size() : end) - begin);

    Movie tempMovie(title, getDirector(atoi(director.c_str())), atoi(yearMade.c_str()), atoi(movieId.c_str()));
    moviesTable.addMovie(tempMovie);

    //cout << tempMovie << endl;
    recordCount++;
  }
  cout << "Movies table size = " << moviesTable.getMovies().size();
  ifs.close();
}


Person& getActor(int director) {
  for (unsigned int i = 0; i < personsTable.getPersons().size(); i++) {
    if (personsTable.getPersons()[i].getId() == director) {
      return personsTable.getPersons()[i];
    }
  }
}


Movie& getMovie(int director) {
  for (unsigned int i = 0; i < moviesTable.getMovies().size(); i++) {
    if (moviesTable.getMovies()[i].getId() == director) {
      return moviesTable.getMovies()[i];
    }
  }
}

void parseActsInFile() {

  string line;
  ifs.open("acts_in.txt");
  cout << endl;
  cout << "Processing acts_in.txt file" << endl;
  int recordCount = 1;

  while (getline(ifs, line)) {
    cout <<"\t Record " << recordCount << ": " << line << endl;
    int begin(0);
    int end;

    end = line.find_first_of("|", begin);
    string actorId = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    begin = end + 1;
    end = line.find_first_of("|", begin);
    string movieId = line.substr(begin, (end == string::npos ? line.size() : end) - begin);
    ActIn tempActIn(getActor(atoi(actorId.c_str())), getMovie(atoi(movieId.c_str())));
    actsInTable.addActIn(tempActIn);

    //cout << tempActIn << endl;
    recordCount++;
  }
  cout << "Acts in table size = " << actsInTable.getActsIn().size();
  ifs.close();
}

void partA1() {

  parsePersonsFile();
  parseMoviesFile();
  parseActsInFile();
}

void createDB() {
  rc = sqlite3_open("movie.db", &db);
   if( rc ){
      cerr << "Can't open database : " << sqlite3_errmsg(db) << "\n";
      exit(0);
   }else{
      cout << "Database: <movie.db> Opened successfully\n";
   }
}

void createTables() {
  createMoviesSQL = "CREATE TABLE MOVIES (" \
                        "TITLE VARCHAR(40) NOT NULL," \
                        "DIRECTOR INT NOT NULL," \
                        "YEAR_MADE INT NOT NULL," \
                        "ID INT," \
                        "PRIMARY KEY (ID)," \
                        "FOREIGN KEY (DIRECTOR) REFERENCES PERSONS(ID));";
  createPersonsSQL = "CREATE TABLE PERSONS (" \
                        "LNAME VARCHAR(40) NOT NULL," \
                        "FNAME VARCHAR(40)," \
                        "ID INT," \
                        "PRIMARY KEY (ID));";
  createActsInSQL = "CREATE TABLE ACTS_IN (ACTOR INT," \
                        "MOVIE INT," \
                        "PRIMARY KEY (ACTOR, MOVIE)," \
                        "FOREIGN KEY (ACTOR) REFERENCES PERSONS(ID)," \
                        "FOREIGN KEY (MOVIE) REFERENCES MOVIES(ID));";

  rc = sqlite3_exec(db, createPersonsSQL, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  } else {
    cout << "Persons table created successfully\n";
  }
  rc = sqlite3_exec(db, createMoviesSQL, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  } else {
    cout << "Movies table created successfully\n";
  }
  rc = sqlite3_exec(db, createActsInSQL, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  } else {
    cout << "ActsIn table created successfully\n";
  }
}

void insertTables() {
  string tempSql = "";
  vector<Person> tempPersons = personsTable.getPersons();
  vector<Movie> tempMovies = moviesTable.getMovies();
  vector<ActIn> tempActsIn = actsInTable.getActsIn();
  for (unsigned int i = 0; i < tempPersons.size(); i++) {
    ostringstream oss;
    oss << tempPersons[i].getId();
    tempSql += "INSERT INTO PERSONS VALUES('" + tempPersons[i].getLName() + "','" + tempPersons[i].getFName() + "'," + oss.str() + ");\n";
  }
  for (unsigned int i = 0; i < tempMovies.size(); i++) {
    ostringstream oss;
    oss << tempMovies[i].getDirector().getId() << "," << tempMovies[i].getYearMade() << "," << tempMovies[i].getId();
    tempSql += "INSERT INTO MOVIES VALUES('" + tempMovies[i].getTitle() + "'," + oss.str() + ");\n";
  }
  for (unsigned int i = 0; i < tempActsIn.size(); i++) {
    ostringstream oss;
    oss << tempActsIn[i].getActorId().getId() << "," << tempActsIn[i].getMovieId().getId();
    tempSql += "INSERT INTO ACTS_IN VALUES(" + oss.str() + ");\n";
  }

  cout << "SQL INSERT STATEMENTS ARE: " << endl;
  cout << tempSql;

  rc = sqlite3_exec(db, tempSql.c_str(), callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      cerr << "SQL error: " << zErrMsg << "\n";
      sqlite3_free(zErrMsg);
   }else{
      cout << "Records inserted successfully\n";
   }
}

void partA2() {

  createDB();
  createTables();
  insertTables();
}

void selectTables() {
  
  string selectSql = "SELECT * FROM PERSONS; SELECT * FROM MOVIES; SELECT * FROM ACTS_IN;";
  rc = sqlite3_exec(db, selectSql.c_str(), callback_select, (void*)data, &zErrMsg);
  if(rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  }else{
    cout << "Select query operation done successfully\n";
  }  
}

void partA3() {
  ofs << "INITIAL DATA......." << endl;
  selectTables();
}

void insertNewData() {
  string lname1 = "Kershner";
  string fname1 = "Irwen"; //Typo
  Person ikPerson = Person(lname1, fname1,19);
  personsTable.addPerson(ikPerson);
  string lname2 = "Hamill";
  string fname2 = "Mark";
  Person mhPerson = Person(lname2, fname2,20);
  personsTable.addPerson(mhPerson);
  string lname3 = "Harrison";
  string fname3 = "Ford";
  Person fhPerson = Person(lname3, fname3,21);
  personsTable.addPerson(fhPerson);
  string lname4 = "Fisher";
  string fname4 = "Carrie";
  Person cfPerson = Person(lname4, fname4,22);
  personsTable.addPerson(cfPerson);

  string title = "Star Wars: Episode IV - A New Hope";
  Movie swMovie = Movie(title, ikPerson, 1977, 4); 
  moviesTable.addMovie(swMovie);

  ActIn a1 = ActIn(ikPerson,swMovie);
  ActIn a2 = ActIn(mhPerson,swMovie);
  ActIn a3 = ActIn(fhPerson,swMovie);
  ActIn a4 = ActIn(cfPerson,swMovie);
  actsInTable.addActIn(a1);
  actsInTable.addActIn(a2);
  actsInTable.addActIn(a3);
  actsInTable.addActIn(a4);
}

void insertNewRecord() {
  const char* sql = "INSERT INTO PERSONS VALUES('Kershner','Irwen',19);" \
  		"INSERT INTO PERSONS VALUES('Hamill','Mark',20);" \
                "INSERT INTO PERSONS VALUES('Harrison','Ford',21);" \
                "INSERT INTO PERSONS VALUES('Fisher','Carrie',22);" \
                "INSERT INTO MOVIES VALUES('Star Wars: Episode IV - A New Hope', 19, 1977, 4);" \
                "INSERT INTO ACTS_IN VALUES(19,4);" \
                "INSERT INTO ACTS_IN VALUES(20,4);" \
                "INSERT INTO ACTS_IN VALUES(21,4);" \
                "INSERT INTO ACTS_IN VALUES(22,4);"; 
  insertNewData();

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  }else{
    cout << "New Records inserted successfully\n";
  }
  ofs << "DATA AFTER INSERTING NEW RECORDS........" << endl;
  selectTables();
}

void updateCorrectedNewData() {
  string correctedName = "Irwin";
  for (unsigned int i = 0; i < personsTable.getPersons().size(); i++) {
    if (personsTable.getPersons()[i].getId() == 19) {
      personsTable.getPersons()[i].setFName(correctedName);
      break;
    }
  }
}

void updateCorrectedNewRecord() {

  const char* sql = "UPDATE PERSONS set FNAME='Irwin' where id=19;";    
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    cerr << "SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
  }else{
    cout << "Corrected record updated successfully\n";
  }
  updateCorrectedNewData();
  ofs << "DATA AFTER UPDATING THE INCORRECT RECORD.........." << endl;
  selectTables();
}

void partA4() {
  insertNewRecord();
  updateCorrectedNewRecord();
}

void initialize() {
  if (remove("all.txt") != 0)
    cerr << "Error deleting file" << endl;
  else
    cout << "File all.txt is deleted successfully" << endl;
  ofs.open("all.txt", ofstream::app);
}

void printAll() {

  cout << "PRINTING ALL VECTORS" << endl;
  for (unsigned int i = 0; i < personsTable.getPersons().size(); i++) {
    cout << personsTable.getPersons()[i] << endl;
  }
  for (unsigned int i = 0; i < moviesTable.getMovies().size(); i++) {
    cout << moviesTable.getMovies()[i] << endl;
  }
  for (unsigned int i = 0; i < actsInTable.getActsIn().size(); i++) {
    cout << actsInTable.getActsIn()[i] << endl;
  }
}

void cleanup() {
  ofs.close();
}

void B2() {
  personsTable.print(); 
  moviesTable.print(); 
  actsInTable.print(); 
}

Movie findMovie(int movieId) {
  for (unsigned int i = 0; i < moviesTable.getMovies().size(); i++) {
    if (moviesTable.getMovies()[i].getId() == movieId) {
      return moviesTable.getMovies()[i];
    }
  }
}


void B3() {
  actsInTable.populateMovieActorMultiMap();
  Movie searchMovie = findMovie(4);
  actsInTable.printMovieActorMultiMap(searchMovie);
}

int main(int argc, char* argv[])
{
  cout << "Assignement 3 - Course CS3376 Spring" << endl;
  initialize();
  partA1();
  partA2();
  partA3();
  partA4();

  B2();
  B3();

  cleanup();
}
