#include "HashFuncGen.h"
#include <cstdint>
#include <string>

uint32_t HashFuncGen::hash(const std::string &s) {
    const uint32_t FNV_OFFSET = 2166136261u;
    const uint32_t FNV_PRIME = 16777619u;

    uint32_t h = FNV_OFFSET;
    for (unsigned char c: s) {
        h ^= c;
        h *= FNV_PRIME;
    }
    return h;
}
