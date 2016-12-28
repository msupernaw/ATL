#ifndef ATL_PLATFORM
#define ATL_PLATFORM


#if defined(linux) || defined(__linux) || defined(__linux__)
#  define ATL_LINUX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#  define ATL_BSD
#elif defined(sun) || defined(__sun)
#  define ATL_SOLARIS
#elif defined(__sgi)
#  define ATL_IRIX
#elif defined(__hpux)
#  define ATL_HPUX
#elif defined(__CYGWIN__)
#  define ATL_CYGWIN
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define ATL_WIN32
#elif defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#  define ATL_WIN64
#elif defined(__BEOS__)
#  define ATL_BEOS
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#  define ATL_MACOS
#elif defined(__IBMCPP__) || defined(_AIX)
#  define ATL_AIX
#elif defined(__amigaos__)
#  define ATL_AMIGAOS
#elif defined(__QNXNTO__)
#  define ATL_QNXNTO
#endif

#if defined(ATL_WIN32) || defined(ATL_WIN64)
#  define ATL_WINDOWS
#endif


namespace atl{
    
    template <int alignment =16> void* AllocateAligned (std::size_t size);
	template <int alignment =16> void DeallocateAligned (void* ptr);

#if defined(__MACH__)
// on Mach-based systems i.e. Mac OS X, malloc always returns 16-byte aligned memory, but we don't guarantee arbitrary alignment, yet...
template <> inline void* AllocateAligned <16> (std::size_t size)	{ return std::malloc (size); }
template <> inline void DeallocateAligned <16> (void* ptr)			{ std::free (ptr); }
#elif defined(_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
// on Windows, we can get arbitrary aligned memory...
template <int alignment> void* AllocateAligned (std::size_t size)	{ return ::_mm_malloc (size, alignment); }
template <int alignment> void DeallocateAligned (void* ptr)		{ ::_mm_free (ptr); }
#elif defined(__linux__)
// on Linux-based systems, we also can get arbitrary aligned memory...
template <int alignment> void* AllocateAligned (std::size_t size)	{ return ::memalign (alignment, size); }
template <int alignment> void DeallocateAligned (void* ptr)		{ ::free (ptr); }
#endif
}

#endif