#include "Vignere.hpp"

int main(){
    Vignere* vignere = new Vignere();
    std::string filename = "vignere_free.txt";
    std::string encryptFilename = "vignere_crypted.txt";
    std::string key = "MARJOLAINE";

    vignere->readText(filename);
    vignere->cryptData(key);
    vignere->writeData(encryptFilename, key);
    
/*
    std::cerr << vignere->tabText << std::endl;
    std::cerr << vignere->cryptedData << std::endl;
*/

    delete vignere;
    return 0;
}
