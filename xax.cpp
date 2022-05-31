#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


using namespace std;

class Config{
   unordered_map<string,string> colors = {{"error",BOLDRED},
      {"reset",RESET}, {"name_installed",GREEN}, {"name",BLUE}, {"prompt",YELLOW}
   };
   public:
      Config(){
      }
      Config(string path, bool custom){
	 string key;
	 string value;
	 ifstream conf_f(path);
	 if (conf_f.is_open()){
	    while (!conf_f.eof() && !conf_f.bad()){
	       getline(conf_f, key, '=');
	       getline(conf_f, value, '\n');
	       if (key == "error_color")
		  colors["error"] = this->translate_color(value);
	       else if (key == "installed_color")
		  colors["name_installed_color"] = this->translate_color(value);
	       else if (key == "name_color")
		  colors["name"] = this->translate_color(value);
	       else if (key == "prompt_color")
		  colors["prompt"] = this->translate_color(value);
	    }
	    conf_f.close();
	 }
	 else if (custom)
	    cout << colors["error"] << "Could not read config file: " << path 
	       << " using defaults" << colors["reset"] << endl;

      }
      string color(string key){ return colors[key]; }
      int screen_w(){
	    struct winsize sz;
	    ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);
	    return sz.ws_col;
      }
      string translate_color (string color){
	 string translated_color;
	 for (int i = 0; i < color.size(); i++)
	    color[i] = toupper(color[i]);
	    if (color == "BLACK")
	       translated_color = string(BLACK);
	    else if (color == "RED")
	        translated_color = RED;
	    else if (color == "GREEN")
	        translated_color = string(GREEN);
	    else if (color == "YELLOW")
	        translated_color = string(YELLOW);
	    else if (color == "BLUE")
	        translated_color = string(BLUE);
	    else if (color == "MAGENTA")
	        translated_color = string(MAGENTA);
	    else if (color == "CYAN")
	        translated_color = string(CYAN);
	    else if (color == "WHITE")
	        translated_color = string(WHITE);
	    else if (color == "BOLDBLACK")
	        translated_color = string(BOLDBLACK);
	    else if (color == "BOLDRED")
	        translated_color = string(BOLDRED);
	    else if (color == "BOLDGREEN")
	        translated_color = string(BOLDGREEN);
	    else if (color == "BOLDYELLOW")
	        translated_color = string(BOLDYELLOW);
	    else if (color == "BOLDBLUE")
	        translated_color = string(BOLDBLUE);
	    else if (color == "BOLDMAGENTA")
	        translated_color = string(BOLDMAGENTA);
	    else if (color == "BOLDCYAN")
	        translated_color = string(BOLDCYAN);
	    else if (color == "BOLDWHITE")
	       translated_color = string(BOLDWHITE);
	    else
	       translated_color = string("");
	 return translated_color;
      }



};
class Package{
   string name;
   string description;
   string version; //stored as string since version numbers include _ char
   bool installed;
   int first_instance;
   public:
      Package(){
	 name = "";
	 description = "";
	 version = "";
	 installed = false;
      }
      Package(string line){
	 stringstream line_s;
	 line_s << line;
	 this->installed = false;
	 if (line[1] == '*')
	    this->installed = true;
	 string read;
	 getline(line_s, read, ' ');
	 getline(line_s, read, ' '); // name-version stored
	 first_instance = read.find_last_of('-');
	 //name
	 this->name = read.substr(0,first_instance);
	 this->version = read.substr(first_instance+1);

	 

	 getline(line_s, read, '\n');

	 description = "";
	 first_instance = 0;
	 for (int i =0; i < int(read.size()); i++){
	    if (read[i] != ' ' || first_instance != 0){
	       first_instance = i;
	       description += read[i];
	    }
	 }
	 while (description.find('\n') < description.size() )
	    description[description.find('\n')] = ' ';
      }
      void print(Config conf, int index){
	 cout << left;
	 cout << conf.color("prompt") <<  index+1 << ") " ;
	 if (installed)
	    cout << conf.color("name_installed") <<  name <<": " << version <<" [installed]";
	 else
	    cout << conf.color("name")  <<  name <<": " << version;
	 cout  << endl << conf.color("reset") ;
	 if (description.size() > (conf.screen_w()))
	    cout << description.substr(0,(conf.screen_w())) << endl;
	 else
	    cout << description << endl;
      }
      void printe(Config conf, int nw, int index){
	 cout << name << endl <<"\t"<< description << endl;
      }
      void printe(){
	 cout << name << endl <<"\t"<< description << endl;
      }
      string get_desc(){ return description; }
      string get_name(){ return name; }
      bool is_installed(){return installed;}
      

};

string GetStdoutFromCommand(string cmd);
void get_packages(vector<Package> &p_vect);
void search_select(string query,vector<Package> p_vect, stringstream &flags, Config conf);
int* str_to_int_arr(string str);

int main(int argc, char* argv[]){
   bool interactive = true;
   bool custom_conf = false;
   string conf_path = string(getenv("HOME")) + "/.config/xax.conf";
   stringstream flags = stringstream("") ;
   stringstream command;
   command = stringstream("/usr/bin/sudo /usr/bin/xbps-install");
   vector<Package> p_vect;
   get_packages(p_vect);
   if(argc < 2){
      flags << "-Su";
   }
   else{
	 for ( int i =1; i < argc; i++){
	    if (argv[i][0] == '-'){
	       if(string(argv[i]).find('R') < string(argv[i]).size()){
		 command = stringstream("/usr/bin/sudo /usr/bin/xbps-remove");
		 interactive = false;
	       }
	       else if(string(argv[i]).find('Q') < string(argv[i]).size()){
		 command = stringstream("/usr/bin/sudo /usr/bin/xbps-query");
		 interactive = false;
	       }
	       else if(string(argv[i]).find('I') < string(argv[i]).size()){
		 interactive = false;
	       }
	       else if(string(argv[i]).find("--conf") < string(argv[i]).size()){
		  i++;
		  conf_path = argv[i];
		  custom_conf = true;
	       }
	       else
		  flags << argv[i] << " ";
	    }
	    else if(interactive){
	       Config conf(conf_path, custom_conf);
	       search_select(argv[i], p_vect,flags, conf);
	    }
	    else
		  flags << argv[i] << " ";
	 }
   }

   command = stringstream(command.str() + " " + flags.str());
   system(command.str().c_str());
}

string GetStdoutFromCommand(string cmd) {
   string data;
   FILE * stream;
   const int max_buffer = 256;
   char buffer[max_buffer];
   cmd.append(" 2>&1");
   
   stream = popen(cmd.c_str(), "r");
   
   if (stream) {
      while (!feof(stream))
	if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
      pclose(stream);
   }
   return data;
}

void get_packages(vector<Package>  &p_vect){
   stringstream packages; 
   packages << GetStdoutFromCommand("/usr/bin/xbps-query -Rs \"*\"");
   while (!packages.eof()){
      string line;
      getline(packages,line);
      //cout << line << endl;
      if (line.size()){
	 p_vect.push_back(Package(line));
      }
   }

}

bool valid_num_input(string input){
   bool valid = true;
   if (input.size() < 1)
      valid = false;
   for (int i = 0; i < input.size(); i++)
      if ((int(input[i]) < int('0') || int(input[i]) > int('9')) && input[i] != ' ')
	valid = false; 
   return valid;
}
int* str_to_int_arr(string str, int &size){
   int* iarr;
   int exponent = 0;
   int index = 0;
   for (int i = 1; i < str.size(); i++)
      if (str[i] == ' ' && str[i-1] != ' ')
	 size++;
   iarr = new int[size];
   iarr[index] = 0;
   for (int i =str.size() -1 ; i >= 0 ; i--){
      if (i && str[i] == ' ' && str[i-1] != ' '){
	 index++;
	 iarr[index] = 0;
	 exponent = 0;
      }
      else{
	 iarr[index] += (int(str[i]) - int('0')) * pow(10,  exponent);
	 exponent++;
      }
   }
   return iarr;


}
void search_select(string query, vector<Package> p_vect, stringstream &flags, Config conf){
   vector<Package> matched; 
   // find any names or descriptions that match the query string
   for (int i =0; i < int(p_vect.size()); i++){
      if ( p_vect.size() && p_vect.at(i).get_name().find(query) < p_vect.at(i).get_name().size()){
	 matched.push_back(p_vect.at(i));
      }
   }
   for (int i =0; i < int(p_vect.size()); i++){
      if (p_vect.at(i).get_desc().find(query) < p_vect.at(i).get_desc().size()){
	 matched.push_back(p_vect.at(i));
      }
   }
   // if there were any matched and the first item already isnt
   // the same as the query string, see if any match the query string
   // and move it to the first position
   if (matched.size() && query != matched.at(0).get_name())
      for (int i = 0; i < int(matched.size()); i++)
	 if (matched.at(i).get_name() == query){
	    Package temp = matched.at(0);
	    matched[0] = matched[i];
	    matched[i] = temp;
	    break;

	 }

   if (matched.size()){
      for (int i = matched.size()-1; i >= 0; i--)
	 matched[i].print(conf, i);
      string input;
      int size;
      do {
	 size = 1;
      cout << conf.color("prompt") << "  Packages to install (ie: 1 2 3):\n: " <<
         conf.color("reset");
      getline(cin,input);
      if (valid_num_input(input)){
         int* iarr =  str_to_int_arr(input,size);
         for (int i = 0; i < size; i++)
   	 flags << matched.at(iarr[i] - 1).get_name() << " ";
	 break;
      }
      else
         cout << conf.color("error") << "Invalid input! enter space seperated numbers" 
   	 << endl << conf.color("reset");
      } while(true);
   }
   else{
         cout << conf.color("error") << "No packages found" 
   	 << endl << conf.color("reset");
	 exit(1);
  }
}
