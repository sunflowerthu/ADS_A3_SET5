#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <vector>
#include <cstdint>

class HyperLogLog {
public:
    explicit HyperLogLog(uint32_t B);

    void add(uint32_t hash);
    double estimate() const;
    void reset();

private:
    uint32_t B_;
    uint32_t m_;
    std::vector<uint8_t> M_;

    static uint32_t leadingZeros(uint32_t x);
    double alpha() const;
};


#endif //HYPERLOGLOG_H
