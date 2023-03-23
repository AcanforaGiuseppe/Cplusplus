cmake_minimum_required(VERSION 3.18)
project(PrintVariables)
get_cmake_property(VarNames VARIABLES)
# KEY=VALUE
foreach(EachVar ${VarNames})
	message(STATUS "${EachVar}=${${EachVar}}")
endforeach()