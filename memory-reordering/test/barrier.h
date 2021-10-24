#ifndef __BARRIER_H__
#define __BARRIER_H__

// 1 -> compiler barrier; 2 -> hardware barrier; others -> noop
#define BARRIER_TYPE 0

#ifdef _MSC_VER
  #define WIN32_LEAD_AND_MEAN
  #include <windows.h>
  #include <intrin.h>
  #if (BARRIER_TYPE==1)
    #define SOME_BARRIER() _ReadWriteBarrier()                   // compiler barrier
  #elif (BARRIER_TYPE==2)
    #define SOME_BARRIER() MemoryBarrier()                       // hardware barrier
  #else
    #define SOME_BARRIER()                                       // noop
  #endif
#else
  #if (BARRIER_TYPE==1)
    #define SOME_BARRIER() asm volatile("" ::: "memory")         // compiler barrier
  #elif (BARRIER_TYPE==2)
    #define SOME_BARRIER() asm volatile("mfence" ::: "memory")   // hardware barrier
  #else
    #define SOME_BARRIER()                                       // noop
  #endif
#endif

#endif // !__BARRIER_H__
