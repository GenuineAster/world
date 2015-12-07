# - Try to find Jsoncpp
# Once done, this will define
#
#  JSONCPP_FOUND - system has Jsoncpp
#  JSONCPP_INCLUDE_DIRS - the Jsoncpp include directories
#  JSONCPP_LIBRARIES - link these to use Jsoncpp

# Include dir
find_path(JSONCPP_INCLUDE_DIR
  NAMES json/features.h
  PATHS
  /usr/include/
  /usr/local/include/
  /usr/include/json
)

# Finally the library itself
find_library(JSONCPP_LIBRARY
  NAMES jsoncpp
  PATHS
  /usr/lib
  /usr/local/lib
)

if(${JSONCPP_LIBRARY})
	if(${JSONCPP_INCLUDE_DIR})
		set(JSONCPP_FOUND TRUE)
	endif()
endif()
