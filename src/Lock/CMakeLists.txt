set(CMAKE_CXX_STANDARD 17)

add_library(Lock SHARED)

set(SRCS
    Dummy.cpp
    )

set(HEADERS_DIR
    ${PROJECT_SOURCE_DIR}/include/Challenge/Lock
    )

set(HEADERS_PUB
    ${HEADERS_DIR}/Lock.hpp
    )

target_sources(Lock PUBLIC
    PUBLIC  ${HEADERS_PUB}
    PRIVATE ${SRCS}
    )

target_include_directories(Lock
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_compile_options(Lock
    PRIVATE -O2
    )

#target_link_libraries(Lock
#    PUBLIC ${OpenCV_LIBS}
#           ${CVC_LIBRARIES}
#           ${HOGL_LIBRARIES}
#           ${SSHASH_LIBRARIES}
#           Boost::headers
#           Boost::filesystem
#           Boost::thread
#           Boost::random
#           cvc-common
#           cvc-math
#           cvc-cv
#           QRideDL  # from libadas
#    )

install(TARGETS Lock DESTINATION lib)
install(FILES ${HEADERS_PUB} DESTINATION include/Lock)

add_library(Challenge::Lock ALIAS Lock)
