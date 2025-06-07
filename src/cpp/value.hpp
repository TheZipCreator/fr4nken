// simple wrapper
#ifndef FR_VALUE_HPP
#define FR_VALUE_HPP

#include "../c/value.h"
#include <unordered_map>


namespace fr4nken {
	struct Value {
		fr_Value val;
		Value();
		Value(fr_Value val, bool acquire = true);
		Value(const Value &other);
		~Value();
		bool operator==(const Value &other) const;
		operator fr_Value();
	};
}

template <>
struct std::hash<fr4nken::Value> {
	size_t operator()(const fr4nken::Value &k) const {
		return fr_hash(k.val);
	}
};
#endif
