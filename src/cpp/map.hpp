#include "../c/value.h"
#include "../c/string.h"

#ifdef __cplusplus
#include <unordered_map>
extern "C" {
#endif
	extern uint32_t fr_mapDataTag;
#ifdef __cplusplus
}
template <>
struct std::hash<fr_Value> {
	size_t operator()(const fr_Value &k) const {
		return fr_hash(k);	
	}
};
template <>
struct std::equal_to<fr_Value> {
	bool operator()(const fr_Value &a, const fr_Value &b) const {
		return fr_equal(a, b);
	}
};
namespace fr4nken {
	/// Data for a map
	struct MapData {
		std::unordered_map<fr_Value, fr_Value> map;
	};

	/// Creates a new map
	fr_Object *newMap(void);
}
#endif
