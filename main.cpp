#include "xax.h"

bool operator==(const Package& lhs, const Package& rhs) {
	return (lhs.name == rhs.name && lhs.description == rhs.description && lhs.version == rhs.version);
}
bool operator!=(const Package& lhs, const Package& rhs) {
	return (lhs.name != rhs.name && lhs.description != rhs.description && lhs.version != rhs.version);
}

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
		interactive = false;
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

	if(interactive){
		Config conf(conf_path, custom_conf);
		std::vector<Package>* packs = query_packages(query, &p_vect, true);
		select_install(packs, flags, conf);
	}
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
