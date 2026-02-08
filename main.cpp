#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cmath>

#include "HashFuncGen.h"
#include "HyperLogLog.h"
#include "RandomStreamGen.h"

size_t exactDistinctCount(const std::vector<std::string>& data) {
    std::unordered_set<std::string> s(data.begin(), data.end());
    return s.size();
}

double mean(const std::vector<double>& v) {
    double sum = 0.0;
    for (double x : v) sum += x;
    return sum / v.size();
}

double stddev(const std::vector<double>& v, double mu) {
    double sum = 0.0;
    for (double x : v)
        sum += (x - mu) * (x - mu);
    return std::sqrt(sum / v.size());
}

/* параметры эксперимента */
const int STREAMS = 30;
const int STEPS = 10;

const std::vector<int> STREAM_SIZES = {100000, 300000, 600000};
const std::vector<int> B_VALUES = {6, 8, 10, 12};

int main() {
    std::ofstream out("hll_results.csv");
    out << "stream_size,B,step,processed_size,exact,mean_estimate,stddev\n";

    for (int streamSize : STREAM_SIZES) {
        for (int B : B_VALUES) {

            std::vector<std::vector<double>> estimates(STEPS);
            std::vector<size_t> exact(STEPS);
            std::vector<size_t> processedSize(STEPS);

            for (int s = 0; s < STREAMS; ++s) {
                RandomStreamGen gen(streamSize, 1234 + s);

                for (int t = 1; t <= STEPS; ++t) {
                    double percent = t * 100.0 / STEPS;
                    auto prefix = gen.getPrefix(percent);

                    HyperLogLog hll(B);
                    for (const auto& str : prefix) {
                        hll.add(HashFuncGen::hash(str));
                    }

                    double est = hll.estimate();
                    estimates[t - 1].push_back(est);

                    if (s == 0) {
                        exact[t - 1] = exactDistinctCount(prefix);
                        processedSize[t - 1] = prefix.size();
                    }
                }
            }

            for (int t = 0; t < STEPS; ++t) {
                double mu = mean(estimates[t]);
                double sigma = stddev(estimates[t], mu);

                out << streamSize << ","
                    << B << ","
                    << (t + 1) << ","
                    << processedSize[t] << ","
                    << exact[t] << ","
                    << mu << ","
                    << sigma << "\n";
            }

            std::cout << "Done: N=" << streamSize << ", B=" << B << "\n";
        }
    }

    out.close();
    std::cout << "Results saved to hll_results.csv\n";
    return 0;
}