#include <thread>
#include <vector>
#include <iostream>
#include "tester.h"

using namespace std;
using namespace ds;
using namespace tester;

int main() {
    unsigned int numThreads = thread::hardware_concurrency();
    int numTests;

    ifstream in("../configs/testingSchema.csv");
    cout << "Available number of threads: " << numThreads << endl;
    
    vector<thread> threads;
    for (int index = 1; index <= numThreads && !in.eof(); index++) {
        string dataType;
        getline(in, dataType, ',');

        string valueCount;
        getline(in, valueCount, ',');
        int valueCountInt = stoi(valueCount);

        string testCount;
        getline(in, testCount, ',');
        int testCountInt = stoi(testCount);

        string distribution;
        getline(in, distribution, ',');
        DistributionType distributionType;
        if (distribution == "normal")
            distributionType = normal;
        else if (distribution == "uniform")
            distributionType = uniform;

        string argumentOne;
        getline(in, argumentOne, ',');
        double argumentOneValue = stod(argumentOne);

        string argumentTwo;
        getline(in, argumentTwo);
        double argumentTwoValue = stod(argumentTwo);

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