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
using namespace chrono;

namespace tester {
    enum DistributionType {
        uniform, normal
    };

    template<typename ValueType>
    class Tester {
    private:
        Vector<ValueType> array;
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
            auto generatedArray = new ValueType[valueCount];

            random_device randomDevice;
            mt19937 mersenneTwister(randomDevice());
            switch (distribution) {
                case uniform: {
                    if (is_same_v<ValueType, int>) {
                        uniform_int_distribution<int> intDistribution(distributionArgumentOne, distributionArgumentTwo);
                        for (int index = 0; index < valueCount; index++)
                            generatedArray[index] = intDistribution(mersenneTwister);
                    } else if (is_same_v<ValueType, double>) {
                        uniform_real_distribution<double> realDistribution(distributionArgumentOne,
                                                                           distributionArgumentTwo);
                        for (int index = 0; index < valueCount; index++)
                            generatedArray[index] = realDistribution(mersenneTwister);
                    }
                    break;
                }
                case normal: {
                    normal_distribution<double> normalDistribution(distributionArgumentOne, distributionArgumentTwo);
                    for (int index = 0; index < valueCount; index++)
                        generatedArray[index] = normalDistribution(mersenneTwister);
                    break;
                }
                default:
                    throw (runtime_error("Invalid distribution type!"));
            }

            array.update(valueCount, generatedArray);
            delete[] generatedArray;
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

            long long durations[10] = {};
            int numSortingAlgorithms = 7;
            batchData
                    << "Test number, STL sort, Radix sort, Merge sort, Shell sort, Quick sort, Heap sort, Intro sort"
                    << endl;
            for (int index = 0; index < testCount; index++) {
                /// Progress evaluation
                cout << "Batch " << batchID << " - Test " << index + 1 << endl;

                /// Should only be included if tests should be shown
                // generateTestFile(testDirectoryPath + "/" + to_string(index + 1));
                long long auxDurations[10] = {}, auxDuration;
                generateArrayValues();
                Vector<ValueType> auxArray;
                time_point<high_resolution_clock> startTime, endTime;
                nanoseconds duration;

                /// 1. STL sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.stlSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[1] = duration.count();

                /// 2. Radix sort, base 10
                // if(is_same_v<int, ValueType>) {

                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.radixSort(10);
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[2] = duration.count();

                /// 3. Radix sort, base 2^16 = 65536
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.radixSort(65536);
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[3] = duration.count();


                /// 4. Merge sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.mergeSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[4] = duration.count();

                /// 5. Shell sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.shellSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[5] = duration.count();

                /// 6. Quick sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.quickSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[6] = duration.count();

                /// 7. Heap sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.heapSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[7] = duration.count();

                /// 8. Intro sort
                auxArray = array;
                startTime = high_resolution_clock::now();
                auxArray.introSort();
                endTime = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(endTime - startTime);
                auxDurations[8] = duration.count();

//                /// 9. Insertion sort
//                auxArray = array;
//                startTime = high_resolution_clock::now();
//                auxArray.insertionSort();
//                endTime = high_resolution_clock::now();
//                duration = duration_cast<nanoseconds>(endTime - startTime);
//                auxDurations[9] = duration.count();


                batchData << index + 1 << ",";
                for (int durationIndex = 1; durationIndex <= numSortingAlgorithms; durationIndex++) {
                    durations[durationIndex] += auxDurations[durationIndex];
                    durations[0] += auxDurations[durationIndex];
                    batchData << 1.0 * auxDurations[durationIndex] / 1e6;
                    if (durationIndex != numSortingAlgorithms)
                        batchData << ",";
                }
                batchData << endl;
            }
            batchInfo
                    << "Total time, STL sort average, Radix sort average, Merge sort average, Shell sort average, Quick sort average, Heap sort average, Intro sort average"
                    << endl;
            batchInfo << (1.0 * durations[0] / 1e6) << ",";
            for (int index = 1; index <= numSortingAlgorithms; index++) {
                batchInfo << (1.0 * durations[index] / 1e6) / testCount;
                if (index != numSortingAlgorithms)
                    batchInfo << ",";
            }
            batchInfo << endl;
        }
    };
}