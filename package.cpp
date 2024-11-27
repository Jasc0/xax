#include "package.h"
#include "config.h"

	Package::Package(){
		name = "";
		description = "";
		version = "";
		installed = false;
	}

	Package::Package(std::string line){
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

	void Package::print(Config conf, int index){
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

	std::string Package::get_desc(){
		std::string desc_lower = "";
		for (int i = 0; i < this->description.size(); i++)
			desc_lower += std::tolower(this->description.at(i));
		return desc_lower; 
	}

	std::string Package::get_name(){
		return this->name; 
	}

	std::string Package::get_lname(){
		std::string name_lower = "";
		for (int i = 0; i < this->name.size(); i++)
			name_lower += std::tolower(this->name.at(i));
		return name_lower; 
	}

	bool Package::is_installed(){return installed;}


