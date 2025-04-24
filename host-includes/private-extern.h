#if ! defined(__private_extern__)

# if defined(__MWERKS__)
#    define __private_extern__ __declspec(private_extern)
#  endif

# if defined(__GNUC__)
#    define __private_extern__ extern __attribute__((visibility("hidden")))
#  endif

#endif
