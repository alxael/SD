#include <map>
#include <array>
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
        long long heapCount;
        Distribution<ValueType> valueDistribution;
        Distribution<double> operationDistribution;
        string testDirectoryPath;
        string reportDirectoryPath;

        const map<OperationType, string> operationTypeDisplay{{push, "Push"}, {pop, "Pop"}, {getMax, "Get maximum"}, {decKey, "Decrease key"}, {merge, "Merge"}};
        inline static const long long heapImplementationsCount = 4;

        inline static const bool generateSingleTestFiles = false;
        inline static const bool generateMultipleTestFiles = true;

        vector<pair<OperationType, ValueType>> generateDataSingle()
        {
            vector<pair<OperationType, ValueType>> result;
            for (int index = 0; index < valueCount; index++)
            {
                double operationValue = operationDistribution.generateValue();
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
            ofstream out(filePath + ".csv");
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

        vector<tuple<OperationType, long long, ValueType>> generateDataMultiple()
        {
            vector<tuple<OperationType, long long, ValueType>> result;
            Distribution<long long> heapDistribution(uniform, 0, heapCount - 1);
            for (int index = 0; index < valueCount; index++)
            {
                double operationValue = operationDistribution.generateValue();
                OperationType type;
                if (operationValue < 0.75)
                    result.emplace_back(tuple<OperationType, long long, ValueType>(pop, heapDistribution.generateValue(), 0));
                if (operationValue > 0.75 && operationValue < 1.5)
                    result.emplace_back(tuple<OperationType, long long, ValueType>(push, heapDistribution.generateValue(), valueDistribution.generateValue()));
                if (operationValue > 1.5 && operationValue < 2.25)
                    result.emplace_back(tuple<OperationType, long long, ValueType>(getMax, heapDistribution.generateValue(), 0));
                if (operationValue > 2.25)
                    result.emplace_back(tuple<OperationType, long long, ValueType>(merge, heapDistribution.generateValue(), heapDistribution.generateValue()));
            }
            return result;
        }
        void generateTestFileMultiple(const string &filePath, vector<tuple<OperationType, long long, ValueType>> data, vector<string> results[])
        {
            ofstream out(filePath + ".csv");
            out << "Type, Heap index, Value, STL Heap result, Leftist Heap result, Pairing Heap result, Fibonacci Heap result" << endl;
            for (long long index = 0; index < data.size(); index++)
            {
                auto itd = operationTypeDisplay.find(get<0>(data[index]));
                out << itd->second << ", " << get<1>(data[index]) << ", " << get<2>(data[index]) << ", ";
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
               string reportDirectoryPath,
               long long heapCount) : batchID(batchID),
                                      testCount(testCount),
                                      valueCount(valueCount),
                                      valueDistribution(valueDistribution),
                                      operationDistribution(operationDistribution),
                                      testDirectoryPath(std::move(testDirectoryPath)),
                                      reportDirectoryPath(std::move(reportDirectoryPath)),
                                      heapCount(heapCount)
        {
        }

        ~Tester() = default;

        void generateTestsSingle()
        {
            vector<pair<long long, vector<long long>>> times;
            ofstream batchInfo(reportDirectoryPath + "/single/" + to_string(batchID) + "-info.csv");
            ofstream batchData(reportDirectoryPath + "/single/" + to_string(batchID) + "-data.csv");

            const string testSingleDirectoryPath = testDirectoryPath + "/single/" + to_string(batchID);
            if (generateSingleTestFiles)
                system(("mkdir " + testSingleDirectoryPath).c_str());

            batchInfo << "Batch ID, Number of tests, Number of values, Tests path" << endl;
            batchInfo << batchID << ", " << testCount << ", " << valueCount << ", " << testDirectoryPath << endl;
            batchInfo << endl;

            valueDistribution.generateDescription(batchInfo, "Value");
            batchInfo << endl;

            operationDistribution.generateDescription(batchInfo, "Operation");
            batchInfo << endl;

            long long durations[heapImplementationsCount + 1] = {};
            long long operationCount[heapImplementationsCount + 1] = {};

            batchData << "Test number, Push count, Pop count, Get maximum count, STL Heap, Leftist Heap, Pairing Heap, Fibonacci Heap" << endl;
            for (long long index = 0; index < testCount; index++)
            {
                /// Progress evaluation
                cout << "Batch " << batchID << " Single - Test " << index + 1 << endl;

                map<OperationType, long long> operationCount{{push, 0}, {pop, 0}, {getMax, 0}};
                vector<pair<OperationType, ValueType>> data = this->generateDataSingle();
                for (const auto &operation : data)
                    operationCount[operation.first]++;

                long long auxDurations[heapImplementationsCount + 1] = {};
                vector<string> results[heapImplementationsCount + 1];
                vector<ValueType> auxArray;
                time_point<high_resolution_clock> startTime, endTime;
                nanoseconds duration, operationDuration;

                /// 1. STL Heap
                try
                {
                    priority_queue<ValueType> heap;
                    startTime = high_resolution_clock::now();
                    for (const auto &operation : data)
                    {
                        switch (operation.first)
                        {
                        case push:
                            try
                            {
                                heap.push(operation.second);
                                results[1].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[1].emplace_back("Push failed");
                            }
                            break;
                        case pop:
                            if (!heap.empty())
                            {
                                heap.pop();
                                results[1].emplace_back("Pop succeded");
                            }
                            else
                                results[1].emplace_back("Pop failed");
                            break;
                        case getMax:
                            if (!heap.empty())
                            {
                                ValueType top = heap.top();
                                results[1].emplace_back(to_string(top));
                            }
                            else
                                results[1].emplace_back("Get maximum failed");
                            break;
                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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
                    for (const auto &operation : data)
                    {
                        switch (operation.first)
                        {
                        case push:
                            try
                            {
                                heap.push(operation.second);
                                results[2].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                heap.pop();
                                results[2].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap.top();
                                results[2].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Get maximum failed");
                            }
                            break;

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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
                    for (const auto &operation : data)
                    {
                        switch (operation.first)
                        {
                        case push:
                            try
                            {
                                heap.push(operation.second);
                                results[3].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                heap.pop();
                                results[3].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap.top();
                                results[3].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Get maximum failed");
                            }
                            break;

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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
                    for (const auto &operation : data)
                    {
                        switch (operation.first)
                        {
                        case push:
                            try
                            {
                                heap.push(operation.second);
                                results[4].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                ValueType top = heap.pop();
                                results[4].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap.top();
                                results[4].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Get maximum failed");
                            }
                            break;

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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

                batchData << index + 1 << "," << operationCount[push] << "," << operationCount[pop] << "," << operationCount[getMax] << ",";
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

                if (generateSingleTestFiles)
                    generateTestFileSingle(testSingleDirectoryPath + "/" + to_string(index + 1), data, results);
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

        void generateTestsMultiple()
        {
            vector<pair<long long, vector<long long>>> times;
            ofstream batchInfo(reportDirectoryPath + "/multiple/" + to_string(batchID) + "-info.csv");
            ofstream batchData(reportDirectoryPath + "/multiple/" + to_string(batchID) + "-data.csv");

            const string testMultipleDirectoryPath = testDirectoryPath + "/multiple/" + to_string(batchID);
            if (generateMultipleTestFiles)
                system(("mkdir " + testMultipleDirectoryPath).c_str());

            batchInfo << "Batch ID, Number of tests, Number of values, Heap count, Tests path" << endl;
            batchInfo << batchID << ", " << testCount << ", " << valueCount << ", " << heapCount << ", " << testDirectoryPath << endl;
            batchInfo << endl;

            valueDistribution.generateDescription(batchInfo, "Value");
            batchInfo << endl;

            operationDistribution.generateDescription(batchInfo, "Operation");
            batchInfo << endl;

            long long durations[heapImplementationsCount + 1] = {};
            long long operationCount[heapImplementationsCount + 1] = {};

            batchData << "Test number, Push count, Pop count, Get maximum count, Merge count, STL Heap, Leftist Heap, Pairing Heap, Fibonacci Heap" << endl;
            for (long long index = 0; index < testCount; index++)
            {
                /// Progress evaluation
                cout << "Batch " << batchID << " Multiple - Test " << index + 1 << endl;

                map<OperationType, long long> operationCount{{push, 0}, {pop, 0}, {getMax, 0}, {merge, 0}};
                vector<tuple<OperationType, long long, ValueType>> data = this->generateDataMultiple();
                for (const auto &operation : data)
                    operationCount[get<0>(operation)]++;

                long long auxDurations[heapImplementationsCount + 1] = {};
                vector<string> results[heapImplementationsCount + 1];
                vector<ValueType> auxArray;
                time_point<high_resolution_clock> startTime, endTime;
                nanoseconds duration, operationDuration;

                // 1. STL Heap
                try
                {
                    vector<priority_queue<ValueType>> heap(heapCount);
                    startTime = high_resolution_clock::now();
                    for (const auto &operation : data)
                    {
                        auto operationType = get<0>(operation);
                        auto heapIndex = get<1>(operation);
                        switch (operationType)
                        {
                        case push:
                            try
                            {
                                heap[heapIndex].push(get<2>(operation));
                                results[1].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[1].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            if (!heap[heapIndex].empty())
                            {
                                heap[heapIndex].pop();
                                results[1].emplace_back("Pop succeded");
                            }
                            else
                                results[1].emplace_back("Pop failed");
                            break;

                        case getMax:
                            if (!heap[heapIndex].empty())
                            {
                                ValueType top = heap[heapIndex].top();
                                results[1].emplace_back(to_string(top));
                            }
                            else
                                results[1].emplace_back("Get maximum failed");
                            break;

                        case merge:
                        {
                            long long targetHeapIndex = get<2>(operation);
                            if (heapIndex == targetHeapIndex)
                            {
                                results[1].emplace_back("Merge failed");
                                break;
                            }
                            while (!heap[targetHeapIndex].empty())
                            {
                                auto top = heap[targetHeapIndex].top();
                                heap[targetHeapIndex].pop();
                                heap[heapIndex].push(top);
                            }
                            results[1].emplace_back("Merge succeded");
                            break;
                        }
                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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
                    vector<LeftistHeap<ValueType>> heap(heapCount);
                    startTime = high_resolution_clock::now();
                    for (const auto &operation : data)
                    {
                        auto operationType = get<0>(operation);
                        auto heapIndex = get<1>(operation);
                        switch (operationType)
                        {
                        case push:
                            try
                            {
                                heap[heapIndex].push(get<2>(operation));
                                results[2].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                heap[heapIndex].pop();
                                results[2].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap[heapIndex].top();
                                results[2].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[2].emplace_back("Get maximum failed");
                            }
                            break;

                        case merge:
                        {
                            long long targetHeapIndex = get<2>(operation);
                            if (heapIndex == targetHeapIndex)
                            {
                                results[2].emplace_back("Merge failed");
                                break;
                            }
                            heap[heapIndex].merge(heap[targetHeapIndex]);
                            heap[targetHeapIndex] = LeftistHeap<ValueType>();
                            results[2].emplace_back("Merge succeded");
                            break;
                        }

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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
                    vector<PairingHeap<ValueType>> heap(heapCount);
                    startTime = high_resolution_clock::now();
                    for (const auto &operation : data)
                    {
                        auto operationType = get<0>(operation);
                        auto heapIndex = get<1>(operation);
                        switch (operationType)
                        {
                        case push:
                            try
                            {
                                heap[heapIndex].push(get<2>(operation));
                                results[3].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                heap[heapIndex].pop();
                                results[3].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap[heapIndex].top();
                                results[3].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[3].emplace_back("Get maximum failed");
                            }
                            break;

                        case merge:
                        {
                            long long targetHeapIndex = get<2>(operation);
                            if (heapIndex == targetHeapIndex)
                            {
                                results[3].emplace_back("Merge failed");
                                break;
                            }
                            heap[heapIndex].merge(heap[targetHeapIndex]);
                            heap[targetHeapIndex] = PairingHeap<ValueType>();
                            results[3].emplace_back("Merge succeded");
                            break;
                        }

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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

                // 4. Fibbonacci Heap
                try
                {
                    vector<FibonacciHeap<ValueType>> heap(heapCount);
                    startTime = high_resolution_clock::now();
                    for (const auto &operation : data)
                    {
                        auto operationType = get<0>(operation);
                        auto heapIndex = get<1>(operation);
                        switch (operationType)
                        {
                        case push:
                            try
                            {
                                heap[heapIndex].push(get<2>(operation));
                                results[4].emplace_back("Push succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Push failed");
                            }
                            break;

                        case pop:
                            try
                            {
                                ValueType top = heap[heapIndex].pop();
                                results[4].emplace_back("Pop succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Pop failed");
                            }
                            break;

                        case getMax:
                            try
                            {
                                ValueType top = heap[heapIndex].top();
                                results[4].emplace_back(to_string(top));
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Get maximum failed");
                            }
                            break;

                        case merge:
                            try
                            {
                                long long targetHeapIndex = get<2>(operation);
                                if (heapIndex == targetHeapIndex)
                                {
                                    results[4].emplace_back("Merge failed");
                                    break;
                                }
                                heap[heapIndex].merge(&heap[targetHeapIndex]);
                                heap[targetHeapIndex] = FibonacciHeap<ValueType>();
                                results[4].emplace_back("Merge succeded");
                            }
                            catch (...)
                            {
                                results[4].emplace_back("Merge failed");
                            }
                            break;

                        default:
                            throw(runtime_error("Unsupported operation type!!!"));
                            break;
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

                batchData << index + 1 << "," << operationCount[push] << "," << operationCount[pop] << "," << operationCount[getMax] << "," << operationCount[merge] << ",";
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

                if (generateMultipleTestFiles)
                    generateTestFileMultiple(testMultipleDirectoryPath + "/" + to_string(index + 1), data, results);
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