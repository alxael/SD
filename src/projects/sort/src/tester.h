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
        long long batchID;
        long long valueCount;
        long long testCount;
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
                    if (is_same_v<ValueType, long long>) {
                        uniform_int_distribution<long long> intDistribution(distributionArgumentOne,
                                                                            distributionArgumentTwo);
                        for (long long index = 0; index < valueCount; index++)
                            generatedArray[index] = intDistribution(mersenneTwister);
                    } else if (is_same_v<ValueType, double>) {
                        uniform_real_distribution<double> realDistribution(distributionArgumentOne,
                                                                           distributionArgumentTwo);
                        for (long long index = 0; index < valueCount; index++)
                            generatedArray[index] = realDistribution(mersenneTwister);
                    }
                    break;
                }
                case normal: {
                    normal_distribution<double> normalDistribution(distributionArgumentOne, distributionArgumentTwo);
                    for (long long index = 0; index < valueCount; index++)
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
            out << array;
        }

    public:
        Tester(long long batchID,
               long long testCount,
               long long valueCount,
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
            vector<pair<long long, vector<long long>>> times;
            ofstream batchInfo(reportDirectoryPath + "/" + to_string(batchID) + "-info.csv");
            ofstream batchData(reportDirectoryPath + "/" + to_string(batchID) + "-data.csv");

            batchInfo << "Batch ID, Number of tests, Number of values, Distribution, ";
            if (distribution == normal)
                batchInfo << "Mean, Standard deviation, ";
            else if (distribution == uniform)
                batchInfo << "Lower bound, Upper bound, ";
            batchInfo << "Tests path" << endl;

            batchInfo << batchID << ", " << testCount << ", " << valueCount << ", ";
            if (distribution == normal)
                batchInfo << "normal, ";
            else if (distribution == uniform)
                batchInfo << "uniform, ";
            batchInfo << distributionArgumentOne << ", " << distributionArgumentTwo << ", tests/" << batchID << endl;

            const long long numSortingAlgorithms = 9;
            long long durations[numSortingAlgorithms + 1] = {};
            batchData
                    << "Test number, STL sort, Radix sort base 10, Radix sort base 1<<16, Merge sort, Shell sort, Quick sort, Quick sort double pivot, Heap sort, Intro sort"
                    << endl;
            for (long long index = 0; index < testCount; index++) {
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
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.stlSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[1] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[1] = 0;
                }

                /// 2. Radix sort, base 10
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.radixSort(10);
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[2] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[2] = 0;
                }

                /// 3. Radix sort base 1<<16
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.radixSort((1 << 16));
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[3] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[3] = 0;
                }

                /// 4. Merge sort
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.mergeSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[4] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[4] = 0;
                }

                /// 5. Shell sort
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.shellSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[5] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[5] = 0;
                }

                /// 6. Quick sort
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.quickSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[6] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[6] = 0;
                }

                /// 7. Quick sort double pivot
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.quickSortDoublePivot();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[7] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[7] = 0;
                }

                /// 8. Heap sort
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.heapSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[8] = duration.count();
                } catch (const exception &exception) {
//                    cout << exception.what() << endl;
                    auxDurations[8] = 0;
                }

                /// 9. Intro sort
                try {
                    auxArray = array;
                    startTime = high_resolution_clock::now();
                    auxArray.introSort();
                    if (!auxArray.isSorted())
                        throw (runtime_error("Array is not sorted!!!"));
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[9] = duration.count();
                } catch (const exception &exception) {
                    cout << exception.what() << endl;
                    auxDurations[9] = 0;
                }

                batchData << index + 1 << ",";
                for (long long durationIndex = 1; durationIndex <= numSortingAlgorithms; durationIndex++) {
                    durations[durationIndex] += auxDurations[durationIndex];
                    durations[0] += auxDurations[durationIndex];
                    batchData << 1.0 * auxDurations[durationIndex] / 1e6;
                    if (durationIndex != numSortingAlgorithms)
                        batchData << ",";
                }
                batchData << endl;
            }
            batchInfo
                    << "Total time, STL sort, Radix sort base 10 average, Radix sort base 1<<16 average, Merge sort average, Shell sort average, Quick sort average, Quick sort double pivot averag, Heap sort average, Intro sort average"
                    << endl;
            batchInfo << (1.0 * durations[0] / 1e6) << ",";
            for (long long index = 1; index <= numSortingAlgorithms; index++) {
                batchInfo << (1.0 * durations[index] / 1e6) / testCount;
                if (index != numSortingAlgorithms)
                    batchInfo << ",";
            }
            batchInfo << endl;
        }
    };
}