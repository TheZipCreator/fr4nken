#ifndef FR_LIST_HPP
#define FR_LIST_HPP
#include <stdint.h>

#include "value.hpp"

#ifdef __cplusplus
#include <vector>
extern "C" {
#endif
	extern uint32_t fr_listDataTag;
#ifdef __cplusplus
}
namespace fr4nken {
	/// Data for a list
	struct ListData {
		std::vector<Value> vec;
	};

	/// Creates a new list
	fr_Object *newList(void);
}
#endif
#endif
