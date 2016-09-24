#include "Vignere.hpp"

int main(){
    Vignere* vignere = new Vignere();
    std::string filename = "vignere_free.txt";
    std::string encryptFilename = "vignere_crypted.txt";
    std::string keyfile = "key.txt";
    std::string key = "KIWOE";

    vignere->readText(filename);
    vignere->cryptData(key);
    vignere->writeData(encryptFilename, keyfile);
    //vignere->decryptWithKey(false);
    vignere->decryptWithOutKey(4);
    vignere->get_All_Length();
    vignere->get_PGCD();
    //vignere->echoResult();
    vignere->max_OccPgcd();
    //vignere->findKey();
    vignere->bruteForce();

    //vignere->cleanDelete();
    //delete vignere;

    return 0;
}
