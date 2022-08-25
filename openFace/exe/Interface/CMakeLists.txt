# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})
# link_libraries(${OpenCV_LIBS} ${OpenBLAS_LIB})


set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/interlib/")

add_compile_options(-fPIC)

add_library(interface SHARED auInterface.cpp)
target_link_libraries(interface LandmarkDetector)
target_link_libraries(interface FaceAnalyser)
target_link_libraries(interface ${OpenCV_LIBS})
install (TARGETS interface DESTINATION interlib)

