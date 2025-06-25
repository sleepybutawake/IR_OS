#ifndef _LRUCACHE_HPP_INCLUDED_
#define	_LRUCACHE_HPP_INCLUDED_

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
                it->second->second = 1;
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
                it->second->second = 1;
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
                a->second->second = 0;
                ++a;
            }
        }

    private:
        std::list<key_value_pair_t> _cache_items_list;
        std::unordered_map<key_t, list_iterator_t> _cache_items_map;
        size_t _max_size;
    };

} // namespace cache

#endif	/* _LRUCACHE_HPP_INCLUDED_ */
