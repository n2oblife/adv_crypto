cmake_minimum_required(VERSION 3.16)
project(CryptoImplementations VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Security and optimization flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
    
    # Security hardening flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
    
    # Timing attack mitigation (constant-time operations)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    
    # Additional security flags for production builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")
    endif()
endif()

if(MSVC)
    # MSVC-specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Include directories
include_directories(include)

# Options for building different components
option(BUILD_TESTS "Build test executables" ON)
option(BUILD_BENCHMARKS "Build benchmark executables" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)
option(ENABLE_OPENMP "Enable OpenMP for parallel computations" OFF)
option(ENABLE_TIMING_ATTACKS_PROTECTION "Enable additional timing attack protections" ON)

# Find OpenMP if enabled
if(ENABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(crypto_lib PUBLIC OpenMP::OpenMP_CXX)
        add_definitions(-DUSE_OPENMP)
    endif()
endif()

# Timing attack protection
if(ENABLE_TIMING_ATTACKS_PROTECTION)
    add_definitions(-DCONSTANT_TIME_OPERATIONS)
endif()

# TODO add auto finding files in folders

# Core Math Library
set(CORE_MATH_SOURCES
    src/core_math/BigInt.cpp
    src/core_math/ModularArithmetic.cpp
    src/core_math/FiniteField.cpp
    src/core_math/Fp.cpp
    src/core_math/F2n.cpp
    src/core_math/PolyArithmetic.cpp
    src/core_math/MultiPrecision.cpp
    src/core_math/PseudoMersenneReduction.cpp
)

set(CORE_MATH_HEADERS
    include/core_math/BigInt.h
    include/core_math/ModularArithmetic.h
    include/core_math/FiniteField.h
    include/core_math/Fp.h
    include/core_math/F2n.h
    include/core_math/PolyArithmetic.h
    include/core_math/MultiPrecision.h
    include/core_math/PseudoMersenneReduction.h
)

# Classical PKC Library
set(CLASSICAL_PKC_SOURCES
    src/classical_pkc/RSA.cpp
    src/classical_pkc/ElGamal.cpp
    src/classical_pkc/Rabin.cpp
    src/classical_pkc/MerkleHellman.cpp
)

set(CLASSICAL_PKC_HEADERS
    include/classical_pkc/RSA.h
    include/classical_pkc/ElGamal.h
    include/classical_pkc/Rabin.h
    include/classical_pkc/MerkleHellman.h
)

# Side Channel Countermeasures Library
set(SIDE_CHANNEL_SOURCES
    src/side_channel_countermeasures/MaskingPrimitives.cpp
    src/side_channel_countermeasures/SBoxEvaluation.cpp
    src/side_channel_countermeasures/MaskingConversion.cpp
    src/side_channel_countermeasures/RekeyingSchemes.cpp
)

set(SIDE_CHANNEL_HEADERS
    include/side_channel_countermeasures/MaskingPrimitives.h
    include/side_channel_countermeasures/SBoxEvaluation.h
    include/side_channel_countermeasures/MaskingConversion.h
    include/side_channel_countermeasures/RekeyingSchemes.h
)

# Modern Crypto Primitives Library
set(MODERN_CRYPTO_SOURCES
    src/modern_crypto_primitives/ChaCha20.cpp
    src/modern_crypto_primitives/Poly1305.cpp
    src/modern_crypto_primitives/AEAD.cpp
    src/modern_crypto_primitives/ECC.cpp
    src/modern_crypto_primitives/X25519.cpp
)

set(MODERN_CRYPTO_HEADERS
    include/modern_crypto_primitives/ChaCha20.h
    include/modern_crypto_primitives/Poly1305.h
    include/modern_crypto_primitives/AEAD.h
    include/modern_crypto_primitives/ECC.h
    include/modern_crypto_primitives/X25519.h
)

# Post Quantum Crypto Library
set(POST_QUANTUM_SOURCES
    src/post_quantum_crypto/KyberKem.cpp
    src/post_quantum_crypto/KyberArithmetic.cpp
)

set(POST_QUANTUM_HEADERS
    include/post_quantum_crypto/KyberKem.h
    include/post_quantum_crypto/KyberArithmetic.h
)

# Utilities Library
set(UTILS_SOURCES
    src/utils/PrimalityTests.cpp
    src/utils/FactorizationAlgs.cpp
    src/utils/RNG.cpp
)

set(UTILS_HEADERS
    include/utils/PrimalityTests.h
    include/utils/FactorizationAlgs.h
    include/utils/RNG.h
)

# Combine all sources and headers
set(ALL_SOURCES
    ${CORE_MATH_SOURCES}
    ${CLASSICAL_PKC_SOURCES}
    ${SIDE_CHANNEL_SOURCES}
    ${MODERN_CRYPTO_SOURCES}
    ${POST_QUANTUM_SOURCES}
    ${UTILS_SOURCES}
)

set(ALL_HEADERS
    ${CORE_MATH_HEADERS}
    ${CLASSICAL_PKC_HEADERS}
    ${SIDE_CHANNEL_HEADERS}
    ${MODERN_CRYPTO_HEADERS}
    ${POST_QUANTUM_HEADERS}
    ${UTILS_HEADERS}
)

# Create the main crypto library
add_library(crypto_lib STATIC ${ALL_SOURCES} ${ALL_HEADERS})

# Set target properties
set_target_properties(crypto_lib PROPERTIES
    OUTPUT_NAME "crypto"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Target include directories
target_include_directories(crypto_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional: Create separate libraries for each module
option(BUILD_MODULAR_LIBS "Build separate libraries for each crypto module" OFF)

if(BUILD_MODULAR_LIBS)
    # Core Math Library
    add_library(crypto_core_math STATIC ${CORE_MATH_SOURCES} ${CORE_MATH_HEADERS})
    target_include_directories(crypto_core_math PUBLIC include)
    
    # Classical PKC Library (depends on core_math)
    add_library(crypto_classical_pkc STATIC ${CLASSICAL_PKC_SOURCES} ${CLASSICAL_PKC_HEADERS})
    target_include_directories(crypto_classical_pkc PUBLIC include)
    target_link_libraries(crypto_classical_pkc PUBLIC crypto_core_math)
    
    # Side Channel Countermeasures Library
    add_library(crypto_side_channel STATIC ${SIDE_CHANNEL_SOURCES} ${SIDE_CHANNEL_HEADERS})
    target_include_directories(crypto_side_channel PUBLIC include)
    target_link_libraries(crypto_side_channel PUBLIC crypto_core_math)
    
    # Modern Crypto Primitives Library
    add_library(crypto_modern STATIC ${MODERN_CRYPTO_SOURCES} ${MODERN_CRYPTO_HEADERS})
    target_include_directories(crypto_modern PUBLIC include)
    target_link_libraries(crypto_modern PUBLIC crypto_core_math)
    
    # Post Quantum Crypto Library
    add_library(crypto_post_quantum STATIC ${POST_QUANTUM_SOURCES} ${POST_QUANTUM_HEADERS})
    target_include_directories(crypto_post_quantum PUBLIC include)
    target_link_libraries(crypto_post_quantum PUBLIC crypto_core_math)
    
    # Utils Library
    add_library(crypto_utils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})
    target_include_directories(crypto_utils PUBLIC include)
    target_link_libraries(crypto_utils PUBLIC crypto_core_math)
endif()

# Test executables
if(BUILD_TESTS)
    enable_testing()
    
    # Create test directory if it doesn't exist
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    
    # Function to add tests
    function(add_crypto_test test_name test_source)
        add_executable(${test_name} tests/${test_source})
        target_link_libraries(${test_name} PRIVATE crypto_lib)
        target_include_directories(${test_name} PRIVATE include)
        add_test(NAME ${test_name} COMMAND ${test_name})
        
        # Set test properties for better output
        set_tests_properties(${test_name} PROPERTIES
            TIMEOUT 30
            FAIL_REGULAR_EXPRESSION "FAIL|ERROR|SEGFAULT"
        )
    endfunction()
    
    # Add test files (create these as needed)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_bigint.cpp)
        add_crypto_test(test_bigint test_bigint.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_rsa.cpp)
        add_crypto_test(test_rsa test_rsa.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_chacha20.cpp)
        add_crypto_test(test_chacha20 test_chacha20.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_x25519.cpp)
        add_crypto_test(test_x25519 test_x25519.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_kyber.cpp)
        add_crypto_test(test_kyber test_kyber.cpp)
    endif()
endif()

# Benchmark executables
if(BUILD_BENCHMARKS)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
    
    function(add_crypto_benchmark benchmark_name benchmark_source)
        add_executable(${benchmark_name} benchmarks/${benchmark_source})
        target_link_libraries(${benchmark_name} PRIVATE crypto_lib)
        target_include_directories(${benchmark_name} PRIVATE include)
        
        # Optimization flags for benchmarks
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_compile_options(${benchmark_name} PRIVATE -O3 -march=native -DNDEBUG)
        endif()
    endfunction()
    
    # Add benchmark files (create these as needed)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/benchmark_bigint.cpp)
        add_crypto_benchmark(benchmark_bigint benchmark_bigint.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/benchmark_rsa.cpp)
        add_crypto_benchmark(benchmark_rsa benchmark_rsa.cpp)
    endif()
endif()

# Example programs
if(BUILD_EXAMPLES)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    
    function(add_crypto_example example_name example_source)
        add_executable(${example_name} examples/${example_source})
        target_link_libraries(${example_name} PRIVATE crypto_lib)
        target_include_directories(${example_name} PRIVATE include)
    endfunction()
    
    # Add example files (create these as needed)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/rsa_demo.cpp)
        add_crypto_example(rsa_demo rsa_demo.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/chacha20_demo.cpp)
        add_crypto_example(chacha20_demo chacha20_demo.cpp)
    endif()
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/x25519_demo.cpp)
        add_crypto_example(x25519_demo x25519_demo.cpp)
    endif()
endif()

# Installation rules
install(TARGETS crypto_lib
    EXPORT CryptoImplementationsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Install CMake config files
install(EXPORT CryptoImplementationsTargets
    FILE CryptoImplementationsTargets.cmake
    NAMESPACE CryptoImplementations::
    DESTINATION lib/cmake/CryptoImplementations
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CryptoImplementationsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CryptoImplementationsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/CryptoImplementations
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CryptoImplementationsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CryptoImplementationsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CryptoImplementationsConfigVersion.cmake"
    DESTINATION lib/cmake/CryptoImplementations
)

# Print configuration summary
message(STATUS "")
message(STATUS "CryptoImplementations Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build benchmarks: ${BUILD_BENCHMARKS}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Build modular libs: ${BUILD_MODULAR_LIBS}")
message(STATUS "  OpenMP support: ${ENABLE_OPENMP}")
message(STATUS "  Timing attack protection: ${ENABLE_TIMING_ATTACKS_PROTECTION}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")