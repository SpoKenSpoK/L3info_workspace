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

    /*std::cerr << "cryptedData : " << cryptedData << std::endl;
    std::cerr << "key : " << key << std::endl;*/
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

    unsigned int tab[26];
    for(unsigned int i=0; i<26; ++i)
        tab[i] = 0;

    for(unsigned int i = 0; i < cryptedData.length(); ++i){
        for(unsigned int j = 0; j < 26; ++j){
            if(tabText[i] == char('A'+j)){
                tab[j] += 1;
            }
        }
    }

    std::cerr << " Fréquence des lettres dans le texte non crypte : " << std::endl;
    for(unsigned int i = 0; i < 26; ++i){
        std::cerr << char('A'+i) << " : " << tab[i] << std::endl;
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
        if(tabOfpgcd[i] > 1 && tabOfpgcd[i] < 6){
            if(one == 0){
                keyLen = tabOfpgcd[i];
                one = 1;
            }
            std::cerr << " Clé possible : " << tabOfpgcd[i] << " | Nb fois PGCD trouvé : " << tabOfOcc[i] << std::endl;
        }
    }
}

void Vignere::findKey(){
    std::string* tab = new std::string[keyLen];
    tabTmpAlphabet = new unsigned int[26];

    //unsigned int cpt = 0;
    for(unsigned int i=0; i<cryptedData.length(); ++i){
        for(unsigned int j=0; j<keyLen; ++j){
            tab[j] += cryptedData[i+j];
        }
    }

    for(unsigned int j=0; j<keyLen; ++j){
        for(unsigned int i=0; i<cryptedData.length()/keyLen; ++i){
            for(unsigned int k=0; k<26; ++k){
                if( char('A'+k) == tab[j][i] ){
                    tabTmpAlphabet[k]+=1;
                }
            }
        }
    }

    delete[] tab;
}

void Vignere::bruteForce(){
    std::cerr << std::endl << "Démarrage du bruteForce ..." << std::endl;
    finalStr = "";

    for(unsigned int i=0; i<keyLen; ++i){
        finalStr += "Z";
    }

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

    int value = 0;
    unsigned int j = 0;


    tabTmpAlphabet = new unsigned int[26];
    double tabFloat[26];

    for(unsigned int i=0; i<26; ++i){
        tabFloat[i] = 0.00;
    }

	for(unsigned int i = 0; i < 26; ++i){
        tabText = "";

		std::string tmpStr = str + tabAlphabet[i];
        for(unsigned int i = 0; i < cryptedData.length(); ++i){
            value = int(cryptedData[i] - (tmpStr[j] - 'A'));

            if(value < int('A')) value = int('Z') -  (int('A') - value);

            tabText += value;

            if(j == tmpStr.length() - 1) j = 0;
            else ++j;
        }

        for(unsigned int i = 0; i < 26; ++i)
            tabTmpAlphabet[i] = 0;

        for(unsigned int i = 0; i < cryptedData.length(); ++i){
            for(unsigned int j = 0; j < 26; ++j){
                if(tabText[i] == char('A'+j)){
                    tabTmpAlphabet[j] += 1;
                }
            }
        }

        for(unsigned int i=0; i < 26; ++i){
            tabFloat[i] = (double(tabTmpAlphabet[i]) * 100) / double(cryptedData.length());
        }

        if(!megaBool && tabFloat[4] > 10.0 && tabFloat[8] > 10.0 && tabFloat[19] > 8.00 && tabFloat[20] > 8.00){
            std::cerr << " Clé probable : " << tmpStr << std::endl;
            megaBool = true;
        }

        if(tmpStr == finalStr){
            megaBool = true;
            std::cerr << "bruteForce terminé, la clé " << finalStr <<" a ete atteinte." << std::endl;
        }

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
