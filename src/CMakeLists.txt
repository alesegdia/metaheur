
# SET SOURCES AND STUFF
file(GLOB_RECURSE CPPs problema/*.cpp util/*.cpp algoritmo/*.cpp)
file(GLOB_RECURSE HPPs *.h)

add_executable(qap test/qaptest.cpp ${CPPs} ${HPPs})
add_executable(tsp test/tsptest.cpp ${CPPs} ${HPPs})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY})

