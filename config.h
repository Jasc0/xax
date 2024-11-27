#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/limits.h>

// ANSI color definitions
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

class Config {
private:
    std::unordered_map<std::string, std::string> colors = {
        {"error", BOLDRED},
        {"reset", RESET},
        {"name_installed", GREEN},
        {"name", BLUE},
        {"prompt", YELLOW}
    };

public:
    // Default constructor
    Config();

    // Constructor to initialize with a configuration file
    Config(std::string path, bool custom);

    // Returns the ANSI color code for a given key
    std::string color(std::string key);

    // Returns the screen width in columns
    int screen_w();

    // Translates a string representation of a color to its corresponding ANSI code
    std::string translate_color(std::string color);
};

#endif // CONFIG_H
