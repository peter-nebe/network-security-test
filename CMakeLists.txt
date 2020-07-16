cmake_minimum_required (VERSION 3.17)
project (network-security-test)

set (project_SRC
  src/dosTest.cpp
  src/downloadTest.cpp
  src/main.cpp
  src/portTest.cpp
  src/signatureTest.cpp
  src/system.cpp
  src/testSuite.cpp
)
link_libraries (
  pthread
)
set (CMAKE_CXX_STANDARD
  17
)
set (CMAKE_CXX_STANDARD_REQUIRED
  ON
)
add_executable (
  ${PROJECT_NAME}
  ${project_SRC}
)
