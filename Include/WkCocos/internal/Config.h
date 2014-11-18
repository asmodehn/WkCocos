

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define WKCOCOS_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define WKCOCOS_DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
#define WKCOCOS_DEPRECATED_ATTRIBUTE
#endif

