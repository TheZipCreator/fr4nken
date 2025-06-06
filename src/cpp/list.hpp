#include <stdint.h>
#include <vector>

#include "../c/value.h"

extern "C" {
	extern uint32_t fr_listDataTag;
}
#ifdef __cplusplus
namespace fr4nken {
	/// Data for a list
	struct ListData {
		std::vector<fr_Value> values;
	};

	/// Creates a new list
	fr_Object *newList(void);
}
#endif
