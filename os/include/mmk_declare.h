///////////////////////////////////
/// type declaration

typedef uint64_t PtHandle;

typedef uint64_t PhysAddr;
typedef uint64_t VirtAddr;
typedef uint64_t PhysPageNum;
typedef uint64_t VirtPageNum;
typedef uint64_t PageTableEntry;

///////////////////////////////////
/// bitflags

typedef uint16_t PTEFlags;
typedef uint16_t MapPermission;

#define PTE_V (1L << 0)
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)
#define PTE_G (1L << 5)
#define PTE_A (1L << 6)
#define PTE_D (1L << 7)

#define MAP_PERM_R PTE_R
#define MAP_PERM_W PTE_W
#define MAP_PERM_X PTE_X
#define MAP_PERM_U PTE_U
#define MAP_PERM_R PTE_R
#define MAP_PERM_W PTE_W
#define MAP_PERM_X PTE_X
#define MAP_PERM_U PTE_U

///////////////////////////////////
/// map type

typedef uint64_t MapType;

#define MAP_IDENTICAL (0xffffffffffffffff-1)
#define MAP_FRAMED (0xffffffffffffffff-2)
#define MAP_FRAMED (0xffffffffffffffff-3)
#define MAP_SPECIFIED(x) (x)

