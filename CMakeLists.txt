cmake_minimum_required(VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("MentalEngine")

add_executable(MentalEngine 
  "SourceFiles/MentalEngine.cpp" 
  "Include/MentalEngine.h"    
  "Include/Window.h" 
  "SourceFiles/video/Window.cpp" 
  "Include/NumbersLangDef.h" 
  #"SourceFiles/graphics/Object.cpp" 
  #"Include/Object.h" 
  "Include/ObjectManager.h" 
  "SourceFiles/graphics/ObjectManager.cpp" 
  "SourceFiles/video/Ui.cpp" 
  "Include/Ui.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MentalEngine PROPERTY CXX_STANDARD 20)
endif()

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)

# Fetch ImGui with docking branch
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)

FetchContent_MakeAvailable(glfw imgui)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

# Set paths for GLEW based on your project structure
set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dependencies/glew/include")
set(GLEW_LIB_DIR "${CMAKE_SOURCE_DIR}/Dependencies/glew/lib/Release/x64")

# Check if the GLEW include directory exists
if (NOT EXISTS "${GLEW_INCLUDE_DIR}")
    message(FATAL_ERROR "GLEW include directory not found: ${GLEW_INCLUDE_DIR}")
endif()

# Check if the GLEW library directory exists
if (NOT EXISTS "${GLEW_LIB_DIR}")
    message(FATAL_ERROR "GLEW library directory not found: ${GLEW_LIB_DIR}")
endif()

# Add GLEW include directory
target_include_directories(MentalEngine PRIVATE ${GLEW_INCLUDE_DIR} ${glfw_SOURCE_DIR}/include)

# Link GLEW and OpenGL libraries
target_link_directories(MentalEngine PRIVATE ${GLEW_LIB_DIR})
target_link_libraries(MentalEngine PRIVATE glfw glew32s opengl32)

# Define GLEW_STATIC if using the static version of GLEW
target_compile_definitions(MentalEngine PRIVATE GLEW_STATIC)

# Add ImGui sources
file(GLOB IMGUI_SOURCES 
  "${imgui_SOURCE_DIR}/*.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Add ImGui include directory
target_include_directories(MentalEngine PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

# Add ImGui sources to the executable
target_sources(MentalEngine PRIVATE ${IMGUI_SOURCES})

message(STATUS "GLEW include directory: ${GLEW_INCLUDE_DIR}")
message(STATUS "GLEW library directory: ${GLEW_LIB_DIR}")
message(STATUS "ImGui source directory: ${imgui_SOURCE_DIR}")