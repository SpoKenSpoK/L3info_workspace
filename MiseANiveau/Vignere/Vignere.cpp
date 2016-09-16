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
    tabStr = new thestr[cryptedData.length() - (nbChar - 1)];
    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i) tabStr[i].tab = new unsigned int[cryptedData.length() - (nbChar - 1)];

    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i){
        // Tester tout d'abord deux par deux
        sortString(nbChar,i);
    }
}

void Vignere::sortString(unsigned int nbChar, unsigned int index){
    unsigned int cpt = 0;
    std::string tmpStr;

    tmpStr = getLittleStr(nbChar,index);

    do{
        if(tmpStr == tabStr[cpt].str){
            tabStr[cpt].nbocc +=1;
            tabStr[cpt].tab[getTailFromTab(tabStr[cpt].tab)] = cpt;
        }
        else{
            /*tabStr[getTailFromTabStruct(tabStr)].nbocc = 1;
            tabStr[getTailFromTabStruct(tabStr)].tab[0] = cpt;
            tabStr[getTailFromTabStruct(tabStr)].str = tmpStr;
            ++nbStr;*/
        }
        ++cpt;
    }while( cpt < cryptedData.length() - (nbChar - 1) );
}

// Cette fonction retourne
std::string Vignere::getLittleStr(unsigned int nbChar, unsigned int index){
    std::string tmpStr;
    for(unsigned int i=0; i<nbChar; ++i){
        tmpStr+=cryptedData[index+i];
    }
    return tmpStr;
}

unsigned int Vignere::getTailFromTab(unsigned int tabInt[]){
    for(unsigned int i=0; i<cryptedData.length(); ++i) if(tabInt[i] == 0) return i;
    return 0;
}

unsigned int Vignere::getTailFromTabStruct(thestr tab[]){
    for(unsigned int i=0; i<nbStr; ++i) if(tab[i].nbocc == 0) return i;
    return 0;
}

/*void Vignere::parseStruct(){

}*/

/*void Vignere::addOccurence(unsigned int index){
}*/
