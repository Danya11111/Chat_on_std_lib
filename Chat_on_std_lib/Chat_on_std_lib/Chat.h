#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#include "Message.h"
#include "TrieNode.h"
#include "Graph.h"
#include "User.h"

using namespace std;

class Chat {
private:
    unordered_map<string, User> Users;
    vector<Message> publicMessages;
    map<string, vector<Message>> privateMessages;
public:
    unique_ptr<Graph> friends = make_unique<Graph>();
    unique_ptr<Trie> trie = make_unique<Trie>();

    bool reg(const string& _name, const string& _login, const string& _pass);
    bool log(string _login, string _pass);
    void logoutUser(const string& login);
    void sendMessage(const string& senderLogin, const string& message, const string& recipient = "");
    void listUsers(string login) const;
    void viewMessages(const string& login) const;
    void addFriend(const string& user_name);
    string T9();
    void insert_lib();
};
