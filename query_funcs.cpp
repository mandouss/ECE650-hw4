#include "query_funcs.h"

//import data from txts
void import_txt(string filename, connection *C){
  const char *fname = filename.c_str();
  string line;
  ifstream infile(fname);
  stringstream ss_buffer;
  string row_info;
  if(infile.is_open()){
    while(getline(infile, line)) {
      //split each line
      istringstream iss(line);
      vector<string> str{istream_iterator<string>{iss}, istream_iterator<string>{}};
      //change ' to ''
      for(vector<string>::iterator it = str.begin(); it != str.end(); ++it) {
	int loc;
	loc = (*it).find("'");
	if(loc < (*it).length()){
	  (*it).insert(loc, "'");
	}
      }
      ss_buffer.str(std::string());  //empty buffer
      if(filename == "player.txt") {
	ss_buffer << "INSERT INTO PLAYER(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES (DEFAULT, " << str[1] << "," << str[2] << ",'" << str[3] << "','" << str[4] << "'," << str[5] << "," << str[6] << "," << str[7] << "," << str[8] << "," << str[9] << "," << str[10] << ");";
      }
      else if(filename == "team.txt") {
	ss_buffer << "INSERT INTO TEAM(TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (DEFAULT, " << "'" << str[1] << "'," << str[2] << "," <<str[3] << "," << str[4] << "," <<str[5] << ");";
      }
      else if(filename == "state.txt") {
	ss_buffer << "INSERT INTO STATE(STATE_ID, NAME) VALUES (DEFAULT, " << "'" << str[1] << "'" << ");";
      }
      else if(filename == "color.txt") {
	ss_buffer << "INSERT INTO COLOR(COLOR_ID, NAME) VALUES (DEFAULT, " << "'" << str[1] << "'" << ");";
      }
      row_info = ss_buffer.str();
      work W(*C);
      W.exec(row_info);
      W.commit();
    }
    //cout << "Records created succesfully" << endl;
  }
  else {
    cerr << "cannot open the file!" << endl;
    exit(EXIT_FAILURE);
  }
  infile.close();
}

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "INSERT INTO PLAYER(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES (DEFAULT, " << team_id << "," << jersey_num << ",'" << first_name  << "','" << last_name << "'," << mpg << "," << ppg << "," << rpg << "," << apg << "," << spg << "," << bpg << ");";
  row_info = ss_buffer.str();
  try {
    work W(*C);
    W.exec(row_info);
    W.commit();
  }
  catch(const std::exception &e) {
    cerr << e.what() << std::endl;
  }
}

void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "INSERT INTO TEAM(TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (DEFAULT, " << "'" << name << "'," << state_id << "," << color_id << "," << wins << "," << losses <<");";
  row_info = ss_buffer.str();
  try {
    work W(*C);
    W.exec(row_info);
    W.commit();
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}


void add_state(connection *C, string name) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "INSERT INTO STATE(STATE_ID, NAME) VALUES (DEFAULT, " << "'" << name << "'" << ");";
  row_info = ss_buffer.str();
  try {
    work W(*C);
    W.exec(row_info);
    W.commit();
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}


void add_color(connection *C, string name) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "INSERT INTO COLOR(COLOR_ID, NAME) VALUES (DEFAULT, " << "'" << name << "'" << ");";
  row_info = ss_buffer.str();
  try{
    work W(*C);
    W.exec(row_info);
    W.commit();
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}

//query1 helper
template <class T>
void insert2vector(vector<T> & v, T a, T b, T c, T d, T e, T f){
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  v.push_back(e);
  v.push_back(f);
}
void query1(connection *C, //for all use_attribute 1:enable 
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ) {
  string mpg = "MPG", ppg = "PPG",  rpg = "RPG", apg = "APG", spg = "SPG", bpg = "BPG";
  stringstream ss_buffer;
  string player_select;
  vector<double> use_arr, min_arr, max_arr;
  vector<string> attr_arr, cond_arr;
  insert2vector(use_arr,  (double)use_mpg, (double)use_ppg, (double)use_rpg, (double)use_apg, (double)use_spg, (double)use_bpg);
  insert2vector(min_arr,  (double)min_mpg, (double)min_ppg, (double)min_rpg, (double)min_apg, min_spg, min_bpg);
  insert2vector(max_arr,  (double)max_mpg, (double)max_ppg, (double)max_rpg, (double)max_apg, max_spg, max_bpg);
  insert2vector(attr_arr, mpg, ppg, rpg, apg ,spg, bpg);
  if(std::find(use_arr.begin(), use_arr.end(), 1) == use_arr.end()){
    ss_buffer << "SELECT * FROM PLAYER";
  }
  else{
    ss_buffer << "SELECT * FROM PLAYER WHERE ";
    for(int i = 0; i < 6; i++) {
      if(use_arr[i] == 1){
	stringstream ss_cond;
	string cond;
	ss_cond << "( " << attr_arr[i] << " >= " << min_arr[i] << " AND " << attr_arr[i] << " <= " << max_arr[i] << " )";
	cond = ss_cond.str();
	cond_arr.push_back(cond);
      }
    }
    if(cond_arr.size() == 1) {
      ss_buffer << cond_arr[0] << ";";
    }
    else {
      for(vector<string>::iterator it = cond_arr.begin(); it != cond_arr.end() - 1; ++it) {
	ss_buffer << *it << " AND ";
      }
      ss_buffer << cond_arr[cond_arr.size()-1] << ";";
    }
  }
  player_select = ss_buffer.str();
  try{
    nontransaction N(*C);
    result R(N.exec(player_select));
    std::cout << "PLAYER_ID  TEAM_ID  UNIFORM_NUM  FIRST_NAME  LAST_NAME  MPG  PPG  RPG  APG  SPG  BPG " <<std::endl;
    for(result::const_iterator it = R.begin(); it != R.end(); ++it){
      cout << it[0].as<int>() << "  " << it[1].as<int>() << "  " << it[2].as<int>() << "  " << it[3].as<string>() << "  " << it[4].as<string>() << "  "
	   << it[5].as<int>() << "  " << it[6].as<int>() << "  " << it[7].as<int>() << "  " << it[8].as<int>() << "  " << it[9].as<double>() << "  "
	   << it[10].as<double>() << std::endl;
    }
    //cout << "Operation done successfully" << std::endl;
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}


void query2(connection *C, string team_color) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "SELECT TEAM.NAME FROM TEAM, COLOR WHERE (COLOR.NAME = '" << team_color << "') AND (TEAM.COLOR_ID = COLOR.COLOR_ID);";
  row_info = ss_buffer.str();
  try {
    nontransaction N(*C);
    result R(N.exec(row_info));
    std::cout <<  "NAME " << std::endl;
    for(result::const_iterator it = R.begin(); it != R.end(); ++it){
      cout << it[0].as<string>() << std::endl;
    }
    //cout << "Operation done successfully" << std::endl;
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }  
}

//show the first and last name of each player that plays for the indicated team, ordered from highest to lowest ppg (points per game)
void query3(connection *C, string team_name) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM WHERE (TEAM.NAME = '" << team_name << "') AND (TEAM.TEAM_ID = PLAYER.TEAM_ID) ORDER BY PPG DESC;";
  row_info = ss_buffer.str();
  try {
    nontransaction N(*C);
    result R(N.exec(row_info));
    std::cout <<  "FIRST_NAME  LAST_NAME" << std::endl;
    for(result::const_iterator it = R.begin(); it != R.end(); ++it){
      cout << it[0].as<string>() << "  " << it[1].as<string>() << std::endl;
    }
    //cout << "Operation done successfully" << std::endl;
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}

//show first name, last name,and jersey number of each player that plays in the indicated state and wears the indicated uniform color
void query4(connection *C, string team_state, string team_color) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM "
	    << "FROM PLAYER, STATE, COLOR, TEAM "
	    << "WHERE ( STATE.NAME = '" << team_state << "' ) AND ( COLOR.NAME = '" << team_color
	    << "' ) AND ( STATE.STATE_ID = TEAM.STATE_ID ) AND ( COLOR.COLOR_ID = TEAM.COLOR_ID ) AND ( TEAM.TEAM_ID = PLAYER.TEAM_ID );";
  row_info = ss_buffer.str();
  try {
    nontransaction N(*C);
    result R(N.exec(row_info));
    std::cout <<  "FIRST_NAME  LAST_NAME  UNIFORM_NUM" << std::endl;
    for(result::const_iterator it = R.begin(); it != R.end(); ++it){
      cout << it[0].as<string>() << "  " << it[1].as<string>() << "  " << it[2].as<int>() << std::endl;
    }
    //cout << "Operation done successfully" << std::endl;
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}

//show first name and last name of each player, and team name and number of wins for each team that has won more than the indicated number of games
void query5(connection *C, int num_wins) {
  stringstream ss_buffer;
  string row_info;
  ss_buffer << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
	    << "FROM PLAYER, TEAM "
	    << "WHERE ( TEAM.WINS > " << num_wins << " ) AND ( TEAM.TEAM_ID = PLAYER.TEAM_ID );";
  row_info = ss_buffer.str();
  try {
    nontransaction N(*C);
    result R(N.exec(row_info));
    std::cout <<  "FIRST_NAME  LAST_NAME  TEAM_NAME  WINS" << std::endl;
    for(result::const_iterator it = R.begin(); it != R.end(); ++it){
      cout << it[0].as<string>() << "  " << it[1].as<string>() << "  " << it[2].as<string>() << "  " << it[3].as<int>() << std::endl;
    }
    //cout << "Operation done successfully" << std::endl;
  }
  catch(const std::exception &e) {
    cerr << e.what() <<std::endl;
  }
}
