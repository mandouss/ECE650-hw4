#include <iostream>
#include <pqxx/pqxx>
#include <cstring>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;
  string drop_table;
  string create_player, create_team, create_state, create_color;
  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
    work W(*C);
    drop_table = "DROP TABLE IF EXISTS PLAYER, TEAM, STATE, COLOR;"; 
    W.exec(drop_table);
    W.commit();
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  work W(*C);
  
  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  create_player= "CREATE TABLE PLAYER("
    "PLAYER_ID    SERIAL   PRIMARY KEY   NOT NULL,"
    "TEAM_ID      INT                    NOT NULL,"
    "UNIFORM_NUM  INT                    NOT NULL,"
    "FIRST_NAME   TEXT                   NOT NULL,"
    "LAST_NAME    TEXT                   NOT NULL,"
    "MPG          INT                    DEFAULT 0 NOT NULL,"
    "PPG          INT                    DEFAULT 0 NOT NULL,"
    "RPG          INT                    DEFAULT 0 NOT NULL,"
    "APG          INT                    DEFAULT 0 NOT NULL,"
    "SPG          REAL                   DEFAULT 0 NOT NULL,"
    "BPG          REAL                   DEFAULT 0 NOT NULL);";

  create_team = "CREATE TABLE TEAM("
    "TEAM_ID      SERIAL   PRIMARY KEY   NOT NULL,"
    "NAME         TEXT                   NOT NULL,"
    "STATE_ID     INT                    NOT NULL,"
    "COLOR_ID     INT                    NOT NULL,"
    "WINS         INT                    DEFAULT 0 NOT NULL,"
    "LOSSES       INT                    DEFAULT 0 NOT NULL);";

  create_state = "CREATE TABLE STATE("
    "STATE_ID     SERIAL   PRIMARY KEY   NOT NULL,"
    "NAME         TEXT                   NOT NULL);";

  create_color = "CREATE TABLE COLOR("
    "COLOR_ID     SERIAL   PRIMARY KEY   NOT NULL,"
    "NAME         TEXT                   NOT NULL);";
    
  W.exec(create_player);
  W.exec(create_team);
  W.exec(create_state);
  W.exec(create_color);
  W.commit();
  //cout << "Table created successfully" << endl;

  string filename[4] = {"player.txt", "team.txt", "state.txt", "color.txt"};
  for(int i = 0; i < 4; i++){
    import_txt(filename[i], C);
  }  
  exercise(C);
  
  //Close database connection
  C->disconnect();
  return 0;
}


