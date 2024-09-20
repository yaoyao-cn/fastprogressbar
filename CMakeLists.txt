cmake_minimum_required(VERSION 3.14)

project(fastprogressbar)

add_executable(fastprogressbar 
	"main.cpp"
	"include/fastprogressbar.h"
)

target_include_directories(fastprogressbar PUBLIC "include")

target_compile_features(fastprogressbar PUBLIC cxx_std_17)