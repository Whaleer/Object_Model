#include <iostream>
#include <cstring>
using namespace std;


int main(){
    char boy[] = "Danny";
    char *p_son;

    p_son = new char[strlen(boy) + 1];
    strcpy(p_son, boy);
    cout << "The boy's name is " << boy << endl;
    cout << "The son's name is " << p_son << endl;

    string girl = "Lucy";
    string daugter;
    // string::operator=()
    daugter = girl;
    cout << "The girl's name is " << girl << endl;
    cout << "The daughter's name is " << daugter << endl;

    return 0;
}

