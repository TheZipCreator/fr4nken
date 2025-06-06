#include <stdint.h>

#include "../c/value.h"

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
		std::vector<fr_Value> vec;
	};

	/// Creates a new list
	fr_Object *newList(void);
}
#endif
