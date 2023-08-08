cmake_minimum_required(VERSION 3.18)
project(quill-bug-report-bad-variant)

Include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#########################
# External dependencies #
#########################

FetchContent_Declare(
  quill
  GIT_REPOSITORY https://github.com/odygrd/quill.git
  GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(quill)

get_target_property(QUILL_INC quill INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(quill PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${QUILL_INC}")

add_executable(quill_bad_variant
  main.cc
)

target_link_libraries(quill_bad_variant
  PRIVATE
  quill::quill
)

target_compile_definitions(quill_bad_variant
  PUBLIC
  QUILL_USE_BOUNDED_QUEUE
)
