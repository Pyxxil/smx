include (FetchContent)

add_executable(server
    main.cpp

    logger.cpp logger.hpp

    http/common.hpp
    http/server.cpp http/server.hpp
    http/request.cpp http/request.hpp
    http/response.cpp http/response.hpp

    smtp/server.cpp smtp/server.hpp
    smtp/request.cpp smtp/request.hpp
    smtp/response.cpp smtp/response.hpp
)

FetchContent_Declare (
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
)

FetchContent_GetProperties (fmt)
if (NOT fmt_POPULATED)
  FetchContent_Populate (fmt)
  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif ()

find_package(Boost 1.76 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(server LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(server PRIVATE fmt)