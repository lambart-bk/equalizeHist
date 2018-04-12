#set(OpenCV_DIR /usr/local/opencv3.1/share/OpenCV)
find_package(OpenCV  REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} )

add_executable(equalizehist equalizehist.cpp)
target_link_libraries(equalizehist ${OpenCV_LIBRARIES} )
