# CMakeList.txt : CMake project for TNCG15, include source and define
# project specific logic here.
#


# Add source to this project's executable.


add_executable(TNCG15 src/main.cpp src/Camera.cpp src/TNCG15.cpp  src/Triangle.cpp src/Polygon.cpp src/Ray.cpp src/Rectangle.cpp src/Camera.h src/Triangle.h src/TNCG15.h src/Polygon.h src/Ray.h src/Rectangle.h src/stb_image_write.h src/Sphere.cpp src/Sphere.h src/Tetrahedron.cpp src/Tetrahedron.h src/Photon.cpp src/Photon.h src/kdtree.hpp src/kdtree.cpp src/kdtree_homemade.h src/kdtree_homemade.cpp)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

target_link_libraries(TNCG15 PRIVATE glm::glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TNCG15 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
function(enable_warnings target)
    target_compile_options(${target} PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4                 # Enable the highest warning level
            /w44388             # eneble 'signed/unsigned mismatch' '(off by default)
            /we4715             # turn 'not all control paths return a value' into a compile error
            /permissive-        # Stick to the standard
			/fsanitize=address  # Enable the Address Sanatizer, helps finding bugs at runtime
            >
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
    )
endfunction()


