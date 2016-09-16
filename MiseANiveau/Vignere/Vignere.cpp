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
        std::cerr << "Entrez un nom de fichier : " << std::endl;
        std::cin >> filename;

        readText(filename);
        cryptedData = tabText;
    }

    int value;
    unsigned int j = 0;
    tabText = "";

    for(unsigned int i = 0; i < cryptedData.length(); ++i){
        value = int(cryptedData[i] - (key[j] - 'A'));

        if(value < int('A')) value = int('Z') -  (int('A') - value);

        tabText += value;

        if(j == key.length() - 1) j = 0;
        else ++j;
    }

    std::cerr << "decryptedData : " << tabText << std::endl;
}

void Vignere::decryptWithOutKey(unsigned int nbChar){
    nbStr = 0;
    this->nbChar = nbChar;
    tabStr = new thestr[cryptedData.length() - (nbChar - 1)];
    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i)
        tabStr[i].tab = new unsigned int[cryptedData.length() - (nbChar - 1)];

    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i){
        sortString(i);
    }
}

void Vignere::sortString(unsigned int index){
    unsigned int cpt = 0;
    bool done = false;
    std::string tmpStr = getLittleStr(index);

    while(!done){
        if(tmpStr == tabStr[cpt].str){
            tabStr[cpt].nbocc +=1;
            tabStr[cpt].tab[tabStr[cpt].lengthTab] = index;
            tabStr[cpt].lengthTab +=1;
            done = true;
        }
        ++cpt;

        if(cpt > nbStr ){
            tabStr[nbStr].nbocc = 1;
            tabStr[nbStr].lengthTab = 1;
            tabStr[nbStr].tab[0] = index;
            tabStr[nbStr].str = tmpStr;
            ++nbStr;
            done = true;
        }
    }
}

// Cette fonction retourne
std::string Vignere::getLittleStr(unsigned int index){
    std::string tmpStr;
    for(unsigned int i=0; i<nbChar; ++i){
        tmpStr+=cryptedData[index+i];
    }
    return tmpStr;
}

void Vignere::echoResult(){
    for(unsigned int i=0; i<nbStr; ++i)
        std::cerr << tabStr[i].str << " | " << tabStr[i].nbocc << std::endl;
}

// Faire le calcul de PGCD
