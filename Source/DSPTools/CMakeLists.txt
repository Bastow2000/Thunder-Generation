
target_include_directories("${PROJECT_NAME}"
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Filter's
	${CMAKE_CURRENT_SOURCE_DIR}/AudioEffects
	${CMAKE_CURRENT_SOURCE_DIR}/OtherTools)

add_subdirectory(Filter's)
add_subdirectory(AudioEffects)
add_subdirectory(OtherTools)