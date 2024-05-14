#include <thread>
#include <fstream>
#include <iostream>

#include "tester.h"

using namespace std;
using namespace heap;
using namespace tester;

int main()
{
    ifstream in("./configs/testingSchema-single.csv");
    try
    {
        system("mkdir ../reports");
    }
    catch (const exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        system("mkdir ../tests");
    }
    catch (const exception &exception)
    {
        cout << exception.what() << endl;
    }

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
        getline(in, argumentTwo, '\n');
        argumentTwoValue = stod(argumentTwo);

        string testsPath = "./tests/" + to_string(index);
        try
        {
            system(("mkdir " + testsPath).c_str());
        }
        catch (const exception &exception)
        {
            cout << exception.what() << endl;
        }

        if (dataType == "int")
        {
            Distribution<long long> valueDistribution(distributionType, argumentOneValue, argumentTwoValue);
            Tester<long long> batchTester(index, testCountInt, valueCountInt, valueDistribution, operationDistribution, testsPath, "./reports");
            threads.emplace_back(&Tester<long long>::generateTestsSingle, batchTester);
        }
        else if (dataType == "double")
        {
            Distribution<double> valueDistribution(distributionType, argumentOneValue, argumentTwoValue);
            Tester<double> batchTester(index, testCountInt, valueCountInt, valueDistribution, operationDistribution, testsPath, "./reports");
            threads.emplace_back(&Tester<double>::generateTestsSingle, batchTester);
        }
    }
    for (auto &thread : threads)
        if (thread.joinable())
            thread.join();
    return 0;
}