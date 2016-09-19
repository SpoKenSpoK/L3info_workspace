#ifndef VIGNERE_HPP
#define VIGNERE_HPP

#include <iostream>
#include <fstream>
#include <string>

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

    unsigned int nbChar;

    thestr* tabStr;
    unsigned int nbStr;

    unsigned long int* tabLenOfKey;
    unsigned long int nbLenOfKey;
    unsigned long int* tabOfpgcd;
    unsigned long int* tabOfOcc;
    unsigned long int nb_pgcd;

    bool megaBool;

    unsigned int keyLen;
    unsigned int* tabTmpAlphabet;
    const char tabAlphabet[26] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G','H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U','V', 'W', 'X', 'Y', 'Z'
    };

    //
    const float tabFrequencyLatin[26] = {
        8.89, 1.58, 3.99, 2.77, 11.38, 0.93, 1.21, 0.69, 11.44, 3.15, 5.38, 6.28, 5.40, 3.03, 1.51, 6.67, 7.60, 8.00, 8.46, 0.96, 0.60, 0.07, 0.01
    };

    // Méthodes
    inline Vignere(){nbStr=1;}
    inline ~Vignere(){ delete[] tabLenOfKey; delete[] tabOfOcc; delete[] tabOfpgcd; }
    void readText(const std::string&);
    void cryptData(const std::string&);
    void writeData(const std::string&, const std::string&) const;
    void decryptWithKey(bool =false);
    void decryptWithOutKey(unsigned int );
    void sortString(unsigned int );
    std::string getLittleStr(unsigned int );
    void echoResult();
    void cleanDelete();
    void get_All_Length();
    void get_PGCD();
    unsigned long int make_PGCD(unsigned long int, unsigned long int );
    long int getMax( long int, long int );
    long int getMin( long int, long int );
    void max_OccPgcd();
    void bruteForce();
    void GenerateStr(unsigned int, std::string );
};

#endif
