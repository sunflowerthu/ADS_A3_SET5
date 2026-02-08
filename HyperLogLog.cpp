#include "HyperLogLog.h"

#include <valarray>

HyperLogLog::HyperLogLog(uint32_t B)
    : B_(B), m_(1u << B), M_(m_, 0) {}

void HyperLogLog::reset() {
    std::fill(M_.begin(), M_.end(), 0);
}

void HyperLogLog::add(uint32_t h) {
    uint32_t index = h >> (32 - B_);      // первые B бит
    uint32_t w = (h << B_);               // оставшиеся биты
    uint32_t rho = leadingZeros(w) + 1;

    M_[index] = std::max(M_[index], static_cast<uint8_t>(rho));
}

double HyperLogLog::estimate() const {
    double sum = 0.0;
    for (uint8_t v : M_) {
        sum += std::pow(2.0, -static_cast<int>(v));
    }
    return alpha() * m_ * m_ / sum;
}

double HyperLogLog::alpha() const {
    if (m_ == 16) return 0.673;
    if (m_ == 32) return 0.697;
    if (m_ == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / m_);
}

uint32_t HyperLogLog::leadingZeros(uint32_t x) {
    if (x == 0) return 32;
#if defined(__GNUG__)
    return __builtin_clz(x);
#else
    uint32_t cnt = 0;
    while ((x & 0x80000000) == 0) {
        cnt++;
        x <<= 1;
    }
    return cnt;
#endif
}