include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(MODULE_NAME ${DIR} NAME)
message("MODULE_NAME: -> ${MODULE_NAME}")

set(UNITY_EXTENSION_FIXTURE ON CACHE INTERNAL "Set UNITY_EXTENSION_FIXTURE CACHE INTERNAL.")
set(UNITY_EXTENSION_MEMORY ON CACHE INTERNAL "Set UNITY_EXTENSION_MEMORY CACHE INTERNAL.")

if(MODULE_NAME STREQUAL "crf")
  set(UNITY_PACKAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../deps/unity)
else()
  set(UNITY_PACKAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../deps/unity)
endif()

if (EXISTS ${UNITY_PACKAGE_PATH})
  set(FETCHCONTENT_FULLY_DISCONNECTED ON)
  message("Unity is already polulated. Skipping download.")
else()
  message("Unity is not yet populated. Downloading...")
endif()

FetchContent_Declare(
  Unity
  GIT_REPOSITORY  https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG         origin/master
  SOURCE_DIR      ${UNITY_PACKAGE_PATH}
  FIND_PACKAGE_ARGS
  )

# Fetch the content using previously declared details
FetchContent_MakeAvailable( Unity )
