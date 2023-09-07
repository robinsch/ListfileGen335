file(GLOB PROJECT_SRC 
	"*.h"
	"*.cpp"
)

add_library(
	${PROJECT_NAME} SHARED ${PROJECT_SRC}
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/deps
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
		Detours
)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
		APP_NAME="${PROJECT_NAME}"
)
