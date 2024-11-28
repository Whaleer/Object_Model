#include <iostream>
#include <cstring>
using namespace std;

class Word{
public:
    Word(const char * s):str(new char[strlen(s)+1]){
        strcpy(str,s);
        cnt = 1;
    }
    ~Word(){
        delete str;
    }
private:
    int cnt;
    char * str;
};

int main(){
    Word noun("book");
    Word verb = noun;
    return 0;
}