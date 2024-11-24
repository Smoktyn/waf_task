#pragma once
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>

class DNSCache {

private:
    explicit DNSCache(size_t max_size) : _capacity(max_size) { };

public:
    DNSCache(const DNSCache&) = delete;
    DNSCache(DNSCache&&) = delete;
    DNSCache& operator=(const DNSCache&) = delete;
    DNSCache& operator=(DNSCache&&) = delete;

    static auto& instance(const size_t max_size = 0) {
        static std::unique_ptr<DNSCache> cache{ nullptr };

        if (cache == nullptr) {
            if (max_size == 0) {
                throw std::invalid_argument("max_size must be greater than 0");
            }
            cache.reset(new DNSCache(max_size));
        }

        return *cache;
    }

	void update(const std::string& name, const std::string& ip) {
        std::lock_guard<std::mutex> lock(_mtx);

        if (auto it = _map.find(name); it != _map.end()) {
            _list.splice(_list.begin(), _list, it->second);
            it->second->second = ip;
            return;
        }

        if (_capacity == _map.size()) {
            _map.erase(_list.back().first);
            _list.pop_back();
        }

        _list.push_front(std::make_pair(name, ip));
        _map[name] = _list.begin();
	}

    std::string resolve(const std::string& name) {
        std::lock_guard<std::mutex> lock(_mtx);

        const auto it = _map.find(name);
        if (it != _map.end()) {
            _list.splice(_list.begin(), _list, it->second);
            return it->second->second;
        }

        return std::string{};
    }

private:
    using ListNodeType = std::pair<std::string, std::string>;
    using ListNodeIterator = std::list<std::pair<std::string, std::string>>::iterator;

private:
    inline static std::mutex _mtx{};
	size_t _capacity{ 0 };
	std::unordered_map<std::string, ListNodeIterator> _map{};
	std::list<ListNodeType> _list{};
};