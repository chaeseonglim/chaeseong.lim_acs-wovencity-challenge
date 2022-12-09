set(CMAKE_CXX_STANDARD 17)

set(SRCS
    ProbSync.cpp
    PetersonSync.cpp
    TestSetSync.cpp
    )

set(HEADERS_DIR
    ${PROJECT_SOURCE_DIR}/include/Challenge
    )

set(HEADERS_PUB
    ${HEADERS_DIR}/Lock.hpp
    ${HEADERS_DIR}/Lock/Lock.hpp
    ${HEADERS_DIR}/Lock/SyncObject.hpp
    ${HEADERS_DIR}/Lock/ProbSync.hpp
    ${HEADERS_DIR}/Lock/PetersonSync.hpp
    ${HEADERS_DIR}/Lock/TestSetSync.hpp
    )

add_library(Lock SHARED ${SRCS} ${HEADERS_PUB} )

target_include_directories(Lock
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_compile_options(Lock
    PRIVATE -O2
    )

install(TARGETS Lock DESTINATION lib)
install(FILES ${HEADERS_PUB} DESTINATION include/Lock)

add_library(Challenge::Lock ALIAS Lock)
