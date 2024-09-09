
target_include_directories("${PROJECT_NAME}"
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Components
	${CMAKE_CURRENT_SOURCE_DIR}/GUICustom)

add_subdirectory(Components)
add_subdirectory(GUICustom)
