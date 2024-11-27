#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

class Config; // Forward declaration for Config

class Package {
private:
    std::string name;
    std::string description;
    std::string version; // Stored as string since version numbers include special characters like '_'
    bool installed;

public:
    // Default constructor
    Package();

    // Constructor that initializes the Package object from a formatted string
    explicit Package(std::string line);

    // Prints the package information
    void print(Config conf, int index);

    // Getter for the description (lowercased)
    std::string get_desc();

    // Getter for the package name
    std::string get_name();

    // Getter for the package name (lowercased)
    std::string get_lname();

    // Checks if the package is installed
    bool is_installed();

    // Equality operator
    friend bool operator==(const Package& lhs, const Package& rhs);

    // Inequality operator
    friend bool operator!=(const Package& lhs, const Package& rhs);
};


#endif // PACKAGE_H
