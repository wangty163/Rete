#pragma once

#include <memory>
#include <vector>

#define DEFINE_STD_HASH_SPECIALIZATION(hashable)                               \
namespace std {                                                                \
    template<>                                                                 \
    struct hash<hashable> {                                                    \
        size_t operator()(const hashable& object) const {                 \
            return object.hashCode();                                          \
        }                                                                      \
    };                                                                         \
};

namespace std {
	template<typename T>
	struct hash<std::vector<T>> {
		size_t operator()(const std::vector<T>& vec) const {
			size_t ret = 2166136261;
			for (auto&& v : vec)
				ret = (ret * 16777619) ^ hash<T>()(v);
			return ret;
		}
	};
};