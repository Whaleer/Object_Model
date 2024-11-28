#include <iostream>
using namespace std;

class String{
private:
    char * str;
    int len;
};

class Word{
private:
    int _occurs;
    String _word;
};

int main(){
    String s1;
    String s2 = s1;
}
