# CMake cross-compile toolchain file for the PS1.

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)
set(CMAKE_CROSSCOMPILING   1)

set(CMAKE_C_COMPILER   /compilers/ps1/gcc2.6.0-mipsel/gcc)
set(CMAKE_CXX_COMPILER /compilers/ps1/gcc2.6.0-mipsel/g++)
#set(CMAKE_OBJCOPY      )
#set(CMAKE_OBJDUMP      )
#set(CMAKE_LINKER       )