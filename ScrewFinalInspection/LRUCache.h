#pragma once

#include "common.h"

#include <list>
#include <unordered_map>
#include <mutex>

/*
LRU»º´æÄ£°å
*/

template <typename T> using LISTT = std::list<T>;

template <typename T, typename Y>
using PAIRTL = std::pair<Y, typename LISTT<T>::iterator>;

template <typename T, typename Y>
using UMAPTP = std::unordered_map<T, PAIRTL<T, Y>>;

template <typename T, typename Y> class LRUCache {
public:
	LRUCache(const size_t &capacity) : capacity_(capacity) {}
	Y get(const T &key) {
		std::lock_guard<std::mutex> lck(cache_mutex_);
		auto it = cache_.find(key);
		if (it == cache_.end()) {
			return Y();
		}
		touch(it);
		return it->second.first;
	}

	bool contains(const T &key) {
		std::lock_guard<std::mutex> lck(cache_mutex_);
		auto it = cache_.find(key);
		if (it == cache_.end()) {
			return false;
		}
		return true;
	}

	void put(const T &key, const Y &value) {
		std::lock_guard<std::mutex> lck(cache_mutex_);
		auto it = cache_.find(key);
		if (it != cache_.end()) {
			touch(it);
		}
		else {
			if (cache_.size() == capacity_) {
				cache_.erase(elements_.back());
				elements_.pop_back();
			}
			elements_.push_front(key);
		}
		cache_[key] = { value, elements_.begin() };
	}

private:
	void touch(typename UMAPTP<T, Y>::iterator it) {
		T key = it->first;
		elements_.erase(it->second.second);
		elements_.push_front(key);
		it->second.second = elements_.begin();
	}
	size_t capacity_;
	std::mutex cache_mutex_;
	LISTT<T> elements_;
	UMAPTP<T, Y> cache_;
};