#include <map>
#include <queue>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <utility>
#include <iostream>
#include <type_traits>
#include <algorithm>

#include "heap.h"

using namespace std;
using namespace heap;
using namespace chrono;

namespace tester
{
    enum DistributionType
    {
        uniform,
        normal
    };

    enum OperationType
    {
        push,
        pop,
        merge,
        getMax,
        decKey,
    };

    template <typename ValueType>
    class Distribution
    {
    private:
        DistributionType type;
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
        ValueType argumentOne, argumentTwo;

    public:
        Distribution(DistributionType type, ValueType argumentOne, ValueType argumentTwo) : type(type), argumentOne(argumentOne), argumentTwo(argumentTwo)
        {
            if (this->type == uniform && argumentOne > argumentTwo)
                throw(runtime_error("Invalid distribution! Lower bound must be smaller than upper bound!"));
        }
        ~Distribution() = default;

        DistributionType getType() { return type; }
        void setType(DistributionType type) { type = type; }

        ValueType getArgumentOne() { return argumentOne; }
        void setArgumentOne(ValueType argumentOne) { argumentOne = argumentOne; }

        ValueType getArgumentTwo() { return argumentTwo; }
        void setArgumentTwo(ValueType argumentTwo) { argumentTwo = argumentTwo; }

        ValueType generateValue()
        {
            random_device randomDevice;
            mt19937 mersenneTwister(randomDevice());
            switch (type)
            {
            case uniform:
            {
                if (is_same<ValueType, long long>::value)
                {
                    uniform_int_distribution<long long> intDistribution(argumentOne, argumentTwo);
                    return intDistribution(mersenneTwister);
                }
                else if (is_same<ValueType, double>::value)
                {
                    uniform_real_distribution<double> realDistribution(argumentOne, argumentTwo);
                    return realDistribution(mersenneTwister);
                }
                break;
            }
            case normal:
            {
                normal_distribution<double> normalDistribution(argumentOne, argumentTwo);
                return normalDistribution(mersenneTwister);
            }
            default:
                throw(runtime_error("Invalid distribution type!"));
            }
        }
        void generateDescription(ostream &out, string description)
        {
            out << description + " distribution type, ";
            if (type == uniform)
                out << description + " distribution lower bound, " << description + " distribution upper bound" << endl;
            if (type == normal)
                out << description + "distribution mean, " << description + "distribution standard deviation" << endl;
            out << (type == normal ? "Normal" : "Uniform") << ", " << argumentOne << ", " << argumentTwo << endl;
        }
    };

    template <typename ValueType>
    class Tester
    {
    private:
        long long batchID;
        long long valueCount;
        long long testCount;
        Distribution<ValueType> valueDistribution;
        Distribution<double> operationDistribution;
        string testDirectoryPath;
        string reportDirectoryPath;

        const map<OperationType, string> operationTypeDisplay{{push, "Push"}, {pop, "Pop"}, {getMax, "Get maximum"}, {decKey, "Decrease key"}};
        static const long long heapImplementationsCount = 4;

        vector<pair<OperationType, ValueType>> generateDataSingle()
        {
            vector<pair<OperationType, ValueType>> result;
            for (int index = 0; index < valueCount; index++)
            {
                double operationValue = operationDistribution.generateValue();
                OperationType type;
                if (operationValue < 1)
                    result.emplace_back(pair<OperationType, ValueType>(pop, 0));
                if (operationValue > 1 && operationValue < 2)
                    result.emplace_back(pair<OperationType, ValueType>(push, valueDistribution.generateValue()));
                if (operationValue > 2)
                    result.emplace_back(pair<OperationType, ValueType>(getMax, 0));
            }
            return result;
        }
        void generateTestFileSingle(const string &filePath, vector<pair<OperationType, ValueType>> data, vector<string> results[])
        {
            ofstream out(filePath + "-single.csv");
            out << "Type, Value, STL Heap result, Leftist Heap result, Pairing Heap result, Fibonacci Heap result" << endl;
            for (long long index = 0; index < data.size(); index++)
            {
                auto itd = operationTypeDisplay.find(data[index].first);
                out << itd->second << ", " << data[index].second << ", ";
                for (long long heapIndex = 1; heapIndex <= heapImplementationsCount; heapIndex++)
                    if (index < results[heapIndex].size())
                        out << results[heapIndex][index] << ", ";
                    else
                        out << " , ";
                out << endl;
            }
        }

    public:
        Tester(long long batchID,
               long long testCount,
               long long valueCount,
               Distribution<ValueType> valueDistribution,
               Distribution<double> operationDistribution,
               string testDirectoryPath,
               string reportDirectoryPath) : batchID(batchID),
                                             testCount(testCount),
                                             valueCount(valueCount),
                                             valueDistribution(valueDistribution),
                                             operationDistribution(operationDistribution),
                                             testDirectoryPath(std::move(testDirectoryPath)),
                                             reportDirectoryPath(std::move(reportDirectoryPath))
        {
        }

        ~Tester() = default;

        void generateTestsSingle()
        {
            vector<pair<long long, vector<long long>>> times;
            ofstream batchInfo(reportDirectoryPath + "/" + to_string(batchID) + "-single-info.csv");
            ofstream batchData(reportDirectoryPath + "/" + to_string(batchID) + "-single-data.csv");

            batchInfo << "Batch ID, Number of tests, Number of values, Tests path" << endl;
            batchInfo << batchID << ", " << testCount << ", " << valueCount << ", tests/" << batchID << endl;
            batchInfo << endl;

            valueDistribution.generateDescription(batchInfo, "Value");
            batchInfo << endl;

            operationDistribution.generateDescription(batchInfo, "Operation");
            batchInfo << endl;

            long long durations[heapImplementationsCount + 1] = {};

            batchData << "Test number, STL Heap, Leftist Heap, Pairing Heap, Fibonacci Heap" << endl;
            for (long long index = 0; index < testCount; index++)
            {
                /// Progress evaluation
                cout << "Batch " << batchID << " - Test " << index + 1 << endl;

                vector<pair<OperationType, ValueType>> data = this->generateDataSingle();

                long long auxDurations[heapImplementationsCount + 1] = {};
                vector<string> results[heapImplementationsCount + 1];
                vector<ValueType> auxArray;
                time_point<high_resolution_clock> startTime, endTime;
                nanoseconds duration;

                /// 1. STL Heap
                try
                {
                    priority_queue<ValueType> heap;
                    startTime = high_resolution_clock::now();
                    for (auto it = data.begin(); it != data.end(); it++)
                    {
                        if (it->first == push)
                        {
                            try
                            {
                                heap.push(it->second);
                                results[1].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[1].emplace_back("Push failed");
                            }
                        }
                        if (it->first == pop)
                        {
                            if (!heap.empty())
                            {
                                heap.pop();
                                results[1].emplace_back("Pop succeded");
                            }
                            else
                                results[1].emplace_back("Pop failed");
                        }
                        if (it->first == getMax)
                        {
                            if (!heap.empty())
                            {
                                ValueType top = heap.top();
                                results[1].emplace_back(to_string(top));
                            }
                            else
                                results[1].emplace_back("Get maximum failed");
                        }
                    }
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[1] = duration.count();
                }
                catch (const exception &exception)
                {
                    cout << exception.what() << endl;
                    auxDurations[1] = 0;
                }

                // 2. Leftist Heap
                try
                {
                    LeftistHeap<ValueType> heap;

                    startTime = high_resolution_clock::now();
                    for (auto it = data.begin(); it != data.end(); it++)
                    {
                        if (it->first == push)
                        {
                            try
                            {
                                heap.push(it->second);
                                results[2].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Push failed");
                            }
                        }
                        if (it->first == pop)
                        {
                            try
                            {
                                heap.pop();
                                results[2].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Pop failed");
                            }
                        }
                        if (it->first == getMax)
                        {
                            try
                            {
                                ValueType top = heap.top();
                                results[2].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Get maximum failed");
                            }
                        }
                    }
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[2] = duration.count();
                }
                catch (const exception &exception)
                {
                    cout << exception.what() << endl;
                    auxDurations[2] = 0;
                }

                // 3. Pairing Heap
                try
                {
                    PairingHeap<ValueType> heap;
                    startTime = high_resolution_clock::now();
                    for (auto it = data.begin(); it != data.end(); it++)
                    {
                        if (it->first == push)
                        {
                            try
                            {
                                heap.push(it->second);
                                results[3].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Push failed");
                            }
                        }
                        if (it->first == pop)
                        {
                            try
                            {
                                heap.pop();
                                results[3].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Pop failed");
                            }
                        }
                        if (it->first == getMax)
                        {
                            try
                            {
                                ValueType top = heap.top();
                                results[3].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Get maximum failed");
                            }
                        }
                    }
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[3] = duration.count();
                }
                catch (const exception &exception)
                {
                    cout << exception.what() << endl;
                    auxDurations[3] = 0;
                }

                // 4. Fibonacci Heap
                try
                {
                    FibonacciHeap<ValueType> heap;
                    startTime = high_resolution_clock::now();
                    for (auto it = data.begin(); it != data.end(); it++)
                    {
                        if (it->first == push)
                        {
                            try
                            {
                                heap.push(it->second);
                                results[4].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Push failed");
                            }
                        }
                        if (it->first == pop)
                        {
                            try
                            {
                                ValueType top = heap.pop();
                                results[4].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Pop failed");
                            }
                        }
                        if (it->first == getMax)
                        {
                            try
                            {
                                ValueType top = heap.top();
                                results[4].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Get maximum failed");
                            }
                        }
                    }
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<nanoseconds>(endTime - startTime);
                    auxDurations[4] = duration.count();
                }
                catch (const exception &exception)
                {
                    cout << exception.what() << endl;
                    auxDurations[4] = 0;
                }

                long long valid[heapImplementationsCount + 1] = {};
                for (long long heapIndex = 2; heapIndex <= heapImplementationsCount; heapIndex++)
                    for (long long index = 0; index < valueCount; index++)
                        if (results[heapIndex][index] != results[1][index])
                        {
                            valid[heapIndex] = 1;
                            break;
                        }

                batchData << index + 1 << ",";
                for (long long durationIndex = 1; durationIndex <= heapImplementationsCount; durationIndex++)
                {
                    durations[durationIndex] += auxDurations[durationIndex];
                    durations[0] += auxDurations[durationIndex];
                    if (valid[durationIndex] == 1)
                        batchData << "INVALID";
                    else
                        batchData << 1.0 * auxDurations[durationIndex] / 1e6;
                    if (durationIndex != heapImplementationsCount)
                        batchData << ",";
                }
                batchData << endl;

                generateTestFileSingle(testDirectoryPath + "/" + to_string(index + 1), data, results);
            }

            batchInfo
                << "Total time, STL Heap average, Leftist Heap average, Pairing Heap average, Fibonacci Heap average"
                << endl;
            batchInfo << (1.0 * durations[0] / 1e6) << ",";
            for (long long index = 1; index <= heapImplementationsCount; index++)
            {
                batchInfo << (1.0 * durations[index] / 1e6) / testCount;
                if (index != heapImplementationsCount)
                    batchInfo << ",";
            }
            batchInfo << endl;
        }
    };
}