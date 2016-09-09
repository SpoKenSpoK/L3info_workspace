#ifndef VIGNERE_HPP
#define VIGNERE_HPP

#include <iostream>
#include <fstream>

class Vignere{
public:
    // Attributs
    std::ifstream file;

    std::string tabText;
    std::string cryptedData;

    // Méthodes
    inline Vignere(){}
    inline ~Vignere(){}
    void readText(const std::string&);
    void cryptData(const std::string&);
    void writeData(const std::string&, const std::string&) const;
};

#endif
