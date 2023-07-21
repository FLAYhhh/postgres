/**
 * ptbm.h: (page table based buffer manager)
 */

#include "ptedit_header.h"


/**
 *  BufferDesc re-design:
 */

typedef ptedit_pte_t BufferDesc;

/*
 * This definition refers to BufferTag
 */
typedef struct relation_fork_tag
{
  Oid spcOid; /*tablespace oid*/
  Oid dbOid;  /*database oid*/
  RelFileNumber relNumber; /*relation file number*/
  ForkNumber forkNumber; /*fork number*/
} RelForkTag;

/*
 * Entry of MappingRangeTable
 * We also orgnize these entries using double linked list ordered by start mapping address, so that we can do remove and expand to a range more quickly.
 */
typedef struct mmap_mapping_range
{
  size_t start; /*mmap mapping start of a relation fork, (inclusive)*/
  size_t size;   /*mmap mapping size of a relation fork, (exclusive)*/
} MmapMapingRange;


struct PtbmManager {
};

void InitMappingRangeTable();

/* Only reserve vma area in pcb (build mapping logically)
 * We do actual page table mapping when bufread/fix (TODO: not sure yet)
 **/
void NewRelationForkMapping(RelForkTag *tag);
void RemoveRelaitonForkMapping(RelForkTag *tag);

/* TODO:Build mapping in page table level
 * before calling this function, we must make sure have called NewRelationForkMapping()
 * Use PTEditor to finish this function
 * Args:
 *      - smgr: file handle, possible has mapping start.
 *      - page_id: page_id in the relation fork
 *
 * TODO: May need a range interface
 **/
void AddPageTableMapping(SMgrRelation smgr, BufferTag *page_id);


//===============TODO: Buffer pool, may need to reuse pg buffer pool===================
