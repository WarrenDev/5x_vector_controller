
/**
* block_allocator.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of block allocation support.
*
*/

#ifndef SRC_SERVICES_DATABASE_DATABASE_CORE_BLOCK_ALLOCATOR_H_
#define SRC_SERVICES_DATABASE_DATABASE_CORE_BLOCK_ALLOCATOR_H_

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

typedef uint32_t  OffsetType;				// Raw offset type
typedef uint8_t	  BlockRefType;				// Block reference type
typedef	uint16_t  ShortRefType;				// Short reference type

/**
 * Computed values.
 */
enum {
    ///The total size of memory
    MEMORY_SIZE = 256UL*1024UL,
    ///The size of a block
    BLOCK_SIZE = 1024UL,
    ///The size of a read chunk
    CHUNK_SIZE = 256UL,
    ///The size of an entry in a superblock allocation table.
    TABLE_ENTRY_SIZE = 32UL,
    ///The maximum number of blocks
    MAX_BLOCKS = MEMORY_SIZE / BLOCK_SIZE,
    ///The number of entries per table
    ENTRIES_PER_TABLE = BLOCK_SIZE / TABLE_ENTRY_SIZE,
    ///The number of superblocks required
    SUPER_BLOCKS = MAX_BLOCKS / ENTRIES_PER_TABLE,
    ///The distance between superblocks
    SUPER_BLOCK_DISTANCE = ENTRIES_PER_TABLE * BLOCK_SIZE,
    ///The maximum number of free blocks available.
    MAX_FREE_BLOCKS = MAX_BLOCKS - SUPER_BLOCKS,
    //Enumeration values related to building short refs
    BITS_PER_BYTE = 8,
    BLOCK_REF_BIT_SIZE = sizeof(BlockRefType) * BITS_PER_BYTE,
    //invalid enum to catch the "trailing comma" warning.
    UNUSED_BLOCK_ENUM
};

/**
 * Get the offset of the given superblock.
 */
OffsetType getSuperBlockOffset(BlockRefType superblock);

/**
 * Get the offset of the given regular block.
 */
OffsetType getBlockOffset(BlockRefType block);

/**
 * Get the superblock to which this block belongs.
 */
BlockRefType getSuperBlockForBlock(BlockRefType block);

/**
 * Given an offset and a relative chunk, compute the chunk offset.
 */
OffsetType getChunkOffset(OffsetType offset, BlockRefType chunk);

/**
 * Given two block references, create a combined short reference.
 */
ShortRefType blockRefsToShortRef(BlockRefType high, BlockRefType low);

/**
 * Given a combined short reference, compute the block references.
 */
void computeBlockRefsFromShortRef(const ShortRefType combined, BlockRefType* high, BlockRefType* low);

/**
 * Given a block reference, compute the allocation table offset for this block.
 */
OffsetType getAllocationTableOffset(const BlockRefType block);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_DATABASE_DATABASE_CORE_BLOCK_ALLOCATOR_H_ */
