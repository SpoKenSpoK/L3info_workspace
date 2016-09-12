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

    thefile.close();
}

void Vignere::cryptData(const std::string& key){
    unsigned int j = 0;
    unsigned int value;

    this->key = key;

    for(unsigned int i = 0; i < tabText.length(); ++i){
        value = int(tabText[i] + (key[j] - 'A'));

        if(value > int('Z')) value = int('A') + (value - int('Z'));

        cryptedData += value;

        if(j == key.length() - 1) j = 0;
        else ++j;
    }

    std::cerr << "cryptedData : " << cryptedData << std::endl;
    std::cerr << "key : " << key << std::endl;
}

void Vignere::writeData(const std::string& filename, const std::string& filekey) const{
    std::ofstream thefile(filename, std::ios::out | std::ios::trunc);
    std::ofstream keyfile(filekey, std::ios::out | std::ios::trunc);
    thefile << cryptedData << '\n';
    keyfile << key << '\n';
    thefile.close();
    keyfile.close();
}

void Vignere::decryptWithKey(bool read){
    if(read){
        std::string filename;
        std::string tmp;
        std::cerr << "Entrez un nom de fichier" << std::endl;
        std::cin >> filename;

        readText(filename);
        cryptedData = tabText;
    }

    int value;
    unsigned int j = 0;
    tabText = "";

    for(unsigned int i = 0; i < cryptedData.length(); ++i){
        value = int(cryptedData[i] - (key[j] - 'A'));

        std::cerr << int(cryptedData[i]) << " | " << key[j] - 'A' << " | " << value << std::endl;

        if(value < int('A')){
            std::cerr << int(cryptedData[i]) << std::endl;
            std::cerr << int('Z') - value << std::endl;
            value = int('Z') - value;
        }



        tabText += value;

        if(j == key.length() - 1) j = 0;
        else ++j;
    }

    std::cerr << tabText << std::endl;
}
