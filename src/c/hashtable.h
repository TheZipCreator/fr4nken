// xstd hashtable
// modified just to used C allocator and no XSTD_SHORTEN
#ifdef __cplusplus
extern "C" {
#endif
#ifndef XSTD_HASHTABLE_GUARD
#define XSTD_HASHTABLE_GUARD

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define XSTD_INTERNAL_HASHTABLE_LOAD_FACTOR 0.65
#define XSTD_INTERNAL_HASHTABLE_EMPTY 0
#define XSTD_INTERNAL_HASHTABLE_FULL 1
#define XSTD_INTERNAL_HASHTABLE_TOMBSTONE 3

/// Generates the header for a hashtable.
///
/// A hashtable is a structure that permits arbitrary key and value associations. 
/// It is similar to C++'s `unordered_map` or Java's `HashMap`.
///
/// Adding and getting elements is best case O(1) worst case O(n); Removing elements is O(1).
///
/// Note: to use as a set, you can set the value type to an empty struct, or if
/// that's not feasible, to `bool`.
///
/// Parameters:
/// 	STRUCT - Name of the struct.
/// 	TYPEDEF - Name of the typedef for the struct.
/// 	ITERATOR - Name of the iterator struct
/// 	ITERATOR_TYPEDEF - Name of the typedef for the iterator struct.
/// 	K - Generic type for the key.
/// 	V - Generic type for the value.
#define XSTD_HASHTABLE_H(STRUCT, TYPEDEF, ITERATOR, ITERATOR_TYPEDEF, K, V) \
typedef struct STRUCT##_internal_entry { \
	uint8_t status; \
	K key; \
	V value; \
} STRUCT##_internal_entry_t; \
typedef struct STRUCT { \
	/** Capacity */ \
	size_t cap; \
	STRUCT##_internal_entry_t *_internal_entries; \
	size_t _internal_filled; \
	size_t _internal_limit; \
} TYPEDEF; \
typedef struct ITERATOR { \
	/** Current key */ \
	K *key; \
	/** Current value */ \
	V *value; \
	TYPEDEF *_internal_table; \
	size_t _internal_index; \
} ITERATOR_TYPEDEF; \
/** Creates a new hashtable with the specified capacity. */ \
TYPEDEF STRUCT##_new(size_t cap); \
/** Destroys the hashtable. Does not destroy the actual keys and values; instead iterate over them and destroy them if you need to. */ \
void STRUCT##_destroy(TYPEDEF *table); \
/** Adds a key-value pair. */ \
void STRUCT##_put(TYPEDEF *table, K key, V value); \
/** Gets the given key in the hashtable. */ \
V *STRUCT##_get(TYPEDEF *table, K key); \
/** Removes a key from a hashtable. */ \
void STRUCT##_remove(TYPEDEF *table, K key); \
/** Resizes the hashtable to be at least `new_cap` big. The new table may be resized to be more than `new_cap` if there's not enough space for existing elements. */ \
void STRUCT##_resize(TYPEDEF *table, size_t new_cap); \
/** Clears the hashtable */ \
void STRUCT##_clear(TYPEDEF *table); \
/** Checks whether the hashtable is empty. Equivalent to `len() == 0` but more performant. */ \
bool STRUCT##_empty(const TYPEDEF *table); \
/** Gets the length (i.e. number of items) of a hashtable */ \
size_t STRUCT##_len(const TYPEDEF *table); \
/** Gets an iterator struct which you can use to iterate over the hashtable. */ \
ITERATOR_TYPEDEF STRUCT##_iterator(TYPEDEF *table); \
/** Proceeds to the next item. Returns `true` if there is a next item, `false` otherwise. */ \
bool STRUCT##_next(ITERATOR_TYPEDEF *iter);

/// Generates implementation for a hashtable.
///
/// Unqiue parameters:
/// 	- HASH - Hashing function (or macro) to use for hashing keys. Should evaluate to a `size_t`.
/// 	- EQUALS - Equality test function (or macro) to use for deteting if two keys are equal. Should evaluate to a `bool`.
#define XSTD_HASHTABLE_C(STRUCT, TYPEDEF, ITERATOR, ITERATOR_TYPEDEF, K, V, HASH, EQUALS) \
TYPEDEF STRUCT##_new(size_t cap) { \
	return (TYPEDEF){ \
		cap,  \
		calloc(sizeof(STRUCT##_internal_entry_t), cap),  \
		0,  \
		cap*XSTD_INTERNAL_HASHTABLE_LOAD_FACTOR \
	}; \
} \
void STRUCT##_destroy(TYPEDEF *table) { \
	free(table->_internal_entries); \
} \
void STRUCT##_put(TYPEDEF *table, K key, V value) { \
	size_t index = (HASH(key))%(table->cap); \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	size_t slot; \
	bool no_tomb = true; \
	/* find first empty slot. if we found a tomb along the way, use that as the slot index instead of the empty slot */ \
	while(true) { \
		STRUCT##_internal_entry_t *entry = entries+index; \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_EMPTY) { \
			if(no_tomb) \
				slot = index; \
			break; \
		} \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_TOMBSTONE) { \
			if(no_tomb) { \
				no_tomb = false; \
				slot = index; \
			} \
			if(++index >= cap) \
				index = 0; \
			continue; \
		} \
		if(EQUALS(entry->key, key)) { \
			entry->value = value; \
			return; \
		} \
		if(++index >= cap) \
			index = 0; \
	} \
	table->_internal_entries[slot] = (STRUCT##_internal_entry_t){XSTD_INTERNAL_HASHTABLE_FULL, key, value}; \
	if(++table->_internal_filled >= table->_internal_limit) \
		STRUCT##_resize(table, table->cap*2); \
} \
V *STRUCT##_get(TYPEDEF *table, K key) { \
	size_t index = (HASH(key))%(table->cap); \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	while(true) { \
		STRUCT##_internal_entry_t *entry = entries+index; \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_EMPTY) \
			return NULL; \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_TOMBSTONE) {\
			if(++index >= cap) \
				index = 0; \
			continue; \
		} \
		if(EQUALS(entry->key, key)) \
			return &entry->value; \
		if(++index >= cap) \
			index = 0; \
	} \
} \
void STRUCT##_remove(TYPEDEF *table, K key) { \
	size_t hash = HASH(key); \
	size_t index = hash%(table->cap); \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	while(true) { \
		STRUCT##_internal_entry_t *entry = entries+index; \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_EMPTY) \
			return; \
		if(entry->status == XSTD_INTERNAL_HASHTABLE_TOMBSTONE) {\
			if(++index >= cap) \
				index = 0; \
			continue; \
		} \
		if(EQUALS(entry->key, key)) { \
			entry->status = XSTD_INTERNAL_HASHTABLE_TOMBSTONE; \
			return; \
		} \
		if(++index >= cap) \
			index = 0; \
	} \
} \
void STRUCT##_resize(TYPEDEF *table, size_t new_cap) { \
	while(new_cap < table->_internal_limit) \
		new_cap *= 2; \
	TYPEDEF new_table = STRUCT##_new(new_cap); \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	for(size_t i = 0; i < cap; i++) { \
		STRUCT##_internal_entry_t *entry = entries+i; \
		if(entry->status != XSTD_INTERNAL_HASHTABLE_FULL) \
			continue; \
		STRUCT##_put(&new_table, entry->key, entry->value); \
	} \
	STRUCT##_destroy(table); \
	*table = new_table; \
} \
void STRUCT##_clear(TYPEDEF *table) { \
	memset(table->_internal_entries, 0, sizeof(STRUCT##_internal_entry_t)*table->cap); \
	table->_internal_filled = 0; \
} \
bool STRUCT##_empty(const TYPEDEF *table) { \
	/* can't use _internal_filled because that includes tombstones */ \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	size_t len = 0; \
	for(size_t i = 0; i < cap; i++) { \
		if(entries[i].status == XSTD_INTERNAL_HASHTABLE_FULL) \
			return false; \
	} \
	return true; \
} \
size_t STRUCT##_len(const TYPEDEF *table) { \
	/* can't use _internal_filled because that includes tombstones */ \
	STRUCT##_internal_entry_t *entries = table->_internal_entries; \
	size_t cap = table->cap; \
	size_t len = 0; \
	for(size_t i = 0; i < cap; i++) { \
		if(entries[i].status == XSTD_INTERNAL_HASHTABLE_FULL) \
			len++; \
	} \
	return len; \
} \
ITERATOR_TYPEDEF STRUCT##_iterator(TYPEDEF *table) { \
	return (ITERATOR_TYPEDEF){NULL, NULL, table, 0}; \
} \
bool STRUCT##_next(ITERATOR_TYPEDEF *iter) { \
	STRUCT##_internal_entry_t *entries = iter->_internal_table->_internal_entries; \
	size_t cap = iter->_internal_table->cap; \
	while(true) { \
		if(iter->_internal_index >= cap) \
			return false; \
		STRUCT##_internal_entry_t *entry = &entries[iter->_internal_index++]; \
		if(entry->status != XSTD_INTERNAL_HASHTABLE_FULL) \
			continue; \
		iter->key = (K *)(&entry->key); \
		iter->value = &entry->value; \
		return true; \
	} \
}

#endif
#ifdef __cplusplus
}
#endif

