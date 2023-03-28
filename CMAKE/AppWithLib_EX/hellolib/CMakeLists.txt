add_library(HelloLib SHARED src/hello.cpp include/hello.h)
target_include_directories(HelloLib PUBLIC include)