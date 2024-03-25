#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <utility>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include "ds.h"

using namespace std;
using namespace ds;

namespace tester {
    enum DistributionType {
        uniform, normal
    };

    template<typename ValueType>
    class Tester {
    private:
        vector<ValueType> array;
        int batchID;
        int valueCount;
        int testCount;
        ValueType distributionArgumentOne, distributionArgumentTwo;
        /**
         * distribution = normal
         * distributionArgumentOne = mean
         * distributionArgumentTwo = standard deviation
         */
        /**
         * distribution = uniform
         * distributionArgumentOne = lower bound
         * distributionArgumentTwo = upper bound
         */
        DistributionType distribution;
        string testDirectoryPath;
        string reportDirectoryPath;

        void generateArrayValues() {
            array.clear();

            random_device randomDevice;
            mt19937 mersenneTwister(randomDevice());
            switch (distribution) {
                case uniform: {
                    if (is_same_v<ValueType, int>) {
                        uniform_int_distribution<int> intDistribution(distributionArgumentOne, distributionArgumentTwo);
                        for (int index = 0; index < valueCount; index++)
                            array.emplace_back(intDistribution(mersenneTwister));
                    } else if (is_same_v<ValueType, double>) {
                        uniform_real_distribution<double> realDistribution(distributionArgumentOne,
                                                                           distributionArgumentTwo);
                        for (int index = 0; index < valueCount; index++)
                            array.emplace_back(realDistribution(mersenneTwister));
                    }
                    break;
                }
                case normal: {
                    normal_distribution<double> normalDistribution(distributionArgumentOne, distributionArgumentTwo);
                    for (int index = 0; index < valueCount; index++)
                        array.emplace_back(normalDistribution(mersenneTwister));
                    break;
                }
                default:
                    throw (runtime_error("Invalid distribution type!"));
            }

        }

        void generateTestFile(const string &filePath) {
            ofstream out(filePath + ".csv");
            for (int index = 0; index < valueCount; index++) {
                out << array[index];
                if (index != valueCount - 1)
                    out << ',';
            }
        }

    public:
        Tester(int batchID,
               int testCount,
               int valueCount,
               DistributionType distribution,
               ValueType distributionArgumentOne,
               ValueType distributionArgumentTwo,
               string testDirectoryPath,
               string reportDirectoryPath) :
                batchID(batchID),
                testCount(testCount),
                valueCount(valueCount),
                distribution(distribution),
                distributionArgumentOne(distributionArgumentOne),
                distributionArgumentTwo(distributionArgumentTwo),
                testDirectoryPath(std::move(testDirectoryPath)),
                reportDirectoryPath(std::move(reportDirectoryPath)) {
            if (distribution == uniform && distributionArgumentOne >= distributionArgumentTwo)
                throw (runtime_error("Lower bound must be strictly smaller than upper bound."));
        }

        ~Tester() = default;

        void generateTests() {
            vector<pair<int, vector<int>>> times;
            ofstream batchInfo(reportDirectoryPath + "/" + to_string(batchID) + "-info.csv");
            ofstream batchData(reportDirectoryPath + "/" + to_string(batchID) + "-data.csv");

            batchInfo << "Batch ID, Number of tests, Distribution, ";
            if (distribution == normal)
                batchInfo << "Mean, Standard deviation, ";
            else if (distribution == uniform)
                batchInfo << "Lower bound, Upper bound, ";
            batchInfo << "Tests path" << endl;

            batchInfo << batchID << ", " << testCount << ", ";
            if (distribution == normal)
                batchInfo << "normal, ";
            else if (distribution == uniform)
                batchInfo << "uniform, ";
            batchInfo << distributionArgumentOne << ", " << distributionArgumentTwo << ", tests/" << batchID << endl;

            batchData << "Test number, STL sort" << endl;
            for (int index = 0; index < testCount; index++) {
                generateArrayValues();
                /// Should only be included if tests should be shown
                /// generateTestFile(testDirectoryPath + "/" + to_string(index + 1));

                /// sorting is done here
                auto startTime = std::chrono::high_resolution_clock::now();
                sort(array.begin(), array.end());
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                long long nanosecondsDuration = duration.count();
                batchData << index + 1 << "," << 1.0 * nanosecondsDuration / 1e9 << endl;
            }
        }
    };
}