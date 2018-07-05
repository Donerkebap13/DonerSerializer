cmake_minimum_required(VERSION 3.6)
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)
set(CURRENT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)
include(CheckCXXSourceCompiles)

set(project_name "DonerSerializer")
set(ide_group "${project_name}")

project("${project_name}")

message(STATUS "${project_name} version ${CURRENT_VERSION}")

file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_source_groups("${header_files}")

include_directories("include")

# Library installation directory
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib)
endif()
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

add_library("${project_name}" INTERFACE)
target_sources("${project_name}" INTERFACE "${header_files}")

target_include_directories("${project_name}" INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(install_libs "${project_name}")
set_property(TARGET "${project_name}" APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

#set_target_properties ("${project_name}" PROPERTIES FOLDER "${ide_group}")

target_compile_features("${project_name}" INTERFACE cxx_auto_type)
target_compile_features("${project_name}" INTERFACE cxx_nullptr)
target_compile_features("${project_name}" INTERFACE cxx_static_assert)
target_compile_features("${project_name}" INTERFACE cxx_decltype)
target_compile_features("${project_name}" INTERFACE cxx_constexpr)
target_compile_features("${project_name}" INTERFACE cxx_sizeof_member)
target_compile_features("${project_name}" INTERFACE cxx_variadic_templates)
target_compile_features("${project_name}" INTERFACE cxx_rvalue_references)
target_compile_features("${project_name}" INTERFACE cxx_long_long_type)
target_compile_features("${project_name}" INTERFACE cxx_lambdas)
target_compile_features("${project_name}" INTERFACE cxx_func_identifier)

if(DONER_ENABLE_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
	enable_testing()
	
	set(gtest_root "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest/1.7.0")
	add_subdirectory("${gtest_root}" "${CMAKE_BINARY_DIR}/gtest")
	
	set(gtest_ide_group "GoogleTest")
	set_target_properties ("gtest" PROPERTIES FOLDER "${gtest_ide_group}")

	set(tests_project_name "${project_name}_tests")
	project("${tests_project_name}")
	
	file(GLOB_RECURSE test_header_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/include/*.h")
	file(GLOB_RECURSE test_source_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/source/*.cpp")

	message(STATUS "test_source_files ${test_source_files}" )
	
	add_source_groups("${test_header_files}")
	add_source_groups("${test_source_files}")
	
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/tests/include")		
	include_directories("${gtest_root}/include")
	
	add_executable ("${tests_project_name}" "${test_header_files}" "${test_source_files}")
	
	set_target_properties("${tests_project_name}" PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties ("${tests_project_name}" PROPERTIES FOLDER "${ide_group}/tests")
	
	target_link_libraries("${tests_project_name}" "${project_name}" "gtest")
	
	if (COVERAGE)
		if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
			message(STATUS "Building ${tests_project_name} with coverage")
			target_compile_options("${tests_project_name}" PRIVATE --coverage)
			target_link_libraries("${tests_project_name}" "--coverage")
		else()
			message(STATUS "Coverage not active for ${CMAKE_CXX_COMPILER_ID}")
		endif()
	endif()
		
	add_test("${tests_project_name}" "${tests_project_name}")
	
	set_compile_flags("${tests_project_name}")
endif()

if (NOT WINDOWS OR CYGWIN)
    set(DonerSerializer_libs -lDonerSerializer)

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/DonerSerializer.pc
        DESTINATION "${libdir}/pkgconfig"
        )
endif()

install(
	DIRECTORY "${project_name}"
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
	)

install(
	TARGETS "${install_libs}"
	LIBRARY DESTINATION "${libdir}"
	ARCHIVE DESTINATION "${libdir}"
	RUNTIME DESTINATION "bin"
	)
	