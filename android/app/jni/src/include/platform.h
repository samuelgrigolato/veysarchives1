
/*
  The line below solves the following compilation problem:
    /home/samuel/Android/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/9.0.9/include/mmintrin.h:525:12: error: invalid conversion between vector type '__m64' (vector of 1 'long long' value) and integer type 'int' of different size
      return (__m64)__builtin_ia32_psubw((__v4hi)__m1, (__v4hi)__m2);
*/
#define SDL_DISABLE_IMMINTRIN_H 1

#define PLATFORM_SDL_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP
#define ASSETS_BASE_DIR ""
#define DESKTOP 0
