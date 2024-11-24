add_executable (waf_task  DNSCache.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET waf_task PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(waf_task PUBLIC "${PROJECT_SOURCE_DIR}/waf_task/include/")
