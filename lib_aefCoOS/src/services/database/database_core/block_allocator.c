
/**
* block_allocator.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of block allocation support.
*         A C implementation of the BlockAllocator C++ class written by Justin Handville.
*/

#include <stdint.h>
#include <CoOS.h>
#include "block_allocator.h"

// Some sanity checks.
_Static_assert(BLOCK_SIZE % TABLE_ENTRY_SIZE == 0, "The block size should be divisible by the table entry size so no space is wasted.");
_Static_assert(MEMORY_SIZE % BLOCK_SIZE == 0, "The memory size should be divisible by the block size.  Every byte falls in a block.");
_Static_assert(SUPER_BLOCKS * ENTRIES_PER_TABLE == MAX_BLOCKS, "All blocks have an entry in a superblock.");
_Static_assert(SUPER_BLOCKS + MAX_FREE_BLOCKS == MAX_BLOCKS, "All blocks should be accounted for.");
_Static_assert(sizeof(BlockRefType)*2 == sizeof(ShortRefType), "The shortened block ref type must be large enough to hold two block reference types.");

/**
 * Get the offset of the given superblock.
 */
OffsetType getSuperBlockOffset(BlockRefType superblock)
{
    return
        ((OffsetType)superblock) * SUPER_BLOCK_DISTANCE;
}

/**
 * Get the offset of the given regular block.
 */
OffsetType getBlockOffset(BlockRefType block)
{
    return
        ((OffsetType)block) * BLOCK_SIZE;
}

/**
 * Get the superblock to which this block belongs.
 */
BlockRefType getSuperBlockForBlock(BlockRefType block)
{
    return block / ENTRIES_PER_TABLE;
}

/**
 * Given an offset and a relative chunk, compute the chunk offset.
 */
OffsetType getChunkOffset(OffsetType offset, BlockRefType chunk)
{
    return
        offset + ((OffsetType)chunk)*CHUNK_SIZE;
}

/**
 * Given two block references, create a combined short reference.
 */
ShortRefType blockRefsToShortRef(BlockRefType high, BlockRefType low)
{
    return
        (high << BLOCK_REF_BIT_SIZE) | low;
}

/**
 * Given a combined short reference, compute the block references.
 */
void computeBlockRefsFromShortRef(const ShortRefType combined, BlockRefType* high, BlockRefType* low)
{
    *high = combined >> BLOCK_REF_BIT_SIZE;
    *low = combined & ((1 << BLOCK_REF_BIT_SIZE) - 1);
}

/**
 * Given a block reference, compute the allocation table offset for this block.
 */
OffsetType getAllocationTableOffset(const BlockRefType block)
{
    BlockRefType superblock = getSuperBlockForBlock(block);

    return
        getSuperBlockOffset(superblock)
      + ((OffsetType)block % ENTRIES_PER_TABLE) * TABLE_ENTRY_SIZE;
}

