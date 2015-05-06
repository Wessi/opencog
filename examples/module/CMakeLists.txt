# The atom_types.h file is written to the build directory
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}
                    ${ODBC_INCLUDE_DIRS}
)

ADD_LIBRARY (example_module SHARED
       ExampleModule
)
ADD_LIBRARY (rosagent SHARED
       RosAgent
)


