#include "value.hpp"

namespace fr4nken {
	Value::Value() {
		val = FR_UNDEFINED;
	}
	Value::Value(fr_Value val, bool acquire) {
		this->val = val;
		if(acquire)
			fr_acquireV(val);
	}
	Value::Value(const Value &other) {
		val = other.val;
		fr_acquireV(other.val);
	}
	Value::~Value() {
		fr_releaseV(val);
	}
	bool Value::operator==(const Value &other) const {
		return fr_equal(val, other.val);
	}
	Value::operator fr_Value() {
		return val;
	}
}
