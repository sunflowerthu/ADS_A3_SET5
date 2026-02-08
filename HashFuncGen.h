#ifndef HASHFUNCGEN_H
#define HASHFUNCGEN_H
#include <cstdint>
#include <string>

class HashFuncGen {
public:
    static uint32_t hash(const std::string& s);
};

#endif //HASHFUNCGEN_H
