#pragma once
#include <string>

using std::string;

class MovieException {
    private:
    string msg;

    public:
    explicit MovieException(const string& _msg = "") : msg{ _msg } {}
    const char* what() const { return msg.c_str(); }
};

class RepoException {
    private:
    string msg;

    public:
    explicit RepoException(const string& _msg = "") : msg{ _msg } {}
    const char* what() const noexcept { return msg.c_str(); }
};
