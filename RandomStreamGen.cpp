#include "RandomStreamGen.h"
#include <vector>
#include <string>
#include <random>
#include <algorithm>

RandomStreamGen::RandomStreamGen(size_t streamSize, uint32_t seed)
    : streamSize_(streamSize), rng_(seed) {
    stream_.reserve(streamSize_);
    for (size_t i = 0; i < streamSize_; ++i) {
        stream_.push_back(generateRandomString());
    }
}

const std::vector<std::string> &RandomStreamGen::getStream() const {
    return stream_;
}

// Момент времени t как процент потока
std::vector<std::string> RandomStreamGen::getPrefix(double percent) const {
    percent = std::clamp(percent, 0.0, 100.0);
    size_t count = static_cast<size_t>(streamSize_ * percent / 100.0);
    return getPrefixByCount(count);
}

// Момент времени t как количество элементов
std::vector<std::string> RandomStreamGen::getPrefixByCount(size_t count) const {
    count = std::min(count, streamSize_);
    return std::vector<std::string>(stream_.begin(), stream_.begin() + count);
}

std::string RandomStreamGen::generateRandomString() {
    static const std::string alphabet =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789-";

    std::uniform_int_distribution<int> lenDist(1, 30);
    std::uniform_int_distribution<int> charDist(0, alphabet.size() - 1);

    int len = lenDist(rng_);
    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; ++i) {
        s.push_back(alphabet[charDist(rng_)]);
    }
    return s;
}
