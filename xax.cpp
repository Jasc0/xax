#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#include <queue>
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


//using namespace std;

class Config{
	std::unordered_map<std::string,std::string> colors = {{"error",BOLDRED},
		{"reset",RESET}, {"name_installed",GREEN}, {"name",BLUE}, {"prompt",YELLOW}
	};
	public:
	Config(){
	}
	Config(std::string path, bool custom){
		std::string key;
		std::string value;
		std::ifstream conf_f(path);
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
			std::cout << colors["error"] << "Could not read config file: " << path 
				<< " using defaults" << colors["reset"] << std::endl;

	}
	std::string color(std::string key){ return colors[key]; }
	int screen_w(){
		struct winsize sz;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);
		return sz.ws_col;
	}
	std::string translate_color (std::string color){
		std::string translated_color;
		for (int i = 0; i < color.size(); i++)
			color[i] = toupper(color[i]);
		if (color == "BLACK")
			translated_color = std::string(BLACK);
		else if (color == "RED")
			translated_color = RED;
		else if (color == "GREEN")
			translated_color = std::string(GREEN);
		else if (color == "YELLOW")
			translated_color = std::string(YELLOW);
		else if (color == "BLUE")
			translated_color = std::string(BLUE);
		else if (color == "MAGENTA")
			translated_color = std::string(MAGENTA);
		else if (color == "CYAN")
			translated_color = std::string(CYAN);
		else if (color == "WHITE")
			translated_color = std::string(WHITE);
		else if (color == "BOLDBLACK")
			translated_color = std::string(BOLDBLACK);
		else if (color == "BOLDRED")
			translated_color = std::string(BOLDRED);
		else if (color == "BOLDGREEN")
			translated_color = std::string(BOLDGREEN);
		else if (color == "BOLDYELLOW")
			translated_color = std::string(BOLDYELLOW);
		else if (color == "BOLDBLUE")
			translated_color = std::string(BOLDBLUE);
		else if (color == "BOLDMAGENTA")
			translated_color = std::string(BOLDMAGENTA);
		else if (color == "BOLDCYAN")
			translated_color = std::string(BOLDCYAN);
		else if (color == "BOLDWHITE")
			translated_color = std::string(BOLDWHITE);
		else
			translated_color = std::string("");
		return translated_color;
	}



};
class Package{
	std::string name;
	std::string description;
	std::string version; //stored as string since version numbers include _ char
	bool installed;
	public:
	Package(){
		name = "";
		description = "";
		version = "";
		installed = false;
	}
	Package(std::string line){
		int first_instance;
		std::stringstream line_s;
		line_s << line;
		this->installed = false;
		if (line[1] == '*')
			this->installed = true;
		std::string read;
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
		std::cout << std::left;
		std::cout << conf.color("prompt") <<  index+1 << ") " ;
		if (installed)
			std::cout << conf.color("name_installed") <<  name <<": " << version <<" [installed]";
		else
			std::cout << conf.color("name")  <<  name <<": " << version;
		std::cout  << std::endl << conf.color("reset") ;
		if (description.size() > (conf.screen_w()))
			std::cout << description.substr(0,(conf.screen_w())) << std::endl;
		else
			std::cout << description << std::endl;
	}
	std::string get_desc(){
		std::string desc_lower = "";
		for (int i = 0; i < this->description.size(); i++)
			desc_lower += std::tolower(this->description.at(i));
		return desc_lower; 
	}
	std::string get_name(){
		return this->name; 
	}
	std::string get_lname(){
		std::string name_lower = "";
		for (int i = 0; i < this->name.size(); i++)
			name_lower += std::tolower(this->name.at(i));
		return name_lower; 
	}
	bool is_installed(){return installed;}
	friend bool operator==(const Package& lhs, const Package& rhs);
	friend bool operator!=(const Package& lhs, const Package& rhs);


};
bool operator==(const Package& lhs, const Package& rhs) {
	return (lhs.name == rhs.name && lhs.description == rhs.description && lhs.version == rhs.version);
}
bool operator!=(const Package& lhs, const Package& rhs) {
	return (lhs.name != rhs.name && lhs.description != rhs.description && lhs.version != rhs.version);
}

std::string GetStdoutFromCommand(std::string cmd);
void get_packages(std::vector<Package> &p_vect);
int* str_to_int_arr(std::string str);
std::queue<std::string>* split_string(std::string start);
std::vector<Package>* query_packages(std::queue<std::string>* query, std::vector<Package>* queried_from, bool first_run);
void select_install(std::vector<Package>* matched,  std::stringstream &flags, Config conf);

int main(int argc, char* argv[]){
	bool interactive = true;
	bool custom_conf = false;
	std::string conf_path = std::string(getenv("HOME")) + "/.config/xax.conf";
	std::stringstream flags = std::stringstream("") ;
	std::stringstream command;
	command = std::stringstream("/usr/bin/sudo /usr/bin/xbps-install");
	std::vector<Package> p_vect;
	std::queue<std::string>* query = new std::queue<std::string>();
	get_packages(p_vect);
	if(argc < 2){
		flags << "-Su";
	}
	else{
		for ( int i =1; i < argc; i++){
			if (argv[i][0] == '-'){
				if(std::string(argv[i]).find('R') < std::string(argv[i]).size()){
					command = std::stringstream("/usr/bin/sudo /usr/bin/xbps-remove");
					interactive = false;
				}
				else if(std::string(argv[i]).find('Q') < std::string(argv[i]).size()){
					command = std::stringstream("/usr/bin/sudo /usr/bin/xbps-query");
					interactive = false;
				}
				else if(std::string(argv[i]).find('I') < std::string(argv[i]).size()){
					interactive = false;
				}
				else if(std::string(argv[i]).find("--conf") < std::string(argv[i]).size()){
					i++;
					conf_path = argv[i];
					custom_conf = true;
				}
				else
					flags << argv[i] << " ";
			}
			else if(interactive){
				std::string temp =  argv[i];
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				query->push(temp);
			}
			else
				flags << argv[i] << " ";
		}
	}

	Config conf(conf_path, custom_conf);
	std::vector<Package>* packs = query_packages(query, &p_vect, true);
	select_install(packs, flags, conf);
	command = std::stringstream(command.str() + " " + flags.str());
	system(command.str().c_str());
}

std::string GetStdoutFromCommand(std::string cmd) {
	std::string data;
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

void get_packages(std::vector<Package>  &p_vect){
	std::stringstream packages; 
	packages << GetStdoutFromCommand("/usr/bin/xbps-query -Rs \"*\"");
	while (!packages.eof()){
		std::string line;
		getline(packages,line);
		//cout << line << endl;
		if (line.size()){
			p_vect.push_back(Package(line));
		}
	}

}

bool valid_num_input(std::string input){
	bool valid = true;
	if (input.size() < 1)
		valid = false;
	for (int i = 0; i < input.size(); i++)
		if ((int(input[i]) < int('0') || int(input[i]) > int('9')) && input[i] != ' ')
			valid = false; 
	return valid;
}
int* str_to_int_arr(std::string str, int &size){
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

std::vector<Package>* query_packages(std::queue<std::string>* query, std::vector<Package>* queried_from, bool first_run = true){
	std::string cur_query = query->front();
	query->pop();
	std::vector<Package>* matched = new std::vector<Package>(); 
	// find any names or descriptions that match the query string
	for (int i =0; i < int(queried_from->size()); i++){
		if ( queried_from->at(i).get_lname().find(cur_query) < queried_from->at(i).get_lname().size()){
			matched->push_back(queried_from->at(i));
		}
	}
	for (int i =0; i < int(queried_from->size()); i++){
		if (queried_from->at(i).get_desc().find(cur_query) < queried_from->at(i).get_desc().size())
		{
			bool already_matched = false;
			for (int j =0; j < matched->size(); j++)
				if (queried_from->at(i) == matched->at(j))
					already_matched = true;

			if (!already_matched)
				matched->push_back(queried_from->at(i));
		}
	}
	// if there were any matched and the first item already isnt
	// the same as the query string, see if any match the query string
	// and move it to the first position
	if (matched->size() && first_run && cur_query.compare(matched->at(0).get_lname()) != 0)
		for (int i = 0; i < int(matched->size()); i++)
			if (matched->at(i).get_lname().compare(cur_query) == 0){
				Package temp = matched->at(0);
				matched->at(0) = matched->at(i);
				matched->at(i) = temp;
				break;

			}
	if (query->empty()){
		delete query;
		return matched;
	}
	else{
		return query_packages(query, matched, false);
	}
}


void select_install(std::vector<Package>* matched,  std::stringstream &flags, Config conf){

	if (matched->size()){
		for (int i = matched->size()-1; i >= 0; i--)
			matched->at(i).print(conf, i);
		std::string input;
		int size;
		do {
			size = 1;
			std::cout << conf.color("prompt") << "  Packages to install (ie: 1 2 3):\n: " <<
				conf.color("reset");
			getline(std::cin,input);
			if (valid_num_input(input)){
				int* iarr =  str_to_int_arr(input,size);
				for (int i = 0; i < size; i++)
					flags << matched->at(iarr[i] - 1).get_name() << " ";
				break;
			}
			else
				std::cout << conf.color("error") << "Invalid input! enter space seperated numbers" 
					<< std::endl << conf.color("reset");
		} while(true);
	}
	else{
		std::cout << conf.color("error") << "No packages found" 
			<< std::endl << conf.color("reset");
		exit(1);
	}
}
