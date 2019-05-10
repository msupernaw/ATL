/*
 * (c) Copyright 2007, IBM Corporation.
 * 
 *	Licensed under the Apache License, Version 2.0 (the "License"); 
 *  you may not use this file except in compliance with the License. 
 *  You may obtain a copy of the License at 
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0 
 *      
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

/*****************************************************************************
 * Contact: Maged M Michael <magedm@us.ibm.com>
 ****************************************************************************/
//#define DEBUGTRACE

#ifndef CLMALLOC_H
#define	CLMALLOC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>




#define IA32_LINUX_GCC




#ifdef PPC32_AIX_XLC
#define PPC
#define BIT32
#define AIX
#define XLC
#endif

#ifdef PPC32_LINUX_GCC
#define PPC
#define BIT32
#define LINUX
#define GCC
#endif

#ifdef PPC64_AIX_XLC
#define PPC
#define BIT64
#define AIX
#define XLC
#endif

#ifdef PPC64_AIX_GCC
#define PPC
#define BIT64
#define AIX
#define GCC
#endif

#ifdef PPC64_LINUX_GCC
#define PPC
#define BIT64
#define LINUX
#define GCC
#endif

#ifdef AMD64_LINUX_GCC
#define X86
#define BIT64
#define LINUX
#define GCC

    //if there is wider cas than length of machine word, such as CMPXCHG8/16
#define CASD
#endif

#ifdef IA32_LINUX_GCC
#define X86
#define BIT32
#define LINUX
#define GCC

#define CASD
#endif

#ifdef SPARC32_SOLARIS_SCC
#define SPARC
#define BIT32
#define SOLARIS
#define SCC
#endif

    /*---------------------------------------------------------------------------*/
#define FALSE	0
#define TRUE	1
#define EIGHT	8
    /*---------------------------------------------------------------------------*/
#define STATS(x)
#ifndef MIN
#define MIN(x,y)	((x)<=(y)?(x):(y))
#endif
#define MMAP(size) \
		mmap(0,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0)
    /*---------------------------------------------------------------------------*/
#if defined(X86) || defined(SPARC)
#define REFCOUNT
#endif
    /*---------------------------------------------------------------------------*/
    //#define DEBUGTRACE
    /*---------------------------------------------------------------------------*/
#ifdef DEBUGTRACE
#define ASSERT(x)	if (!(x)) { debug(); assert(x);}
#else
    //#define ASSERT(x)	assert(x)
#define ASSERT(x)
#endif
    /*---------------------------------------------------------------------------*/
    /*---------------------------------------------------------------------------*/
#define IFUNI
    //#define IFUNI		if (!uniproc)
    /*---------------------------------------------------------------------------*/
#if defined(X86) || defined(SPARC)
#define ret_t		char
#define rc_ret_t	char
#else
#define ret_t 		void *
#define rc_ret_t	unsigned
#endif
    /*---------------------------------------------------------------------------*/
#if defined(PPC)

#define ISYNC		IFUNI asm volatile ("isync")
#define LWSYNC		IFUNI asm volatile ("lwsync")
    //#define SYNC		IFUNI asm volatile ("sync")
#define RBR
#define RBW
#define WBW

#define LL64(ret,addr) 	asm volatile ("ldarx %0,0,%1;":"=r"(ret):"r"(addr))
#define LL32(ret,addr) 	asm volatile ("lwarx %0,0,%1;":"=r"(ret):"r"(addr))
#define SC64(ret,addr,new_val) \
 asm volatile ( \
	"  stdcx. %2,0,%1;\n" \
 	"  mfcr %0;\n" \
 	"  andis. %0,%0,0x2000;" \
	: "=r"(ret):"r"(addr),"r"(new_val) \
        : "cr0","memory")
#define SC32(ret,addr,new_val) \
 asm volatile ( \
	"  stwcx. %2,0,%1;\n" \
 	"  mfcr %0;\n" \
 	"  andis. %0,%0,0x2000;" \
	: "=r"(ret):"r"(addr),"r"(new_val) \
        : "cr0","memory")
#define CAS64(ret,addr,expval,new_val) \
 do { \
    unsigned long long oldval; \
    LL64(oldval,addr); \
    if (oldval != (unsigned long long)expval) { ret = 0; break; } \
    SC64(ret,addr,new_val); \
 } while (!ret);
#define CAS32(ret,addr,expval,new_val) \
 do { \
    unsigned oldval; \
    LL32(oldval,addr); \
    if (oldval != (unsigned)expval) { ret = 0; break; } \
    SC32(ret,addr,new_val); \
 } while (!ret);

#ifdef BIT64

#define LL(ret,addr)			LL64(ret,addr)
#define SC(ret,addr,new_val)		SC64(ret,addr,new_val)
#define CAS(ret,addr,expval,new_val)	CAS64(ret,addr,expval,new_val)
#define ptrsize_t			unsigned long

#else /* PPC32 */

#define LL(ret,addr)			LL32(ret,addr)
#define SC(ret,addr,new_val)		SC32(ret,addr,new_val)
#define CAS(ret,addr,expval,new_val)	CAS32(ret,addr,expval,new_val)
#define ptrsize_t			unsigned

#endif

#define anchorsize_t			ptrsize_t
#define LLA(ret,addr)			LL(ret,addr)
#define SCA(ret,addr,expval,new_val)    	SC(ret,addr,new_val)

#elif defined(X86)

#define CAS32(ret,addr,expval,new_val) \
 __asm__ __volatile__( \
    "lock; cmpxchg %2, %1; setz %0;\n" \
    : "=a"(ret), "=m"(*(addr)) \
    : "r" (new_val), "a"(expval) \
    : "cc")

    char cas64(unsigned long long volatile * addr,
            unsigned long long oval,
            unsigned long long nval);

#define CAS64(ret,addr,expval,new_val) \
	ret = cas64((unsigned long long volatile *)addr, \
		    (unsigned long long)expval, \
		    (unsigned long long)new_val)

#define ISYNC
#define LWSYNC
#define SYNC
    //		IFUNI asm volatile ("mfence")
#define RBR
#define RBW
#define WBW

#ifdef BIT32 

#define ptrsize_t			unsigned long
#define CAS(ret,addr,expval,new_val)	CAS32(ret,addr,expval,new_val)
#define LLA(ret,addr)			ret = *(addr);
#define SCA(ret,addr,expval,new_val)	CAS32(ret,addr,expval,new_val)
#define anchorsize_t			unsigned long

#else /* X64 */

#define ptrsize_t			unsigned long long
#define CAS(ret,addr,expval,new_val)	CAS64(ret,addr,expval,new_val)
#define LLA(ret,addr)			ret = *(addr);
#define SCA(ret,addr,expval,new_val)	CAS64(ret,addr,expval,new_val)
#define anchorsize_t			unsigned long long

#endif /* X86 32 and 64 */

#elif defined(SPARC)

#define CAS32(ret,addr,expval,new_val) \
	ret = cas32((unsigned volatile *)addr,(unsigned)expval,(unsigned)new_val)

    static char cas32(unsigned volatile * addr, unsigned expval, unsigned new_val) {
        asm volatile ( \
       "casa  [%2] 0x80, %3, %1\n" \
       : "+m"(*(addr)), "+r"(new_val) \
       : "r"(addr), "r" (expval) \
       );
        return (new_val == expval);
    }

#define CAS64(ret,addr,expval,new_val) \
	ret = cas64((unsigned long long volatile *)addr,(unsigned long long)expval,(unsigned long long)new_val)

    static char cas64(unsigned long long volatile * addr, unsigned long long expval, unsigned long long new_val) {
        asm volatile ( \
       "casxa  [%2] 0x80, %3, %1\n" \
       : "+m"(*(addr)), "+r"(new_val) \
       : "r"(addr), "r" (expval) \
       );
        return (new_val == expval);
    }

#define ISYNC
#define LWSYNC
#define SYNC
#define RBR	IFUNI asm volatile ("membar #LoadLoad")
#define RBW	IFUNI asm volatile ("membar #LoadStore")
#define WBW	IFUNI asm volatile ("membar #StoreStore")

#ifdef BIT32 

#define ptrsize_t			unsigned
#define CAS(ret,addr,expval,new_val)	CAS32(ret,addr,expval,new_val)
#define LLA(ret,addr)			ret = *(addr);
#define SCA(ret,addr,expval,new_val)	CAS32(ret,addr,expval,new_val)
#define anchorsize_t			unsigned

#else /* SPARC64 */

#define ptrsize_t			unsigned long long
#define CAS(ret,addr,expval,new_val)	CAS64(ret,addr,expval,new_val)
#define LLA(ret,addr)			ret = *(addr);
#define SCA(ret,addr,expval,new_val)	CAS64(ret,addr,expval,new_val)
#define anchorsize_t			unsigned long long

#endif /* SPARC32 v SPARC64 */

#else
#error architecture not defined
#endif /* architecture */
    /*---------------------------------------------------------------------------*/
#if defined(X86) || defined(SPARC)
#define aret_t				char
#else
#define aret_t				void *
#endif
    /*---------------------------------------------------------------------------*/
#define FAA(oldval,addr,val) \
 do { \
    ret_t ret; \
    ptrsize_t new_val; \
    oldval = *(addr); \
    new_val = (ptrsize_t)oldval + val; \
    CAS(ret,addr,new_val); \
    if (ret) break; \
 } while (1);
    /*---------------------------------------------------------------------------*/
    /* Reference counting */
#define SAFE_READ(x)		rc_safe_read(&x)
#define DESC_RETIRE(x)		rc_desc_release(x)
#define RC_INIT(x)		x->rc = 1
#define RC_RELEASE(x)		rc_desc_release(x)

/*---------------------------------------------------------------------------*/
    typedef union anchor {

        struct {
            unsigned head : 7;
            unsigned count : 7;
            unsigned active : 1;
            unsigned valid : 1;
            anchorsize_t tag : 8 * sizeof (anchorsize_t) - 16;
        } sep;
        anchorsize_t all;
    } anchor_t;

    /*---------------------------------------------------------------------------*/
    typedef struct desc {
        anchor_t volatile Anchor;
        struct desc * volatile Next;
        void * volatile SB;
        struct procheap * volatile Heap;
        struct desc * volatile Pdesc;
        struct desc * volatile Ptr;
        int sz;
        int maxcount;
        int volatile rc;
        char pad[128
                - sizeof (anchor_t)
        - 5 * sizeof (void*)
        - 3 * sizeof (int)
        ];
    } desc_t;

    /*---------------------------------------------------------------------------*/
    typedef struct procheap {
        desc_t * volatile Active;
        desc_t * volatile Partial;
        struct sizeclass * sc;
        char pad[32 - 3 * sizeof (void*) ];
    } procheap_t;

    /*---------------------------------------------------------------------------*/
    typedef struct sizeclass {
        procheap_t procheap;
        desc_t * volatile PartialHead;
        desc_t * volatile PartialTail;
        unsigned numprocheaps;
        unsigned sz;
        unsigned sbsize;
        unsigned maxcount;
        char pad[128
                - sizeof (procheap_t)
        - 2 * sizeof (void*)
        - 4 * sizeof (unsigned)
        ];
    } sizeclass_t;
    /*---------------------------------------------------------------------------*/
#define MAXSIZECLASSES		32 /* max size classes */

typedef struct {
        sizeclass_t sizeclass[MAXSIZECLASSES];
        procheap_t procheap[1024][MAXSIZECLASSES];
        /* not really going to allocate that much */
    } controlblock_t;
    /*---------------------------------------------------------------------------*/
#define MAXCREDITS		128
#define CREDITSMASK		127
#define DESCBATCHSIZE		128
#define DESCSBSIZE		DESCBATCHSIZE*sizeof(desc_t)
#define HYPERBLOCKSIZE		1024*1024
#define DEFAULTNUMHEAPS		64
#define MAXNUMHEAPS		512
#define NUMSIZECLASSES		27
    /*---------------------------------------------------------------------------*/
    //static char pad1[128];
    static controlblock_t * volatile controlblock = NULL;
    static unsigned numprocheaps = DEFAULTNUMHEAPS;
    static unsigned heapidmask;
    //static char pad2[128];
    static unsigned scsz[NUMSIZECLASSES] = {
        16, 24, 32, 40, 48, 64, 80, 96,
        128, 160, 192, 256, 320, 384, 512, 640,
        768, 1024, 1280, 2048, 4096, 8192, 16384, 32768,
        65536, 131072, 262144
    };
    //static char pad3[128];
    static desc_t * volatile DescAvail = NULL;
    static desc_t * volatile HyperblockHead = NULL;
    /*---------------------------------------------------------------------------*/
    /*---------------------------------------------------------------------------*/
#define DOTRACE(a,b,c,d,e,f,g) \
 if (!trace_stop) { \
    unsigned id; \
    unsigned i; \
    id =  (pthread_self() & 0xf)-1; /* only for AIX */ \
    i = trace_ind[id][0]++ & TRACEMASK; \
    tr[id][i].op = a; \
    tr[id][i].sz = b; \
    tr[id][i].desc = d; \
    tr[id][i].heap = f; \
    tr[id][i].anchor.all = g; \
 }
    /*
        tr[id][i].sb = e; \
        tr[id][i].id = id; \
        tr[id][i].addr = c; \
        unsigned ind; \
        FAA32(ind,&trace_ind,1); \
     */
#ifdef DEBUGTRACE
#define TRACE(a,b,c,d,e,f,g)  std::cout<<a<<","<<b<<","<<c<<","<<d<<","<<e<<","<<f<<","<<g<<"\n";
#else
#define TRACE(a,b,c,d,e,f,g)
#endif
    /*---------------------------------------------------------------------------*/
#ifdef DEBUGTRACE

typedef struct trace {
        unsigned id;
        unsigned op;
        unsigned sz;
        unsigned pad;
        void * addr;
        void * desc;
        void * sb;
        void * heap;
        anchor_t anchor;
    } trace_t;
    /*---------------------------------------------------------------------------*/
#define MAXTRACE	0x8000
#define TRACEMASK	MAXTRACE-1
#define OP_MAKE_ACTIVE		1
#define OP_MADE_ACTIVE		2
#define OP_NOT_ACTIVE		3
#define OP_PARTIAL_STR		4
#define OP_PARTIAL_RSRV		5
#define OP_PARTIAL_END		6
#define OP_new__STR		7
#define OP_NOT_new_		8
#define OP_new__END		9
#define OP_MALLOC_STR		10
#define OP_REGULAR_RSRV		11
#define OP_REGULAR_END		12
#define OP_FREE_STR		13
#define OP_FREE_END		14
#define OP_LIST_ENQ_STR		15
#define OP_LIST_ENQ_END		16
#define OP_PARTIAL_PUSH		17
#define OP_PARTIAL_POP_SLT	18
#define OP_PARTIAL_POP_LST	19
#define OP_PARTIAL_POPPED	20
#define OP_DESC_REMOVE_SLT	21
#define OP_DESC_REMOVE_LST	22
#define OP_DESC_REMOVED		23
#define OP_DESC_NOT_REMOVED    	24
    /*---------------------------------------------------------------------------*/
    unsigned volatile trace_ind[3][32];
    ptrsize_t volatile trace_stop = 0;
    unsigned volatile trace_dumped = 0;
    trace_t tr[3][MAXTRACE];

    /*---------------------------------------------------------------------------*/
    void debug() {
        unsigned total, i, j, k;
        FILE * fd;
        ret_t ret;
        char opstr[40];
        char trfile[40];

        printf("%d assertion failed .....\n", pthread_self());
        CAS(ret, &trace_stop, 0, 1);
        if (!ret) {
            while (!trace_dumped);
            return;
        }
        printf("%d dumping trace .....\n", pthread_self());
        strcpy(trfile, "trace");
        fd = fopen(trfile, "w");
        assert(fd);
        for (k = 0; k < 3; k++) {
            if (trace_ind[k][0] <= MAXTRACE) {
                j = 0;
                total = trace_ind[k][0];
            } else {
                j = trace_ind[k][0] % MAXTRACE;
                total = MAXTRACE;
            }
            printf("%d total trace = %lld  %d\n", k, trace_ind[k][0], total);
            for (i = 1; i <= total; i++) {
                switch (tr[k][j].op) {
                    case OP_MAKE_ACTIVE: strcpy(opstr, "MAKE_ACTIVE");
                        break;
                    case OP_MADE_ACTIVE: strcpy(opstr, "MADE_ACTIVE");
                        break;
                    case OP_NOT_ACTIVE: strcpy(opstr, "NOT_ACTIVE");
                        break;
                    case OP_PARTIAL_STR: strcpy(opstr, "PARTIAL_STR");
                        break;
                    case OP_PARTIAL_RSRV: strcpy(opstr, "PARTIAL_RSRV");
                        break;
                    case OP_PARTIAL_END: strcpy(opstr, "PARTIAL_END");
                        break;
                    case OP_new__STR: strcpy(opstr, "new__STR");
                        break;
                    case OP_NOT_new_: strcpy(opstr, "NOT_new_");
                        break;
                    case OP_new__END: strcpy(opstr, "new__END");
                        break;
                    case OP_MALLOC_STR: strcpy(opstr, "MALLOC_STR");
                        break;
                    case OP_REGULAR_RSRV: strcpy(opstr, "REGULAR_RSRV");
                        break;
                    case OP_REGULAR_END: strcpy(opstr, "REGULAR_END");
                        break;
                    case OP_FREE_STR: strcpy(opstr, "FREE_STR");
                        break;
                    case OP_FREE_END: strcpy(opstr, "FREE_END");
                        break;
                    case OP_LIST_ENQ_STR: strcpy(opstr, "LIST_ENQ_STR");
                        break;
                    case OP_LIST_ENQ_END: strcpy(opstr, "LIST_ENQ_END");
                        break;
                    case OP_PARTIAL_PUSH: strcpy(opstr, "PARTIAL_PUSH");
                        break;
                    case OP_PARTIAL_POP_SLT: strcpy(opstr, "PARTIAL_POP_SLT");
                        break;
                    case OP_PARTIAL_POP_LST: strcpy(opstr, "PARTIAL_POP_LST");
                        break;
                    case OP_PARTIAL_POPPED: strcpy(opstr, "PARTIAL_POPPED");
                        break;
                    case OP_DESC_REMOVE_SLT: strcpy(opstr, "DESC_REMOVE_SLT");
                        break;
                    case OP_DESC_REMOVE_LST: strcpy(opstr, "DESC_REMOVE_LST");
                        break;
                    case OP_DESC_REMOVED: strcpy(opstr, "DESC_REMOVED");
                        break;
                    case OP_DESC_NOT_REMOVED: strcpy(opstr, "DESC_NOT_REMOVED");
                        break;
                    default: assert(0);
                }
                fprintf(fd, "%5d ", i);
                /* fprintf(fd,"p%d ",tr[k][j].id); */
                fprintf(fd, "p%d ", k);
                fprintf(fd, "sz=%5d ", tr[k][j].sz);
                //fprintf(fd,"a = 0x%15llx ",tr[k][j].addr);
                fprintf(fd, "d=%6llx ", (ptrsize_t) tr[k][j].desc % 0x1000000);
                //fprintf(fd,"sb = 0x%15llx ",tr[k][j].sb);
                fprintf(fd, "h=%5llx ", (ptrsize_t) tr[k][j].heap % 0x100000);
                fprintf(fd, "<%2d,", tr[k][j].anchor.sep.head);
                fprintf(fd, "%05x,", tr[k][j].anchor.sep.tag % 0x100000);
                fprintf(fd, "%2d,", tr[k][j].anchor.sep.count);
                fprintf(fd, "%d,", tr[k][j].anchor.sep.active);
                fprintf(fd, "%d> ", tr[k][j].anchor.sep.valid);
                fprintf(fd, "%s ", opstr);
                fprintf(fd, "\n");
                j = (j + 1) % MAXTRACE;
            }
        }
        fflush(fd);
        fclose(fd);
        trace_dumped = 1;
    }
#endif
    /*---------------------------------------------------------------------------*/
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void __malloc_start__()
#else

__attribute__((constructor)) void clfmalloc_init()
#endif
    {
        controlblock_t * cb;
        sizeclass_t * sc;
        ret_t ret;
        unsigned size;
        unsigned i;


        assert(sizeof (desc_t) == 128);
        assert(sizeof (sizeclass_t) == 128);
        assert(sizeof (procheap_t) == 32);
        assert(NUMSIZECLASSES <= MAXSIZECLASSES);

       
        /* Set number of proc heaps */
        char * nphstr = getenv("CLFMALLOC_NUMHEAPS");
        if (nphstr) {
            int nph = atoi(nphstr);
            if ((nph >= 1) && (nph <= MAXNUMHEAPS)) {
                numprocheaps = 1;
                while (numprocheaps < nph)
                    numprocheaps <<= 1;
            }
        }
        heapidmask = numprocheaps - 1;

        /* allocate control block */
        size = 4096 + numprocheaps * 1024;
        if ((cb = (controlblock_t *) MMAP(size)) == MAP_FAILED) {
            perror("clfmalloc_init mmap failed\n");
        }

        for (i = 0; i < NUMSIZECLASSES; i++) {
            sc = &cb->sizeclass[i];
            sc->PartialHead = NULL;
            sc->PartialTail = NULL;
            sc->sz = scsz[i];
            if (sc->sz <= 32) {
                sc->sbsize = 2 * 1024;
            } else if (sc->sz <= 64) {
                sc->sbsize = 4 * 1024;
            } else if (sc->sz <= 128) {
                sc->sbsize = 8 * 1024;
            } else if (sc->sz <= 256) {
                sc->sbsize = 16 * 1024;
            } else if (sc->sz <= 512) {
                sc->sbsize = 32 * 1024;
            } else if (sc->sz <= (32 * 1024)) {
                sc->sbsize = 64 * 1024;
            } else {
                sc->sbsize = HYPERBLOCKSIZE;
            }
            sc->maxcount = sc->sbsize / sc->sz;
            if ((i < 20) && (numprocheaps > 1)) {
                unsigned j;
                sc->numprocheaps = numprocheaps;
                for (j = 0; j < sc->numprocheaps; j++) {
                    cb->procheap[j][i].Active = NULL;
                    cb->procheap[j][i].Partial = NULL;
                    cb->procheap[j][i].sc = sc;
                }
            } else {
                sc->numprocheaps = 1;
                sc->procheap.Active = NULL;
                sc->procheap.Partial = NULL;
                sc->procheap.sc = sc;
            }
        }
        LWSYNC; /* wbw */
        WBW;
        CAS(ret, &controlblock, NULL, cb);
        if (!ret) {
            if (munmap((void *) cb, size)) {
                perror("clfmalloc_init munmap failed\n");
            }
        }
    }

    /*---------------------------------------------------------------------------*/
    static void desc_push(desc_t * desc) {
        ret_t ret;
        do {
            desc_t * head = DescAvail;
            desc->Next = head;
            LWSYNC; /* wbw */
            WBW;
            CAS(ret, &DescAvail, head, desc); /* no ABA */
        } while (!ret);
    }
    /*---------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------------*/
    static void rc_desc_release(desc_t * desc) {
        int old, new_;
        rc_ret_t ret;

        LWSYNC; /* rbw */
        RBW;
        ASSERT(desc);
        do {
            old = desc->rc;
            new_ = old - 1;
            CAS32(ret, &desc->rc, old, new_);
        } while (!ret);
        ASSERT(new_ >= 0);
        if (new_ == 0) {
            /* Probably store is sufficient */
            do {
                old = desc->rc;
                new_ = old + 1;
                CAS32(ret, &desc->rc, old, new_);
            } while (!ret);
            desc_push(desc);
        }
    }

    /*---------------------------------------------------------------------------*/
    static desc_t * rc_safe_read(desc_t * volatile * addr) {
        desc_t * desc;
        int old, new_;
        rc_ret_t ret;

        while (1) {
            desc = *addr;
            if (desc == NULL) return NULL;
            do {
                old = desc->rc;
                new_ = old + 1;
                if (old == 0) break;
                CAS32(ret, &desc->rc, old, new_);
            } while (!ret);
            if (old == 0) continue;
            ISYNC; /* rbr */
            RBR;
            if (desc == *addr) return desc;
            rc_desc_release(desc);
        }
    }
    /*---------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------------*/
    static desc_t * desc_alloc() {
        desc_t * desc;
        ret_t ret;

        while (1) {
            desc = SAFE_READ(DescAvail);
            if (desc) {
                desc_t * next = desc->Next;
                CAS(ret, &DescAvail, desc, next); /* ABA */
                RC_RELEASE(desc);
                if (ret) {
                    break;
                }
            } else {
                desc_t * ptr;
                if ((desc = (desc_t *) MMAP(DESCSBSIZE)) == MAP_FAILED) {
                    perror("desc_alloc mmap failed\n");
                }
                RC_INIT(desc);
                for (ptr = desc + 1;
                        (ptrsize_t) (ptr + 1)<(ptrsize_t) (desc) + DESCSBSIZE;
                        ptr++) {
                    ptr->Next = ptr + 1;
                    RC_INIT(ptr);
                }
                ptr->Next = NULL;
                RC_INIT(ptr);
                LWSYNC; /* wbw */
                WBW;
                CAS(ret, &DescAvail, NULL, (desc_t *) (desc + 1));
                if (ret)
                    break;
                if (munmap((void *) desc, DESCSBSIZE)) {
                    perror("desc_alloc munmap failed\n");
                }
            }
        }
        desc->Ptr = (desc_t *) 0xfedcba98;
        desc->Heap = (procheap_t *) 0xfedcba98;
        return desc;
    }
    /*---------------------------------------------------------------------------*/
    static void * super_malloc(unsigned sz);
    static void super_free(void * ptr);

    /*---------------------------------------------------------------------------*/
    static void sb_alloc(desc_t * desc) {
        sizeclass_t * sc;
        void ** addr;
        unsigned sz;

        sc = desc->Heap->sc;
        sz = sc->sz;
        if (sc->sbsize < HYPERBLOCKSIZE) {
            ptrsize_t num;
            addr = (void**) super_malloc(sc->sbsize - EIGHT);
            num = (ptrsize_t) addr;
            num -= EIGHT;
            addr = (void**) num;
            desc->Pdesc = *(desc_t**) addr;
        } else {
            /* sbsize == 1M */
            desc->Pdesc = NULL;
            do {
                desc_t * head;
                desc_t * next;
                ret_t ret;

                head = SAFE_READ(HyperblockHead);
                if (head) {
                    next = head->Next;
                    CAS(ret, &HyperblockHead, head, next); /* ABA */
                    RC_RELEASE(head);
                    if (ret) {
                        addr = (void**) head->SB;
                        DESC_RETIRE(head);
                        break;
                    }
                } else {
                    ASSERT(sc->sbsize == 0x100000);
                    if ((addr = (void **) MMAP(sc->sbsize)) == MAP_FAILED) {
                        perror("sb_alloc mmap failed\n");
                    }
                    break;
                }
            } while (1);
        }
        /* prepare sb - make linked list - set header to desc */
        {
            unsigned i;
            desc_t ** ptr1;
            unsigned * ptr2;
            ptrsize_t num1;
            ptrsize_t num2;

            num1 = (ptrsize_t) addr;
            for (i = 0; i < sc->maxcount; i++) {
                ptr1 = (desc_t**) num1;
                *ptr1 = desc;
                num2 = num1 + EIGHT;
                ptr2 = (unsigned*) num2;
                *ptr2 = i + 1;
                num1 += sz;
            }
        }
        desc->SB = addr;
    }

    /*---------------------------------------------------------------------------*/
    static void sb_dealloc(void * sb, desc_t * pdesc) {
        if (pdesc) {
            /* sbsize <= 64K */
            *(desc_t**) sb = pdesc;
            sb = (char*) sb + EIGHT;
            super_free(sb);
        } else {
            /* sbsize is 1 MB */
            ret_t ret;

            desc_t * desc = desc_alloc();
            desc->SB = sb;
            do {
                desc_t * head;
                head = HyperblockHead;
                desc->Next = head;
                LWSYNC; /* wbw */
                WBW;
                CAS(ret, &HyperblockHead, head, desc); /* no ABA */
            } while (!ret);
        }
    }
    /*---------------------------------------------------------------------------*/
#ifdef REFCOUNT

/*---------------------------------------------------------------------------*/
    static void list_enqueue(sizeclass_t * sc, desc_t * desc) {
        ret_t ret;
        ASSERT(sc);
        desc_t * aux = desc_alloc();
        aux->Ptr = desc;
        do {
            desc_t * head = sc->PartialHead;
            aux->Next = head;
            LWSYNC; /* wbw */
            WBW;
            CAS(ret, &sc->PartialHead, head, aux);
        } while (!ret);
        TRACE(OP_LIST_ENQ_END, desc->sz, 0, desc, desc->SB, desc->Heap, desc->Anchor.all);
    }

    /*---------------------------------------------------------------------------*/
    static desc_t * list_pop(sizeclass_t * sc) {
        /* pop from central list */
        desc_t * desc;
        ret_t ret;
        TRACE(OP_PARTIAL_POP_LST, 0, 0, 0, 0, 0, 0);
        do {
            desc_t * head = SAFE_READ(sc->PartialHead);
            if (!head)
                return NULL;
            desc_t * next = head->Next;
            desc = head->Ptr;
            CAS(ret, &sc->PartialHead, head, next);
            RC_RELEASE(head);
            if (ret) {
                DESC_RETIRE(head);
                ASSERT(desc);
                break;
            }
        } while (1);
        TRACE(OP_PARTIAL_POPPED, desc->sz, 0, desc, desc->SB, desc->Heap, desc->Anchor.all);
        return desc;

    }

    /*---------------------------------------------------------------------------*/
    static void partial_push(procheap_t * heap, desc_t * desc) {
        ret_t ret;
        desc_t * desc2;

        ASSERT(desc);
        ASSERT(heap);
        ASSERT(heap->sc);
        do {
            desc2 = heap->Partial;
            CAS(ret, &heap->Partial, desc2, desc); /* no ABA */
        } while (!ret);
        TRACE(OP_PARTIAL_PUSH, desc->sz, 0, desc, desc->SB, heap, desc->Anchor.all);
        if (!desc2)
            return;
        if (!desc2->Anchor.sep.valid) {
            DESC_RETIRE(desc2);
        } else {
            list_enqueue(heap->sc, desc2);
        }
    }

    /*---------------------------------------------------------------------------*/
    static desc_t * partial_pop(procheap_t* heap) {
        ret_t ret;
        TRACE(OP_PARTIAL_POP_SLT, 0, 0, 0, 0, heap, 0);
        ASSERT(heap);
        ASSERT(heap->sc);
        do {
            desc_t * desc = heap->Partial;
            if (!desc)
                break;
            CAS(ret, &heap->Partial, desc, NULL); /* no ABA */
            if (ret) {
                return desc;
            }
        } while (1);
        /* pop from central list */
        return list_pop(heap->sc);
    }

    /*---------------------------------------------------------------------------*/
    static void list_desc_remove(procheap_t * heap) {
    }
    /*---------------------------------------------------------------------------*/
#else /* LLSC */

/*---------------------------------------------------------------------------*/
    static void list_enqueue(desc_t * desc) {
        sizeclass_t * sc;

        //TRACE(OP_LIST_ENQ_STR,desc->sz,0,desc,desc->SB,desc->Heap,desc->Anchor.all);
        sc = desc->Heap->sc;
        ASSERT(sc);
        desc->Next = NULL;
        LWSYNC;
        while (1) {
            desc_t * tail;
            desc_t * head;
            desc_t * next;
            void * ret;

            tail = sc->PartialTail;
            if (tail == NULL) {
                LL(tail, &sc->PartialTail);
                if (tail != NULL)
                    continue;
                SC(ret, &sc->PartialTail, desc); /* no ABA */
                if (!ret)
                    continue;
                CAS(ret, &sc->PartialHead, NULL, desc);
                break;
            }
            /* tail != NULL */
            if (sc->PartialHead == NULL) {
                LL(head, &sc->PartialHead);
                if (head != NULL)
                    continue;
                ISYNC;
                if (sc->PartialTail != tail)
                    continue;
                SC(ret, &sc->PartialHead, tail); /* no ABA */
                continue;
            }
            /* tail != NULL && head != NULL */
            next = tail->Next;
            if (next != NULL) {
                /* help previous enqueue */
                LL(ret, &sc->PartialTail);
                if (ret != tail)
                    continue;
                ISYNC;
                if (tail->Next != next)
                    continue;
                SC(ret, &sc->PartialTail, next); /* ABA */
                continue;
            }
            /* next == NULL */
            LL(next, &tail->Next);
            if (next != NULL)
                continue;
            ISYNC;
            if (sc->PartialTail != tail)
                continue;
            SC(ret, &tail->Next, desc); /* ABA */
            if (!ret)
                continue;
            LL(ret, &sc->PartialTail);
            if (ret != tail)
                break;
            ISYNC;
            if (tail->Next != desc)
                break;
            SC(ret, &sc->PartialTail, desc); /* ABA */
            break;
        }
        TRACE(OP_LIST_ENQ_END, desc->sz, 0, desc, desc->SB, desc->Heap, desc->Anchor.all);
    }

    /*---------------------------------------------------------------------------*/
    static void partial_push(procheap_t * heap, desc_t * desc) {
        ret_t ret;
        desc_t * desc2;

        //ASSERT(desc);
        //ASSERT(heap);
        //ASSERT(heap->sc);
        do {
            LL(desc2, &heap->Partial);
            SC(ret, &heap->Partial, desc); /* no ABA */
        } while (!ret);
        TRACE(OP_PARTIAL_PUSH, desc->sz, 0, desc, desc->SB, heap, desc->Anchor.all);
        if (!desc2) return;
        if (!desc2->Anchor.sep.valid) {
            desc_push(desc2); /* LL/SC no rc or hp */
            return;
        } else {
            list_enqueue(desc2);
        }
    }

    /*---------------------------------------------------------------------------*/
    static desc_t* partial_pop(procheap_t* heap) {
        desc_t* desc;
        desc_t* next;
        sizeclass_t* sc;
        void* ret;
        anchor_t tmp;

        TRACE(OP_PARTIAL_POP_SLT, 0, 0, 0, 0, heap, 0);
        ASSERT(heap);
        ASSERT(heap->sc);
        do {
            LL(desc, &heap->Partial);
            if (!desc)
                break;
            SC(ret, &heap->Partial, NULL); /* no ABA */
            if (ret) {
                /*
                tmp.all = desc->Anchor.all;
                TRACE(OP_PARTIAL_POPPED,desc->sz,0,desc,desc->SB,heap,tmp.all);
                ASSERT(!tmp.sep.active);
                ASSERT(tmp.sep.count > 0);
                 */
                return desc;
            }
        } while (1);
        /* pop from central list */
        TRACE(OP_PARTIAL_POP_LST, 0, 0, 0, 0, heap, 0);
        sc = heap->sc;
        do {
            LL(desc, &sc->PartialHead);
            if (!desc)
                return NULL;
            next = desc->Next;
            if (next == NULL) {
                if (!desc->Anchor.sep.valid)
                    return NULL;
                else {
                    desc_t * desc2;
                    desc2 = desc_alloc();
                    desc2->Anchor.sep.valid = FALSE;
                    desc2->Heap = heap; /* needed */
                    list_enqueue(desc2);
                }
                continue;
            }
            ISYNC;
            if (sc->PartialTail == desc) {
                /* help the enqueue */
                LL(ret, &sc->PartialTail);
                if (ret != desc)
                    continue;
                ISYNC;
                if (desc->Next != next)
                    continue;
                SC(ret, &sc->PartialTail, next); /* ABA */
                continue;
            }
            SC(ret, &sc->PartialHead, next); /* ABA */
            if (ret) {
                break;
            }
        } while (1);
        TRACE(OP_PARTIAL_POPPED, desc->sz, 0, desc, desc->SB, heap, desc->Anchor.all);
        return desc;
    }

    /*---------------------------------------------------------------------------*/
    static void list_desc_remove(procheap_t * heap) {
        desc_t * head;
        desc_t * next;
        sizeclass_t * sc;
        void * ret;
        unsigned nonempty = 0;

        TRACE(OP_DESC_REMOVE_SLT, 0, 0, 0, 0, heap, 0);
        ASSERT(heap);
        ASSERT(heap->sc);
        do {
            LL(head, &heap->Partial);
            if (!head)
                break;
            if (head->Anchor.sep.valid)
                break;
            SC(ret, &heap->Partial, NULL); /* can recover later from ABA */
            if (ret) {
                ISYNC;
                if (head->Anchor.sep.valid) {
                    /* ABA happenned - put it back */
                    partial_push(heap, head);
                    break;
                } else {
                    /* really invalid */
                    TRACE(OP_DESC_REMOVED, head->sz, 0, head, head->SB, heap, head->Anchor.all);
                    desc_push(head); /****/
                    return;
                }
            }
        } while (1);
        /* pop from global heap */
        TRACE(OP_DESC_REMOVE_LST, 0, 0, 0, 0, heap, 0);
        sc = heap->sc;
        do {
            LL(head, &sc->PartialHead);
            if (!head)
                break;
            next = head->Next;
            if (!next)
                break;
            ISYNC;
            if (sc->PartialTail != head) {
                ISYNC;
                if (head->Next != next)
                    continue;
                SC(ret, &sc->PartialHead, next); /* ABA */
                if (ret) {
                    if (head->Anchor.sep.valid) {
                        ASSERT(head->Heap);
                        ASSERT(head->Heap->sc);
                        list_enqueue(head);
                        if (nonempty)
                            break;
                        nonempty = 1;
                    } else {
                        TRACE(OP_DESC_REMOVED, head->sz, 0, head, head->SB, heap, head->Anchor.all);
                        desc_push(head); /***/
                        return;
                    }
                }
            } else {
                /* help the enqueue */
                LL(ret, &sc->PartialTail);
                if (ret != head)
                    continue;
                ISYNC;
                if (head->Next != next)
                    continue;
                SC(ret, &sc->PartialTail, next); /* ABA */
            }
        } while (1);
        TRACE(OP_DESC_NOT_REMOVED, 0, 0, 0, 0, heap, 0);
    }
    /*---------------------------------------------------------------------------*/
#endif /* REFCOUNT or LLSC */
    /*---------------------------------------------------------------------------*/
#ifdef PLDI2004

/*---------------------------------------------------------------------------*/
    static void make_active(procheap_t * heap, desc_t * desc, unsigned morecredits) {
        ptrsize_t new_active;
        anchor_t oldanchor;
        anchor_t new_anchor;
        ret_t ret;

        TRACE(OP_MAKE_ACTIVE, desc->sz, 0, desc, desc->SB, heap, desc->Anchor.all);
        new_active = (ptrsize_t) (desc)+(morecredits - 1);
        CAS(ret, &heap->Active, NULL, new_active);
        if (ret) {
            TRACE(OP_MADE_ACTIVE, desc->sz, 0, desc, desc->SB, heap, desc->Anchor.all);
            return;
        }
        /*
          Someone installed another active sb.
          Return credits and make sb partial
         */
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            new_anchor.sep.count += morecredits;
            new_anchor.sep.active = FALSE;
            new_anchor.sep.tag++; /* for debugging */
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all); /* no ABA */
            if (aret)
                break;
        } while (1);
        TRACE(OP_NOT_ACTIVE, desc->sz, 0, desc, desc->SB, heap, new_anchor.all);
        LWSYNC; /* wbw */
        WBW;
        partial_push(heap, desc);
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_from_partial(procheap_t * heap) {
        desc_t * desc;
        desc_t ** addr;
        anchor_t oldanchor;
        anchor_t new_anchor;
        void * sb;
        unsigned morecredits;
        unsigned sz;

        TRACE(OP_PARTIAL_STR, 0, 0, 0, 0, heap, 0);
retry:
        desc = partial_pop(heap);
        if (!desc)
            return NULL;
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            if (!oldanchor.sep.valid) {
                DESC_RETIRE(desc);
                goto retry;
            }
            morecredits = MIN(oldanchor.sep.count - 1, MAXCREDITS);
            new_anchor.sep.count -= morecredits + 1;
            new_anchor.sep.active = (morecredits > 0);
            new_anchor.sep.tag++; /* for debugging */
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        ASSERT(!oldanchor.sep.active);
        ASSERT(oldanchor.sep.count > 0);
        ASSERT(oldanchor.sep.count < desc->maxcount);
        sb = desc->SB;
        sz = heap->sc->sz;
        TRACE(OP_PARTIAL_RSRV, sz, 0, desc, sb, heap, new_anchor.all);
        do { /* pop block */
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            ASSERT(oldanchor.sep.valid);
            addr = sb + oldanchor.sep.head * sz + EIGHT;
            new_anchor.sep.head = *(unsigned *) addr;
            new_anchor.sep.tag++;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        TRACE(OP_PARTIAL_END, sz, addr, desc, sb, heap, new_anchor.all);
        if (morecredits > 0) { /* update Active */
            /* cannot check for new_anchor.active because someone else
            might have already made it active even though our morecredits
            is zero */
            ASSERT(new_anchor.sep.active);
            desc->Heap = heap;
            LWSYNC; /* wbw */
            WBW;
            make_active(heap, desc, morecredits);
        }
        return addr;
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_from_new__sb(procheap_t * heap) {
        desc_t ** addr;
        desc_t * desc;
        void * sb;
        ret_t ret;
        anchor_t new_anchor;
        ptrsize_t new_active;
        unsigned credits;

        desc = desc_alloc();
        desc->Heap = heap;
        sb_alloc(desc);
        credits = MIN(heap->sc->maxcount - 1, MAXCREDITS);
        new_active = (ptrsize_t) (desc)+(credits - 1);
        new_anchor.all = desc->Anchor.all;
        TRACE(OP_new__STR, desc->sz, 0, desc, desc->SB, heap, new_anchor.all);
        new_anchor.sep.head = 1;
        new_anchor.sep.count = (heap->sc->maxcount - 1) - credits;
        new_anchor.sep.active = TRUE;
        new_anchor.sep.valid = TRUE;
        new_anchor.sep.tag++; /* for debugging */
        desc->Anchor.all = new_anchor.all;
        desc->sz = heap->sc->sz;
        desc->maxcount = heap->sc->maxcount;
        LWSYNC; /* wbw */
        WBW;
        CAS(ret, &heap->Active, NULL, new_active);
        if (!ret) {
            sb_dealloc(desc->SB, desc->Pdesc);
            new_anchor.sep.valid = FALSE;
            desc->Anchor.all = new_anchor.all;
            TRACE(OP_NOT_new_, desc->sz, 0, desc, desc->SB, heap, new_anchor.all);
            DESC_RETIRE(desc);
            return NULL;
        }
        sb = desc->SB;
        addr = sb + EIGHT;
        TRACE(OP_new__END, heap->sc->sz, addr, desc, sb, heap, new_anchor.all);
        return addr;
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_regular(procheap_t * heap) {
        void ** addr;
        desc_t * desc;
        ptrsize_t oldactive;
        ptrsize_t new_active;
        anchor_t oldanchor;
        anchor_t new_anchor;
        void * sb;
        unsigned credits;
        unsigned morecredits = 0;

        TRACE(OP_MALLOC_STR, heap->sc->sz, 0, 0, 0, heap, 0);
        do {
            ret_t ret;
            oldactive = (ptrsize_t) (heap->Active);
            new_active = oldactive;
            if (!oldactive) {
                addr = malloc_from_partial(heap);
                if (addr) return addr;
                addr = malloc_from_new__sb(heap);
                if (addr) return addr;
                continue;
            }
            credits = oldactive & CREDITSMASK;
            if (credits == 0) {
                new_active = (ptrsize_t) NULL;
            } else {
                new_active--;
            }
            CAS(ret, &heap->Active, oldactive, new_active); /* no ABA */
            if (ret)
                break;
        } while (1);
        desc = (desc_t*) (oldactive - credits);
        sb = desc->SB;
        TRACE(OP_REGULAR_RSRV, desc->sz, 0, desc, sb, heap, desc->Anchor.all);
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            ASSERT(oldanchor.sep.valid);
            //ASSERT(oldanchor.sep.head < desc->maxcount);
            //ASSERT(oldanchor.sep.count < desc->maxcount);
            //addr = sb+oldanchor.sep.head*sc->sz+sizeof(desc_t*);
            addr = sb + oldanchor.sep.head * desc->sz + EIGHT;
            new_anchor.sep.head = *(unsigned *) addr;
            if (credits == 0) {
                ASSERT(oldanchor.sep.active);
                morecredits = MIN(oldanchor.sep.count, MAXCREDITS);
                new_anchor.sep.count -= morecredits;
                new_anchor.sep.active = (morecredits > 0);
            }
            new_anchor.sep.tag++;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        TRACE(OP_REGULAR_END, desc->sz, addr, desc, sb, heap, new_anchor.all);
        if (morecredits > 0)
            make_active(heap, desc, morecredits);
        return addr;
    }
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void __free__(void * ptr)
#else

void free(void * ptr)
#endif
    {
        desc_t ** header;
        desc_t * desc;
        anchor_t oldanchor;
        anchor_t new_anchor;
        ptrsize_t num;
        void * sb;
        procheap_t * heap;
        desc_t * pdesc;
        size_t sz;

        if (!ptr)
            return;
        header = ptr - EIGHT;
        desc = *header;
        num = (ptrsize_t) desc;
        if (num & 1) {
            /* Big block */
            num--;
            if (munmap((void *) header, num)) {
                perror("free munmap failed\n");
            }
            return;
        }
        sb = desc->SB;
        heap = desc->Heap;
        //sz = heap->sc->sz;
        sz = desc->sz;
        TRACE(OP_FREE_STR, sz, header, desc, sb, heap, desc->Anchor.all);
        /* Small block */
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            ASSERT(oldanchor.sep.valid);
            *(unsigned *) ptr = oldanchor.sep.head;
            /* need fence before SC */
            LWSYNC; /* wbw */
            WBW;
            new_anchor.sep.head = ((ptr - sb) / sz);
            if ((!oldanchor.sep.active) &&
                    //(oldanchor.sep.count == heap->sc->maxcount-1)) {
                    (oldanchor.sep.count == desc->maxcount - 1)) {
                new_anchor.sep.valid = FALSE;
                pdesc = desc->Pdesc;
                if (pdesc == (void*) 1) continue; /* rbw */
                RBW;
            } else
                new_anchor.sep.count++;
            new_anchor.sep.tag++; /* for debugging */
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        TRACE(OP_FREE_END, sz, header, desc, sb, heap, new_anchor.all);
        if (!new_anchor.sep.valid) {
            sb_dealloc(sb, pdesc);
            list_desc_remove(heap);
        } else if ((!new_anchor.sep.active) && (new_anchor.sep.count == 1)) {
            LWSYNC; /* wbw */
            WBW;
            partial_push(heap, desc);
        }
    }
    /*---------------------------------------------------------------------------*/
#else /* 2005 */

/*---------------------------------------------------------------------------*/
    static void make_active(procheap_t * heap, desc_t* desc) {
        anchor_t oldanchor;
        anchor_t new_anchor;
        ret_t ret;

        CAS(ret, &heap->Active, NULL, desc);
        if (ret)
            return;
        /*
          Someone installed another active sb.
          Return credits and make sb partial
         */
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            new_anchor.sep.count++;
            new_anchor.sep.active = FALSE;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        LWSYNC; /* wbw */
        WBW;
        partial_push(heap, desc);
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_from_partial(procheap_t * heap) {
        desc_t * desc;
        desc_t ** addr;
        anchor_t oldanchor;
        anchor_t new_anchor;
        void * sb;
        unsigned sz;

retry:
        desc = partial_pop(heap);
        if (!desc)
            return NULL;
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            if (!oldanchor.sep.valid) {
                DESC_RETIRE(desc);
                goto retry;
            }
            ASSERT(!oldanchor.sep.active);
            ASSERT(oldanchor.sep.count > 0);
            ASSERT(oldanchor.sep.count < desc->maxcount);
            sb = desc->SB;
            sz = heap->sc->sz;
            addr = (desc_t**) ((char*) sb + oldanchor.sep.head * sz + EIGHT);
            new_anchor.sep.head = *(unsigned *) addr;
            if (oldanchor.sep.count > 1) {
                new_anchor.sep.count -= 2;
                new_anchor.sep.active = TRUE;
            } else {
                ASSERT(oldanchor.sep.count == 1);
                new_anchor.sep.count = 0;
            }
            new_anchor.sep.tag++;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        /* update Active */
        if (new_anchor.sep.active) {
            desc->Heap = heap;
            LWSYNC; /* wbw */
            WBW;
            make_active(heap, desc);
        }
        return addr;
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_from_new__sb(procheap_t * heap) {
        desc_t ** addr;
        desc_t * desc;
        void * sb;
        ret_t ret;
        anchor_t new_anchor;

        desc = desc_alloc();
        desc->Heap = heap;
        sb_alloc(desc);
        new_anchor.all = desc->Anchor.all;
        new_anchor.sep.head = 1;
        new_anchor.sep.count = heap->sc->maxcount - 2;
        new_anchor.sep.active = TRUE;
        new_anchor.sep.valid = TRUE;
        desc->Anchor.all = new_anchor.all;
        desc->sz = heap->sc->sz;
        desc->maxcount = heap->sc->maxcount;
        LWSYNC; /* wbw */
        WBW;
        CAS(ret, &heap->Active, NULL, desc);
        if (!ret) {
            sb_dealloc(desc->SB, desc->Pdesc);
            new_anchor.sep.valid = FALSE;
            desc->Anchor.all = new_anchor.all;
            DESC_RETIRE(desc);
            return NULL;
        }
        sb = desc->SB;
        addr = (desc_t**) ((char*) sb + EIGHT);
        return addr;
    }

    /*---------------------------------------------------------------------------*/
    static void * malloc_regular(procheap_t * heap) {
        void ** addr;
        desc_t * desc;
        anchor_t oldanchor;
        anchor_t new_anchor;
        void * sb;
        ret_t ret;
        aret_t aret;

        do {
            desc = heap->Active;
            if (!desc) {
                addr = (void**) malloc_from_partial(heap);
                if (addr) break;
                addr = (void**) malloc_from_new__sb(heap);
                if (addr) break;
                continue;
            }
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            if (oldanchor.sep.count == 0) {
                CAS(ret, &heap->Active, desc, NULL);
                if (!ret) continue;
                do { /* malloc last */
                    LLA(oldanchor.all, &desc->Anchor.all);
                    new_anchor.all = oldanchor.all;
                    ASSERT(oldanchor.sep.active);
                    /* no need for RBR */
                    sb = desc->SB;
                    addr = (void**) ((char*) sb + oldanchor.sep.head * desc->sz + EIGHT);
                    new_anchor.sep.head = *(unsigned *) addr;
                    if (oldanchor.sep.count == 0)
                        new_anchor.sep.active = FALSE;
                    else
                        new_anchor.sep.count--;
                    new_anchor.sep.tag++;
                    SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
                    if (aret)
                        break;
                } while (1);
                if (new_anchor.sep.active)
                    make_active(heap, desc);
                break;
            }
            /* malloc from active */
            ISYNC; /* rbr */
            RBR;
            sb = desc->SB;
            addr = (void**) ((char*) sb + oldanchor.sep.head * desc->sz + EIGHT);
            if (heap->Active != desc) continue;
            if (!sb) continue; /* rbr */
            ISYNC; /* rbr */
            RBR;
            if (desc->Anchor.all != oldanchor.all) continue;
            ASSERT(oldanchor.sep.valid); /* failed ******/
            ASSERT(oldanchor.sep.active);
            new_anchor.sep.head = *(unsigned *) addr;
            new_anchor.sep.count--;
            new_anchor.sep.tag++;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret) {
                break;
            }
        } while (1);
        return addr;
    }
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void __free__(void * ptr)
#else

void free(void * ptr)
#endif
    {
        desc_t ** header;
        desc_t * desc;
        anchor_t oldanchor;
        anchor_t new_anchor;
        ptrsize_t num;
        void * sb;
        procheap_t * heap;
        desc_t * pdesc;
        unsigned sz;

        if (!ptr)
            return;
        header = (desc_t**) ((char*) ptr - EIGHT);
        desc = *header;
        num = (ptrsize_t) desc;
        if (num & 1) {
            /* Big block */
            num--;
            if (munmap((void *) header, num)) {
                perror("free munmap failed\n");
            }
            return;
        }
        sb = desc->SB;
        heap = desc->Heap;
        sz = desc->sz;
        //    sz = heap->sc->sz;
        /* Small block */
        do {
            aret_t aret;
            LLA(oldanchor.all, &desc->Anchor.all);
            new_anchor.all = oldanchor.all;
            ASSERT(oldanchor.sep.valid);
            *(unsigned *) ptr = oldanchor.sep.head;
            /* need WBW fence before SC */
            LWSYNC; /* WBW */
            WBW;
            new_anchor.sep.head = (((char*) ptr - (char*) sb) / sz);
            if ((!oldanchor.sep.active) &&
                    (oldanchor.sep.count == desc->maxcount - 1)) {
                //	    (oldanchor.sep.count == heap->sc->maxcount-1)) {
                new_anchor.sep.valid = FALSE;
                pdesc = desc->Pdesc;
                if (pdesc == (void*) 1) continue; /* RBW */
                RBW;
            } else
                new_anchor.sep.count++;
            SCA(aret, &desc->Anchor.all, oldanchor.all, new_anchor.all);
            if (aret)
                break;
        } while (1);
        if (!new_anchor.sep.valid) {
            sb_dealloc(sb, pdesc);
            list_desc_remove(heap);
        } else if ((!new_anchor.sep.active) && (new_anchor.sep.count == 1)) {
            LWSYNC; /* wbw */
            WBW;
            partial_push(heap, desc);
        }
    }
    /*---------------------------------------------------------------------------*/
#endif /* PLDI2004 */
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void * __malloc__(size_t sz)
#else

void * malloc(size_t sz)
#endif
    {
        procheap_t * heap;
        sizeclass_t * sc;
        void ** addr;
        unsigned ind;
        unsigned heapid;

#if defined(PPC)
        /* __malloc_start__ is guaranteed to have been called */
#else
        /* Check for initialization */
        if (controlblock == NULL) clfmalloc_init();
#endif

        /* find sizeclass */
        sz += EIGHT;
        /* find procheap */
        for (ind = 0; ind < NUMSIZECLASSES; ind++) {
            if (sz <= scsz[ind])
                break;
        }
        if (ind == NUMSIZECLASSES) {
            /* Block > 64KB */
            if (sz & 7)
                sz = sz + 8 - (sz & 7);
            if ((addr = (void **) MMAP(sz)) == MAP_FAILED) {
                perror("malloc mmap failed\n");
            }
            ASSERT(addr);
            *addr = (void*) (sz + 1);
            LWSYNC; /* wbw - not needed in practice */
            WBW;
            return (void *) ((ptrsize_t) (addr) + EIGHT);
        }
        /* Block <= 64 KB */
        sc = &controlblock->sizeclass[ind];
        if (sc->numprocheaps == 1) {
            heap = &sc->procheap;
        } else {
            {
                unsigned id, x;
#if defined(SPARC)
                id = thr_self();
#else
                id = (unsigned long) pthread_self();
#endif
                x = id & 0x7ff;
                x ^= (id >> 11) & 0x7ff;
                x ^= (id >> 22) & 0x7ff;
                //	    heapid = x % sc->numprocheaps;
                heapid = x & heapidmask;
            }
            heap = &controlblock->procheap[heapid][ind];
        }
        /* regular block */
        return malloc_regular(heap);
    }

    /*---------------------------------------------------------------------------*/
    static void * super_malloc(unsigned sz) {
        return malloc(sz);
    }

    /*---------------------------------------------------------------------------*/
    static void super_free(void * ptr) {
        free(ptr);
    }
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void * __calloc__(size_t n, size_t sz)
#else

void * calloc(size_t n, size_t sz)
#endif
    {
        void * addr = malloc(n * sz);
        memset(addr, 0, n * sz);
        return addr;
    }
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
    void * __realloc__(void * ptr, size_t sz)
#else

void * realloc(void * ptr, size_t sz)
#endif
    {
        desc_t ** header;
        size_t copysz;
        void * addr;
        desc_t * desc;
        ptrsize_t num;

        /* realloc zero frees */
        if (sz == 0) {
            free(ptr);
            return NULL;
        }
        /* find old size */
        if (!ptr)
            return malloc(sz);
        header = (desc_t**) (reinterpret_cast<char*> (ptr) - EIGHT);
        desc = *header;
        num = (ptrsize_t) desc;
        if (num & 1) {
            /* Big block */
            num--;
            num -= EIGHT;
            addr = malloc(sz);
            copysz = MIN(num, sz);
            memcpy(addr, ptr, copysz);
            free(ptr);
            return addr;
        }
        /* regular block */
        copysz = desc->sz - EIGHT;
        /* see if same block can accomodate new_ size */
        if (sz <= copysz)
            return ptr;
        /* need to reallocate */
        addr = malloc(sz);
        memcpy(addr, ptr, copysz);
        free(ptr);
        return addr;
    }
    /*---------------------------------------------------------------------------*/
#if defined(PPC)
#include <malloc.h>
#include <errno.h>

int __mallopt__(int cmd, int value) {
        errno = ENOSYS;
        perror("mallopt not supported");
        return 1;
    }
    /*---------------------------------------------------------------------------*/
    static struct mallinfo mi;

    struct mallinfo __mallinfo__() {
        errno = ENOSYS;
        perror("mallinfo not supported");
        return mi;
    }

    /*---------------------------------------------------------------------------*/
    int __posix_memalign__(void **p2p, size_t align, size_t sz) {
        errno = ENOSYS;
        perror("posix_memalign not supported");
        *p2p = NULL;
        return ENOSYS;
    }

    /*---------------------------------------------------------------------------*/
    void __malloc_init__() {
    }

    /*---------------------------------------------------------------------------*/
    void __malloc_prefork_lock__() {
    }

    /*---------------------------------------------------------------------------*/
    void __malloc_postfork_unlock__() {
    }
    /*---------------------------------------------------------------------------*/
#endif



#ifdef	__cplusplus
}
#endif

#endif	/* CLMALLOC_H */
