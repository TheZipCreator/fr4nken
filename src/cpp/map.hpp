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
struct std::hash<fr_String *> {
	size_t operator()(const fr_String *&k) const {
		return fr_hashString(k);	
	}
};
template <>
struct std::equal_to<fr_String *> {
	bool operator()(const fr_String *&a, const fr_String *&b) {
		if(a->len != b->len)
			return false;
		return memcmp(a->data, b->data, a->len) == 0;
	}
};
namespace fr4nken {
	/// Data for a map
	struct MapData {
		std::unordered_map<fr_String *, fr_Value> map;
	};

	/// Creates a new map
	fr_Object *newMap(void);
}
#endif
