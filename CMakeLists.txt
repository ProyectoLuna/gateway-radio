cmake_minimum_required (VERSION 2.8.1)

######## Project settings ########
PROJECT(proyecto-luna)
SET(LICENSE "TBD")

######## Build and include settings ########
include_directories(
	inc
)

link_directories(
	${LINK_DIRECTORIES}
)


file(GLOB SOURCES
	"src/*.cpp"
)

add_executable(
	proyecto-luna

	${SOURCES}
)

TARGET_LINK_LIBRARIES(
	proyecto-luna
	rf24
	rf24network
	rf24mesh
)

######## Install targets ########
INSTALL(TARGETS proyecto-luna
	RUNTIME DESTINATION usr/bin
)
