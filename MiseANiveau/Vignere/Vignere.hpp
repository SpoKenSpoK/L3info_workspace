#ifndef VIGNERE_HPP
#define VIGNERE_HPP

#include <iostream>
#include <fstream>
#include <vector>

struct thestr{
    std::string str; // La chaîne de caractère trouvée dans le texte crypté (minimum deux caractères)
    unsigned int nbocc; // Le nombre de fois qu'apparaît la châine de caractère
    unsigned int* tab; // Les index où apparaissent cette chaîne de caractère
};

class Vignere{
public:
    // Attributs
    std::ifstream file;
    std::string tabText;
    std::string cryptedData;
    std::string key;

    thestr* tabStr;
    unsigned int nbStr;

    // Méthodes
    inline Vignere(){nbStr=1;}
    inline ~Vignere(){}
    void readText(const std::string&);
    void cryptData(const std::string&);
    void writeData(const std::string&, const std::string&) const;
    void decryptWithKey(bool =false);
    void decryptWithOutKey(unsigned int );
    unsigned int getTailFromTab(unsigned int []);
    unsigned int getTailFromTabStruct(thestr []);
    void sortString(unsigned int nbChar, unsigned int index);
    std::string getLittleStr(unsigned int, unsigned int );
    //unsigned int mashupStr(const std::string&);
};

#endif
