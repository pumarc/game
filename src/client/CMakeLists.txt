set(client_sources
        main.cpp
)

if (WIN32)
else ()
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

message(WARN ${URHO3D_HOME}/lib)
link_directories(${URHO3D_HOME}/lib)
add_executable(${PROJECT_NAME} ${client_sources})
target_link_libraries(${PROJECT_NAME} Urho3D ${X11_LIBRARIES} ${OPENGL_LIBRARIES} dl)