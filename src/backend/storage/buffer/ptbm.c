#include "postgres.h"

#include "storage/bufmgr.h"
#include "storage/ipc.h"
#include "storage/ptbm.h"
#include <sys/mman.h>

static void *SharedFirstAvailableAddress;
static void *SharedNextAvailableAddress;
static void *SharedMappingMaxLimit;
/*TODO: need a LWlock to protect this Hash Table, because we also need
 * to maintain a list on the entries and these list operations are Not
 * protected by hash table's lock.
 **/
static HTAB *SharedMappingRangeHash;
static dlist_head *SharedMappingRangeList;

void InitMappingRangeTable() {
  HASHCTL info;
  info.keysize = sizeof(RelForkTag);
  info.entrysize = sizeof(MmapMapingRange);
  /* Note
   * we reuse NUM_BUFFER_PARTITIONS instead of defining a new macro because:
   * 1. No buffer table anymore, it can be used.
   * 2. if we define new Macro, we need to modify code that manipulate MainLWLockArray
   **/
  info.num_partitions = NUM_BUFFER_PARTITIONS;


  SharedMappingRangeHash = ShmemInitHash("Shared Mapping Range Lookup Table",
                                         size, size,
                                         &info,
                                         HASH_ELEM | HASH_BLOBS | HASH_PARTITION);
}

/**
 * 1.anounymous mmap
 * 2.add mapping to hash table
 * (3).(lazy syncronize) syncronize mapping to smgr ()
 *      - if mapping info isn't in smgr, check shm hash table
 *      - if isn't in shm hash table, call NewRelationForkMapping()
 * This funciton must be concurent safe
 */
void NewRelationForkMapping(RelForkTag *relfork_tag) {
  void *addr = mmap(NULL, PTBM_DEFAULT_MAPPING_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    bool found = false;
	MmapMapingRange *result = (MmapMapingRange *)
		hash_search(SharedMappingRangeHash,
									relfork_tag,
									HASH_ENTER,
									&found);
    assert(found = false);
}

void RemoveRelaitonForkMapping(RelForkTag *relfork_tag) {
  bool found = false;
  hash_search(SharedMappingRangeHash,
							relfork_tag,
							HASH_REMOVE,
							&found);
  assert(found = true);
}

void AddPageTableMapping(SmgrRelation smgr, BufferTag *page_id) {
  // 1. try to get mapping start from smgr;

  // 2. if mapping start not in smgr, find it in shm hash table, and cache it in smgr;

  // 3. compute virtual address of page_id

  // 4. alloc physical memory (get physical memory from buffer pool)

  // Warnninng: In this step, page table operations must be concurrent safe
  // 5. use PTEditor to build page table mapping from vitual addres to buffer frame pfn
}
