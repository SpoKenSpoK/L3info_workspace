#include "Vignere.hpp"

void Vignere::readText(const std::string& filename){
    std::ifstream thefile(filename, std::ios::in);
    std::string row;

    unsigned int j = 0;
    while(!thefile.eof()){
        getline(thefile,row);
        for(unsigned int i = 0; i < row.length(); ++i){
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
    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i){
        tabStr[i].tab = new unsigned int[cryptedData.length() - (nbChar - 1)];
        for(unsigned int j=0; j<cryptedData.length() - (nbChar - 1); ++j)
            tabStr[i].tab[j] = 0;
    }

    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i) sortString(i);
}

void Vignere::sortString(unsigned int index){
    unsigned int cpt = 0;
    bool done = false;
    std::string tmpStr = getLittleStr(index);

    while(!done){
        if(tmpStr == tabStr[cpt].str){
            tabStr[cpt].tab[tabStr[cpt].nbocc] = index;
            tabStr[cpt].nbocc +=1;
            done = true;
        }
        ++cpt;

        if(cpt > nbStr ){
            tabStr[nbStr].nbocc = 1;
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

void Vignere::get_All_Length(){
    // Parcourir l'ensemble des données trouvées, ne prendre en compte que celles ayant une occurence strictement suppérieure à 1
    unsigned int cpt;

    tabLenOfKey = new unsigned long int[cryptedData.length() - (nbChar - 1)];
    nbLenOfKey = 0;

    for(unsigned int i=0; i<nbStr; ++i){
        if(tabStr[i].nbocc > 1){
            for(unsigned int j=tabStr[i].nbocc-1; j>0; --j){
                cpt = 1;
                while(cpt <= j){
                    tabLenOfKey[nbLenOfKey] = tabStr[i].tab[j] - tabStr[i].tab[j - cpt];
                    ++nbLenOfKey;
                    ++cpt;
                }
            }
        }
    }
}

void Vignere::get_PGCD(){
    tabOfpgcd = new unsigned long int[nbLenOfKey*nbLenOfKey];
    tabOfOcc = new unsigned long int[nbLenOfKey*nbLenOfKey];
    nb_pgcd = 0;
    unsigned int tmp;
    bool done;

    for(unsigned int i=0; i<nbLenOfKey; ++i){
        for(unsigned int j=0; j<nbLenOfKey; ++j){
            tmp = make_PGCD(tabLenOfKey[i], tabLenOfKey[j]);
            done = false;

            for(unsigned int k=0; k<nb_pgcd; ++k){
                if(tmp == tabOfpgcd[k]){
                    tabOfOcc[k] += 1;
                    done = true;
                }
            }

            if(!done){
                tabOfpgcd[nb_pgcd] = tmp;
                tabOfOcc[nb_pgcd] = 1;
                ++nb_pgcd;
            }
        }
    }
}

unsigned long int Vignere::make_PGCD(unsigned long int integerOne, unsigned long int integerTwo){
    long int result, min;
    bool done = false;

    result = getMax(integerOne, integerTwo);
    min = getMin(integerOne, integerTwo);

    while(!done){
        if(result != min){
            result -= min;
            min = result;
        }
        else done = true;
    }

    return result;
}

void Vignere::max_OccPgcd(){
    unsigned int tmp;
    bool done = false;
    long unsigned int cpt = 0;

    while(!done){
        cpt = 0;
        for(unsigned long int i=0; i<nb_pgcd; ++i){
            if(tabOfOcc[i] > tabOfOcc[i+1]){
                tmp = tabOfOcc[i];
                tabOfOcc[i] = tabOfOcc[i+1];
                tabOfOcc[i+1] = tmp;

                tmp = tabOfpgcd[i];
                tabOfpgcd[i] = tabOfpgcd[i+1];
                tabOfpgcd[i+1] = tmp;

                ++cpt;
            }
        }

        if(cpt == 0) done = true;
    }

    short one = 0;

    for(unsigned int i = nb_pgcd; i > 0; --i){
        if(tabOfpgcd[i] > 1 && tabOfpgcd[i] <= 10){
            if(one == 0){
                keyLen = tabOfpgcd[i];
                one = 1;
            }
            std::cerr << " Clé possible : " << tabOfpgcd[i] << " | Nb fois PGCD trouvé : " << tabOfOcc[i] << std::endl;
        }
    }
}

void Vignere::bruteForce(){
    std::cerr << std::endl << "Démarrage du bruteForce ..." << std::endl;
    while(!megaBool){
		static unsigned int strLen = keyLen;
		GenerateStr(strLen, "");
		strLen++;
	}
}

void Vignere::GenerateStr(unsigned int length, std::string str){
	if(length == 0){
		std::cout << str << std::endl;
		return;
	}

    int value;
    unsigned int j = 0;
    tabText = "";

    tabTmpAlphabet = new unsigned int[26];
    for(unsigned int i = 0; i < 26; ++i){
        tabTmpAlphabet[i] = 0;
    }
    double tabFloat[26];

	for(unsigned int i = 0; i < 26; ++i){
		std::string tmpStr = str + tabAlphabet[i];
        key = tmpStr;

        for(unsigned int i = 0; i < cryptedData.length(); ++i){
            value = int(cryptedData[i] - (key[j] - 'A'));

            if(value < int('A')) value = int('Z') -  (int('A') - value);

            tabText += value;

            if(j == key.length() - 1) j = 0;
            else ++j;
        }

        for(unsigned int i = 0; i < cryptedData.length(); ++i){
            for(unsigned int j = 0; j < 26; ++j){
                if(tabText[i] == char('A'+j)){
                    tabTmpAlphabet[j] += 1;
                }
            }
        }

        for(unsigned int i=0; i < 26; ++i){
            tabFloat[i] = double(tabTmpAlphabet[i]) / double(cryptedData.length());
            //std::cerr << tabFloat[i] << std::endl;
        }

        //double tmp;
        bool done = false;
        unsigned int cpt = 0;
        double tmp;
        unsigned int tmpU;
        //unsigned indexTwo;

        unsigned int tabIndex[26] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };

        while(!done){
            cpt = 0;
            for(unsigned long int i=0; i<26; ++i){
                if(tabFloat[i] > tabFloat[i+1]){

                    tmp = tabFloat[i];
                    tabFloat[i] = tabFloat[i+1];
                    tabFloat[i+1] = tmp;

                    tmpU = tabIndex[i];
                    tabIndex[i] = tabIndex[i+1];
                    tabIndex[i+1] = tmpU;

                    ++cpt;
                }
            }
            if(cpt == 0) done = true;
        }

        for(unsigned int i=0; i<26; ++i){
            std::cerr << tabIndex[i] << ".";
        }
        std::cerr << std::endl;

        if( (tabIndex[25] == 5 && tabIndex[24] == 20 && tabIndex[23] == 9) || (tabIndex[24] == 5 && tabIndex[25] == 9 && tabIndex[23] == 20) ){
            std::cerr << "Clé : " << key << std::endl;
            megaBool = true;
            return;
        }

        //megaBool = true;

        if(!megaBool) GenerateStr(length-1, tmpStr);
	}
}

long int Vignere::getMax(long int integerOne, long int integerTwo){
    if(integerOne >= integerTwo) return integerOne;
    return integerTwo;
}

long int Vignere::getMin(long int integerOne, long int integerTwo){
    if(integerOne >= integerTwo) return integerTwo;
    return integerOne;
}

void Vignere::echoResult(){
    for(unsigned long int i=0; i<nbStr; ++i){
        std::cerr << tabStr[i].str << " | " << tabStr[i].nbocc << " Index : " << tabStr[i].tab[0] << " " << tabStr[i].tab[1] << " " << tabStr[i].tab[2] << " " << tabStr[i].tab[3] << std::endl;
    }

    std::cerr << " ----------------- " << std::endl;

    for(unsigned long int i=0; i<nbLenOfKey; ++i){
        std::cerr << tabLenOfKey[i] << std::endl;
    }

    std::cerr << " ----------------- " << std::endl;

    for(unsigned long int i=0; i<nb_pgcd; ++i){
        std::cerr << tabOfpgcd[i] << " | Occ : " << tabOfOcc[i] << std::endl;
    }


}

void Vignere::cleanDelete(){
    for(unsigned int i=0; i<cryptedData.length() - (nbChar - 1); ++i){
        delete[] tabStr[i].tab;
    }
    delete[] tabStr;
}

// Faire le calcul de PGCD
