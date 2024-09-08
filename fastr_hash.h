#define u64 unsigned long long
#define u16 unsigned short
#define u32 unsigned int

u32 murmur_32_scramble(u32 k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

u32 murmur3_32(const char* const key, size_t len, u32 seed)
{
	u32 h = seed;
    u32 k = 0;
    for (size_t i = len; i; i--) {
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
        k <<= 8;
        k |= key[i - 1];
    }
    h ^= murmur_32_scramble(k);
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h & 8388607; // We want the hash to be under 2^23
}

u16* init_hash_map(arena hash_arena) {
    u16* hashpos = _abytepush(hash_arena, (u64)16777600);
    return hashpos;
}

// 1 if true, 0 if false
u16 is_in_hashmap(u16* map, char* str, int length) {
    u32 result_hash = (murmur3_32(str,length,0));
    return map[result_hash];
}

u32 hashmap_push(u16* map, char* str, int length, u16 value) {
    u32 result_hash = (murmur3_32(str,length,0));
    map[result_hash] = 1;
    return result_hash;
}


#undef u64