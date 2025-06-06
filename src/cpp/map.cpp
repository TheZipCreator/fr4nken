#include "map.hpp"
#include "../c/objectutils.h"
#include "../c/errorhandling.h"
	
uint32_t fr_mapDataTag;

namespace fr4nken {
	static void destroyMap(uint32_t tag, void *data) {
		delete (MapData *)data;
	}
	
	FR_METHOD(fr_Map_get) {
		FR_NARGS_EQUALS(1);
		FR_GET_STRING(str, 0);
		FR_GET_DATA(MapData);
		FR_ASSERT_MSG(data->map.contains(str), FR_UNDEFINED, "Attempt to get non-existent key in map '%.*s'", str->data, (int)str->len);
		return data->map[str];
	}
	FR_METHOD(fr_Map_put) {
		FR_NARGS_EQUALS(2);
		FR_GET_STRING(str, 0);
		FR_GET_DATA(MapData);
		data->map[str] = args[1];
		return FR_UNDEFINED;
	}
	FR_METHOD(fr_Map_has) {
		FR_NARGS_EQUALS(1);
		FR_GET_STRING(str, 0);
		FR_GET_DATA(MapData);
		return FR_BOOL(data->map.contains(str));
	}
	FR_METHOD(fr_Map_delete) {
		FR_NARGS_EQUALS(1);
		FR_GET_STRING(str, 0);
		FR_GET_DATA(MapData);
		FR_ASSERT_MSG(data->map.contains(str), FR_UNDEFINED, "Attempt to delete non-existent key in map '%.*s'", str->data, (int)str->len);
		data->map.erase(str);
		return FR_UNDEFINED;
	}

	fr_Object *newMap(void) {
		MapData *data = new MapData();
		fr_Object *ret = fr_new(fr_mapDataTag, data, &destroyMap);
		#define ADD_METHOD(NAME) fr_addMethod(ret, #NAME, &fr_Map_##NAME)
		ADD_METHOD(get);
		ADD_METHOD(put);
		ADD_METHOD(has);
		ADD_METHOD(delete);
		#undef ADD_METHOD
		return ret;
	}

}
