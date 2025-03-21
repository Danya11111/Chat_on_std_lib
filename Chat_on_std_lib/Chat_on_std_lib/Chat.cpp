#include "Chat.h"

bool Chat::log(string _login, string _pass) {
    unordered_map<string, User>::iterator it = Users.find(_login);
    if (it != Users.end() && it->second.prov(_pass)) {
        cout << "User " << it->second.name << " logged in." << endl;
        return true;
    }
    cout << "Error: incorrect login or password." << endl;
    return false;
}

bool Chat::reg(const string& _name, const string& _login, const string& _pass) {
    auto it = Users.find(_login);
    if (it != Users.end()) {
        cout << "Error: login is already taken." << endl;
        return false;
    }
    uint* hashed_pass = sha1(_pass, _pass.size());
    if (hashed_pass == nullptr) {
        cout << "Error: password hashing failed." << endl;
        return false;
    }
    User person(_name, _login, hashed_pass);
    Users.insert({ _login, person });
    cout << "User  " << _name << " successfully registered!" << endl;
    friends->vname.push_back(_login);
    friends->addVertex(friends->vname.size() - 1);
    delete[] hashed_pass;
    return true;
}

void Chat::logoutUser(const string& login) {
    cout << "User " << login << " logged out." << endl;
}

void Chat::sendMessage(const string& senderLogin, const string& message, const string& recipient) {
    if (!recipient.empty()) { // Личное сообщение
        auto it = Users.find(recipient);
        if (it == Users.end()) {
            cout << "Error: user " << recipient << " not found." << endl;
            return;
        }
        privateMessages[senderLogin].emplace_back(senderLogin, message, recipient);
        privateMessages[recipient].emplace_back(senderLogin, message, recipient);
        cout << senderLogin << " (to " << recipient << "): " << message << endl;
    }
    else { // Сообщение всем
        publicMessages.emplace_back(senderLogin, message);
        cout << senderLogin << " (to everyone): " << message << endl;
    }
}

void Chat::listUsers(string login) const {
    friends->findMinDistancesFloyd(login);
}

void Chat::viewMessages(const string& login) const {
    cout << "Chat messages for " << login << ":" << endl;

    // Публичные сообщения
    for (const auto& msg : publicMessages) {
        cout << msg.sender << " (to everyone): " << msg.content << endl;
    }

    // Личные сообщения для данного пользователя
    if (privateMessages.count(login)) {
        for (const auto& msg : privateMessages.at(login)) {
            cout << msg.sender << " (to " << msg.recipient << "): " << msg.content << endl;
        }
    }
}

void Chat::addFriend(const string& user_name) {
    cout << endl << "Who do you want to add as a friend? If you want to cancel press - 0." << endl;
    listUsers(user_name);
    bool found = false;
    string friend_name;

    do {
        cout << "Enter the username of the user you want to add as a friend: ";
        cin >> friend_name;

        if (friend_name == "0") {
            break; // Пользователь отменил добавление
        }

        if (Users.find(friend_name) == Users.end()) {
            cout << "Error: user " << friend_name << " not found." << endl;
            continue; // Пользователь не найден
        }

        if (friend_name == user_name) {
            cout << "You cannot add yourself as a friend." << endl;
            continue; // Нельзя добавить самого себя
        }

        int index_st = -1;
        int index_user = -1;
        for (int i = 0; i < friends->vname.size(); i++) {
            if (friends->vname[i] == friend_name)
                index_st = i;
            if (friends->vname[i] == user_name)
                index_user = i;
        }

        if (index_st == -1 || index_user == -1) {
            cout << "Error: user not found in friends list." << endl;
            continue; // Если пользователь не найден в списке друзей
        }

        if (friends->edgeExists(index_user, index_st)) {
            cout << "You're already friends!" << endl;
            break;
        }

        friends->addEdge(user_name, friend_name);
        cout << "You have successfully added a friend: " << friend_name << endl;
        found = true;

    } while (!found);
}

string Chat::T9() {
    string input;
    cout << "Enter a prefix to autocomplete: " << endl;
    string itog;
    while (true) {
        cout << "> ";
        input = "";
        cin.ignore();
        getline(cin, input);
        vector<string> suggestions = trie->autocomplete(input);
        if (suggestions.empty()) {
            cout << "No suggestions found." << endl;
            cout << "Write your own message to send: ";
            input = "";
            getline(cin, input);
            itog = input;
            trie->insert(input);
            break;
        }
        else {
            cout << "Suggestions: ";
            int i = 1;
            for (const string& suggestion : suggestions) {
                cout << i << " - " << suggestion << " ";
                i++;
            }
            cout << endl;
            cout << "Enter the supplement that suits you, write its number or write 0 and write your own message: ";
            int choice;
            cin >> choice;
            if (choice == 0) {
                cout << "Enter the message you wanted to enter: ";
                string str;
                cin.ignore();
                getline(cin, str);
                trie->insert(str);
                itog = str;
                break;
            }
            else {
                itog = suggestions[choice - 1];
                break;
            }
        }
    }
    return itog;
}

void Chat::insert_lib() {
    trie->insert("Hello");
    trie->insert("How");
    trie->insert("are");
    trie->insert("you");
    trie->insert("Hi");
}
