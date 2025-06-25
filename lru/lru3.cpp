#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace cache {

    template<typename key_t, typename value_t>
    class lru_cache {
    public:
        typedef typename std::pair<key_t, value_t> key_value_pair_t;
        typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

        lru_cache(size_t max_size) :
            _max_size(max_size) {
        }

        void put(const key_t& key, const value_t& value) {
            auto it = _cache_items_map.find(key);
            if (it != _cache_items_map.end()) {
                it->second->second = it->second->second >> 1;
                it->second->second = (it->second->second) ^ (0x80);
            }
            else {
                _cache_items_list.push_front(key_value_pair_t(key, value));
                _cache_items_map[key] = _cache_items_list.begin();
            }

            if (_cache_items_map.size() > _max_size) {
                auto a = _cache_items_list.begin();
                int min = a->second;
                auto pos = a;
                for (int i = 1; i < _cache_items_list.size(); ++i)
                {
                    ++a;
                    if (a->second < min)
                    {
                        min = a->second;
                        pos = a;
                    }
                }
                _cache_items_map.erase(pos->first);
                _cache_items_list.erase(pos);
            }
        }

        const value_t& get(const key_t& key) {
            auto it = _cache_items_map.find(key);
            if (it == _cache_items_map.end()) {
                throw std::range_error("There is no such key in cache");
            }
            else {
                it->second->second = it->second->second >> 1;
                it->second->second = (it->second->second) ^ (0x80);
                return it->second->second;
            }
        }

        bool exists(const key_t& key) const {
            return _cache_items_map.find(key) != _cache_items_map.end();
        }

        size_t size() const {
            return _cache_items_map.size();
        }

        void rightShift()
        {
            auto a = _cache_items_map.begin();
            for (int i = 0; i < size(); ++i)
            {
                a->second->second = a->second->second >> 1;
                ++a;
            }
        }

    private:
        std::list<key_value_pair_t> _cache_items_list;
        std::unordered_map<key_t, list_iterator_t> _cache_items_map;
        size_t _max_size;
    };

}

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
            y = 0x80;
            cache_lru.put(x, y);
            cout << "Put " << x << ',' << y << endl;
            ++numberOfPageFault;
        }
        cache_lru.rightShift();
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
        cout << "²âÊÔ" << i << endl;
        numberOfPageFault = test(records, capacity);
        cout << "Ò³´íÎóÊý£º" << numberOfPageFault << endl;
        cout << "Ò³´íÎóÂÊ£º" << (double)numberOfPageFault / records << endl << endl;
        totalNumberOfPageFault += numberOfPageFault;
    }
    cout << "×ÜÒ³´íÎóÊý£º" << totalNumberOfPageFault << endl;
    cout << "Æ½¾ùÒ³´íÎóÂÊ£º" << (double)totalNumberOfPageFault / (records * numberOfTest) << endl;
    return 0;
}
