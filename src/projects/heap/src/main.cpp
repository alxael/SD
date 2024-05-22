#include <thread>
#include <fstream>
#include <iostream>

#include "tester.h"

using namespace std;
using namespace heap;
using namespace tester;

int main()
{
    ifstream in("./configs/testingSchema.csv");
    vector<string> createDirectoriesCommands{"mkdir ./results", "mkdir ./results/reports", "mkdir ./results/tests", "mkdir ./results/reports/single", "mkdir ./results/reports/multiple", "mkdir ./results/tests/single", "mkdir ./results/tests/multiple"};
    for (const auto &command : createDirectoriesCommands)
        system(command.c_str());

    unsigned long long numThreads = thread::hardware_concurrency();
    cout << "Available number of threads: " << numThreads << endl;
    system("pwd");

    cout << "Begin?" << endl;
    string beg;
    getline(cin, beg);

    vector<thread> threads;
    for (long long index = 1; !in.eof(); index++)
    {
        string dataType;
        getline(in, dataType, ',');

        string testCount;
        getline(in, testCount, ',');
        long long testCountInt = stoll(testCount);

        string valueCount;
        getline(in, valueCount, ',');
        long long valueCountInt = stoll(valueCount);

        string distribution, argumentOne, argumentTwo;
        DistributionType distributionType;
        double argumentOneValue, argumentTwoValue;

        getline(in, distribution, ',');

        if (distribution == "normal")
            distributionType = normal;
        else if (distribution == "uniform")
            distributionType = uniform;
        getline(in, argumentOne, ',');
        argumentOneValue = stod(argumentOne);
        getline(in, argumentTwo, ',');
        argumentTwoValue = stod(argumentTwo);

        Distribution<double> operationDistribution(distributionType, argumentOneValue, argumentTwoValue);

        getline(in, distribution, ',');
        if (distribution == "normal")
            distributionType = normal;
        else if (distribution == "uniform")
            distributionType = uniform;
        getline(in, argumentOne, ',');
        argumentOneValue = stod(argumentOne);
        getline(in, argumentTwo, ',');
        argumentTwoValue = stod(argumentTwo);

        string heapCount;
        long long heapCountInt;
        getline(in, heapCount, '\n');
        heapCountInt = stoll(heapCount);

        if (dataType == "int")
        {
            Distribution<long long> valueDistribution(distributionType, argumentOneValue, argumentTwoValue);
            Tester<long long> batchTester(index, testCountInt, valueCountInt, valueDistribution, operationDistribution, "./results/tests", "./results/reports", heapCountInt);
            threads.emplace_back(&Tester<long long>::generateTestsSingle, batchTester);
            threads.emplace_back(&Tester<long long>::generateTestsMultiple, batchTester);
        }
        else if (dataType == "double")
        {
            Distribution<double> valueDistribution(distributionType, argumentOneValue, argumentTwoValue);
            Tester<double> batchTester(index, testCountInt, valueCountInt, valueDistribution, operationDistribution, "./results/tests", "./results/reports", heapCountInt);
            threads.emplace_back(&Tester<double>::generateTestsSingle, batchTester);
            threads.emplace_back(&Tester<double>::generateTestsMultiple, batchTester);
        }
    }
    for (auto &thread : threads)
        if (thread.joinable())
            thread.join();
    return 0;
}