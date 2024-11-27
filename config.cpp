#include "config.h"

	Config::Config(){
	}
	Config::Config(std::string path, bool custom){
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

	std::string Config::color(std::string key){ return colors[key]; }

	int Config::screen_w(){
		struct winsize sz;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);
		return sz.ws_col;
	}
	std::string Config::translate_color (std::string color){
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



