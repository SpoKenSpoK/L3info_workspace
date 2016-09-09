#include "Vignere.hpp"

void Vignere::readText(const std::string& filename){
    std::ifstream thefile(filename, std::ios::in);
    std::string row;

    unsigned int j = 0;
    while(!thefile.eof()){
        getline(thefile,row);
        for(unsigned int i = 0; i < row.length(); ++i)
        {
            tabText += row[i];
            ++j;
        }
    }

    file.close();
}

void Vignere::cryptData(const std::string& key){
    unsigned int j = 0;
    unsigned int value;

    for(unsigned int i = 0; i < tabText.length(); ++i){
        value = int(tabText[i] + (key[j] - 'A'));

        if(value > int('Z')) value = int('A') + (value - int('Z'));

        cryptedData += value;

        if(j == key.length() - 1) j = 0;
        else ++j;
    }
}

void Vignere::writeData(const std::string& filename, const std::string& key) const{
    std::ofstream thefile(filename, std::ios::out | std::ios::trunc);
    thefile << cryptedData << '\n' << key << '\n';
    thefile.close();
}
