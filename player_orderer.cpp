#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>


using namespace std;


pair<int,double> d20(){
  static bool is_inited=false;
  if(!is_inited){
    srand(time(nullptr));
    is_inited=true;
  }
  int val20 = (rand() % 20) + 1;
  double dval = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
  return make_pair(val20,dval);
}


template <typename T>
struct own_greater{
  bool operator()(const T& a, const T& b) const {
    if(a.first > b.first){
      return true;
    }else if(a.first == b.first){
	return a.second > b.second;
    }else{ 
      return false;
    }
  }
};


void print_ordered(const map<string,int>& cmap){
  multimap<pair<int,double>,string,own_greater<pair<int,double>>> outmap;

  for(auto& elem : cmap){
    pair<int,double> init = d20();
    init.first += elem.second;
    //outmap[init] = elem.first;
    outmap.insert({init, elem.first});
  }
  cout<<"New player order: "<<endl;
  for(auto& elem : outmap){
    cout<<elem.second<<"  "<<elem.first.first<<endl;
  }
  cout<<endl;
}


void print(const map<string,int>& cmap){
  cout<<"Characters currently added:"<<endl;
  for(auto& elem : cmap){
    cout<<elem.first<<"  "<<elem.second<<endl;
  }
  cout<<endl;
}


void add_chars(map<string,int>& cmap){
  while(true){
    string s;
    int d;
    cout<<"Add characters by entering their name. Terminate by entering \"x\"."<<endl;
    cin >> s;
    if(s != "x"){
      cout << "Initiative for " << s << ":"<<endl;
      cin >> d;
      cout<<endl<<endl;
      cmap[s] = d;
    }else{
      cout<<endl;
      break;
    }
  }
  print(cmap);
}

void rm_chars(map<string,int>& cmap){
  while(true){
    string s;
    cout<<"Remove characters by entering their name. Terminate by entering \"x\"."<<endl;
    cin >> s;
    if(s != "x"){
      cmap.erase(s);
      cout << "Removed " << s << endl<<endl;
    }else{
      cout<<endl;
      break;
    }
  }
  print(cmap);
}


int main(int argc, char** argv){
  map<string,int> character_map;

  if(argc > 1){
    std::string fn = argv[1];
    cout<<"Reading character stats from file " << fn << endl << endl;

    ifstream ifs(fn);
    if(ifs.good()){
      string line;
      while(getline(ifs,line)){
	istringstream iss(line);
	string name;
	int init;

	iss >> name;
	iss >> init;
	character_map[name] = init;
      }
      print(character_map);
    }else{
      cout << "Could not read Characters from file " << fn << endl
	   << "Expected format: Name Initiative(int) \\n" << endl
	   << "\"x\" is not allowed as a name." << endl;
    }
  }


  cout<<"Generating new battle orders. Enter \"a\" to add characters, \"r\" to remove and any other string for new battle order."<<endl;
  while(true){
    string s;
    cin >> s;
    cout<<endl;
    if(s == "a") add_chars(character_map);
    else if (s == "r") rm_chars(character_map);
    else print_ordered(character_map);
  }

  return 0;
}
