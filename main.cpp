#include <iostream>
#include <cstring>
#include <fstream>

int isStartLock = 0;

int dotSign(const char *str,const char *pattern, int i) {

    if (str[i] != '\0') {
        return 1;
    }
    return 0;
}

int dollerSign(const char *str,const char *pattern) {

    if (str[strlen(str)-1] == pattern[strlen(pattern)-2] ) {
        return 1;
    }

    return 0;
}

int upperSign(const char *str, const char *pattern) {

    if (str[0] == pattern[1]) {
        return 1;
    }
    return 0;
}


int astrixCase(const char *str,const char *pattern, int i) {
    char letter = *pattern;
    if (letter == '\0'){
        return i;
    }

    while (str[i] != '\0') {
        if (str[i] == letter) {
            return i;
        }
        i++;
    }
    return 0;
}

void computeLPSArray(char* pat, int M, int* lps)
{
    int len = 0;

    lps[0] = 0;

    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0) {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPSearch(char* pat, char* txt)
{
    int M = strlen(pat);
    int N = strlen(txt);

    int lps[M];
    computeLPSArray(pat, M, lps);

    int i = 0;
    int j = 0;
    int temp;

    while ((N - i) >= (M - j)) {

        if(pat[j] == '.' && pat[j-1] != '\\'){
            int found = dotSign(txt, pat, i);
            if (found){
                j++;
                i++;
            }
        }

        else if(pat[j+1] == '$' && pat[j-1] != '\\' ) {
            if(N == strlen(txt)){
                if (j == strlen(pat)-2){
                    int found = dollerSign(txt, pat);
                    if (found) {
                        std::cout << "Found pattern at index " << N << std::endl;
                        return;
                    }
                }
                else{
                    std::cout << "Invalid Pattern";
                    return;
                }
            }
        }

        else if(pat[j] == '^' && pat[j-1] != '\\' && i == 0) {

            int found = upperSign(txt, pat);
            if (found) {
                j += 2;
                i += 3;
            }
        }

        if(pat[j] == '*' && pat[j-1] != '\\'){

            int found = dotSign(txt, pat, i);
            temp = found;
            if (found){
                j+=temp;
                i++;
            }
        }

        else if (pat[j] == txt[i]) {
            j++;
            i++;
        }

        if (j >= M) {
            std::cout << "Found pattern at index " << i - j << std::endl;
            j = lps[j - 1];
        }

        else if (i < N && (pat[j] != txt[i] && pat[j] != '.' && pat[j] != '^'  && pat[j] != '$')) {
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
}

int main() {

    std::ifstream patternFile("pattern.txt");

    std::ifstream textFile("text.txt");

    if (!patternFile.is_open()) {
        std::cerr << "Failed to open pattern.txt" << std::endl;
        return 1;
    }
    std::string pattern;

    std::getline(patternFile, pattern);

    patternFile.close();

    if (!textFile.is_open()) {

        std::cerr << "Failed to open text.txt" << std::endl;

        return 1;
    }
    std::string text;

    std::getline(textFile, text);

    textFile.close();

    char* pat = new char[pattern.size() + 1];

    strcpy(pat, pattern.c_str());

    char* txt = new char[text.size() + 1];

    strcpy(txt, text.c_str());

    KMPSearch(pat, txt);

    delete[] pat;
    delete[] txt;

    return 0;

}