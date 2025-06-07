#ifndef FR_MAP_HPP
#define FR_MAP_HPP
#include "value.hpp"
#include "../c/string.h"

#ifdef __cplusplus
#include <unordered_map>
extern "C" {
#endif
	extern uint32_t fr_mapDataTag;
#ifdef __cplusplus
}
namespace fr4nken {
	/// Data for a map
	struct MapData {
		std::unordered_map<Value, Value> map;
	};

	/// Creates a new map
	fr_Object *newMap(void);
}
#endif
#endif
