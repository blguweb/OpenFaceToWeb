# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
	
add_executable(AuDetection AuDetection.cpp)
target_link_libraries(AuDetection LandmarkDetector)
target_link_libraries(AuDetection FaceAnalyser)
# target_link_libraries(AuDetection GazeAnalyser)
# target_link_libraries(AuDetection Utilities)




install (TARGETS AuDetection DESTINATION bin)
