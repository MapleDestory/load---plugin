#ifndef SDK
#define SDK

#ifndef SDK_API
#define SDK_API
#endif

#ifndef SDK_IMPL_API
#define SDK_IMPL_API	extern "C" __declspec(dllexport)
#endif

SDK_IMPL_API int SDK_Impl_Instance(void);

#endif