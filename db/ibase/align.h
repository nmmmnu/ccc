#ifdef VMS
#define ALIGN(n,b)              (n)
#endif

#ifdef sun
#ifdef sparc
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif
#endif

#ifdef hpux
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#ifdef ultrix
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#ifdef sgi
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#ifdef _AIX
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif
 
#ifdef __m88k__
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif
 
#if (defined __osf__ && defined __alpha)
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#ifdef mpexl
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#if (defined(_MSC_VER) && defined(WIN32)) || (defined(__BORLANDC__) \
    && defined(__WIN32__))
#define ALIGN(n,b)          ((n + b - 1) & ~(b - 1))
#endif

#ifndef ALIGN
#define ALIGN(n,b)          ((n+1) & ~1)
#endif
