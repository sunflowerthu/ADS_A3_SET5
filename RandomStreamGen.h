#ifndef RANDOMSTREAMGEN_H
#define RANDOMSTREAMGEN_H
#include <cstdint>
#include <random>
#include <string>
#include <vector>

class RandomStreamGen {
public:
    RandomStreamGen(size_t streamSize, uint32_t seed = 42);

    const std::vector<std::string>& getStream() const;
    std::vector<std::string> getPrefix(double percent) const;
    std::vector<std::string> getPrefixByCount(size_t count) const;

private:
    size_t streamSize_;
    std::vector<std::string> stream_;
    std::mt19937 rng_;

    std::string generateRandomString();
};


#endif //RANDOMSTREAMGEN_H
