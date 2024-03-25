#include <thread>
#include <vector>
#include <iostream>
#include "tester.h"

using namespace std;
using namespace ds;
using namespace tester;

int main() {
    int numThreads = thread::hardware_concurrency();
    int numTests;

    ifstream in("../configs/testingSchema.csv");
    cout << "Available number of threads: " << numThreads << endl;
    
    vector<thread> threads;
    for (int index = 1; index <= numThreads && !in.eof(); index++) {
        string dataType, valueCount, testCount, distribution, argumentOne, argumentTwo;
        DistributionType distributionType;
        int testCountInt, valueCountInt;
        double argumentOneValue, argumentTwoValue;
        getline(in, dataType, ',');
        getline(in, valueCount, ',');
        valueCountInt = stoi(valueCount);
        getline(in, testCount, ',');
        testCountInt = stoi(testCount);
        getline(in, distribution, ',');
        if (distribution == "normal")
            distributionType = normal;
        else if (distribution == "uniform")
            distributionType = uniform;
        getline(in, argumentOne, ',');
        argumentOneValue = stod(argumentOne);
        getline(in, argumentTwo);
        argumentTwoValue = stod(argumentTwo);

        string testsPath = "../tests/" + to_string(index);
        // try {
        //     system(("mkdir " + testsPath).c_str());
        // } catch (const exception &exception) {}

        if (dataType == "int") {
            Tester<int> batchTester(index, testCountInt, valueCountInt, distributionType, argumentOneValue,
                                    argumentTwoValue,
                                    testsPath, "../reports");
            threads.emplace_back(&Tester<int>::generateTests, batchTester);
        } else if (dataType == "double") {
            Tester<double> batchTester(index, testCountInt, valueCountInt, distributionType, argumentOneValue,
                                       argumentTwoValue,
                                       testsPath, "../reports");
            threads.emplace_back(&Tester<double>::generateTests, batchTester);
        }
    }
    for (auto &thread: threads)
        if (thread.joinable())
            thread.join();
    return 0;
}