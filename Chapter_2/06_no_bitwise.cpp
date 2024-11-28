#include <iostream>
#include <string>
using namespace std;

class String {
public:
    String(const char *);
    String(const String &);
    ~String();
};

class Word {
public:
    Word(const String &s) : str(s), cnt(1) {}
    ~Word();
private:
    int cnt;
    String str;
};

