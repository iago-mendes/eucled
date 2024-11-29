# Distributed under the MIT License.
# See LICENSE.txt for details.

# Specify the Python interpreter
find_package(Python3 REQUIRED)
message(STATUS "Python3 executable found: ${Python3_EXECUTABLE}")

# Copy files to the build directory
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__init__.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/__init__.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py"
)
add_custom_command(


  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/embed.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/embed.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/embed.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/embed.py"
)
add_custom_command(
  OUTPUT "${EXECUTABLE_OUTPUT_PATH}/eucled"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/eucled"
                                   "${EXECUTABLE_OUTPUT_PATH}/eucled"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/eucled"
)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/eucled.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/eucled.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/eucled.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/eucled.py"
)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/interpolate.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/interpolate.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/interpolate.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/interpolate.py"
)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/run.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/run.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/run.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/run.py"
)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/test.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test.py"
                                   "${CMAKE_CURRENT_BINARY_DIR}/test.py"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test.py"
)

# Create a custom target to "build" the CLI
add_custom_target(cli
  ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/__init__.py"
          "${CMAKE_CURRENT_BINARY_DIR}/embed.py"
          "${EXECUTABLE_OUTPUT_PATH}/eucled"
          "${CMAKE_CURRENT_BINARY_DIR}/eucled.py"
          "${CMAKE_CURRENT_BINARY_DIR}/interpolate.py"
          "${CMAKE_CURRENT_BINARY_DIR}/run.py"
          "${CMAKE_CURRENT_BINARY_DIR}/test.py"
)
