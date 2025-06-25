#include "lru4.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int test(int records, int capacity)
{
    int numberOfPageFault = 0;
    cache::lru_cache<int, int> cache_lru(capacity);
    int x, y;
    for (int i = 0; i < records; ++i)
    {
        cout << "Test " << i << ": ";
        x = rand() % 10;
        if (cache_lru.exists(x))
            cout << "Get " << x << ',' << cache_lru.get(x) << endl;
        else
        {
            y = 1;
            cache_lru.put(x, y);
            cout << "Put " << x << ',' << y << endl;
            ++numberOfPageFault;
        }
    }
    return numberOfPageFault;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    int records = atoi(argv[1]);
    int capacity = atoi(argv[2]);
    int numberOfTest = atoi(argv[3]);
    int numberOfPageFault;
    int totalNumberOfPageFault = 0;
    for (int i = 0; i < numberOfTest; ++i)
    {
        cout << "����" << i << endl;
        numberOfPageFault = test(records, capacity);
        cout << "ҳ��������" << numberOfPageFault << endl;
        cout << "ҳ�����ʣ�" << (double)numberOfPageFault / records << endl << endl;
        totalNumberOfPageFault += numberOfPageFault;
    }
    cout << "��ҳ��������" << totalNumberOfPageFault << endl;
    cout << "ƽ��ҳ�����ʣ�" << (double)totalNumberOfPageFault / (records * numberOfTest) << endl;
    return 0;
}