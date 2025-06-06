#include "list.hpp"
#include "../c/object.h"
#include "../c/objectutils.h"
	
uint32_t fr_listDataTag;

namespace fr4nken {
	static void destroyList(uint32_t tag, void *data) {
		delete (ListData *)data;
	}

	#define BOUNDS_CHECK(I) FR_ASSERT_MSG(I >= 0 && I < data->values.size(), FR_UNDEFINED, "List index %" PRId64 " out of bounds for list of length %zu", I, data->values.size())
	
	FR_METHOD(fr_List_len) {
		FR_NARGS_EQUALS(0);
		FR_GET_DATA(ListData);
		return FR_INT((int64_t)data->values.size());
	}

	FR_METHOD(fr_List_get) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		FR_GET_INT(index, 0);
		BOUNDS_CHECK(index);
		return data->values[index];
	}
	FR_METHOD(fr_List_put) {
		FR_NARGS_EQUALS(2);
		FR_GET_DATA(ListData);
		FR_GET_INT(index, 0);
		BOUNDS_CHECK(index);
		data->values[index] = args[1];
		return FR_UNDEFINED;
	}
	FR_METHOD(fr_List_push) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		data->values.push_back(args[0]);
		return FR_UNDEFINED;
	}
	FR_METHOD(fr_List_unshift) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		data->values.insert(data->values.begin(), args[0]);
		return FR_UNDEFINED;
	}
	FR_METHOD(fr_List_pop) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		auto ret = data->values.back();
		data->values.pop_back();
		return ret;
	}
	FR_METHOD(fr_List_shift) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		auto ret = data->values.front();
		data->values.erase(data->values.begin());
		return ret;
	}
	FR_METHOD(fr_List_concat) {
		FR_NARGS_EQUALS(1);
		FR_GET_DATA(ListData);
		FR_GET_LIST(other, 0);
		data->values.insert(data->values.end(), other->values.begin(), other->values.end());
		return FR_UNDEFINED;	
	}

	fr_Object *newList(void) {
		ListData *data = new ListData();
		fr_Object *ret = fr_new(fr_listDataTag, data, &destroyList);
		#define ADD_METHOD(NAME) fr_addMethod(ret, #NAME, &fr_List_##NAME)
		ADD_METHOD(len);
		ADD_METHOD(get);
		ADD_METHOD(put);
		ADD_METHOD(push);
		ADD_METHOD(unshift);
		ADD_METHOD(pop);
		ADD_METHOD(shift);
		ADD_METHOD(concat);
		#undef ADD_METHOD
		return ret;
	}
}
