#include <fstream>
#include <iostream>

#include "heap.h"
#include "tester.h"

using namespace std;
using namespace heap;

int main()
{
    ifstream in("tests/1.txt");
    LeftistNode<int>::LeftistHeap leftistHeap;
    int value, numberOfValues;

    in >> numberOfValues;
    for (int index = 1; index <= numberOfValues; index++)
    {
        in >> value;
        leftistHeap.push(value);
    }
    cout << leftistHeap;

    while (!leftistHeap.empty())
    {
        cout << leftistHeap.top() << endl;
        leftistHeap.pop();
    }
    return 0;
}