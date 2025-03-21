#pragma once
#include "sha-1.h"
#include <string>
#include <cstring>
using namespace std;

class User {
public:
    User(const string& _name, const string& _login, const uint* _pass)
        : name(_name), login(_login) {
        pass_hash = new uint[5];
        memcpy(pass_hash, _pass, 5 * sizeof(uint));
    }

    ~User() {}

    bool prov(const string& pass) {
        uint* digest = sha1(pass, pass.size());
        bool isValid = true;
        for (int i = 0; i < 5; i++) {
            if (digest[i] != pass_hash[i]) {
                isValid = false;
                break;
            }
        }
        delete[] digest;
        return isValid;
    }
    string name;
private:
    string login;
    uint* pass_hash;
};
