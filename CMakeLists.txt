cmake_minimum_required(VERSION 3.1.0)

project(iGameMeshLab VERSION 1.0.0 LANGUAGES CXX)



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# QT编译器路径
set(QT_MSVC_PATH "C:/Qt/Qt5.14.2/5.14.2/msvc2017_64") #笔记本的路径
#set(QT_MSVC_PATH "D:/Users/CL/Qt/Qt5.14.2/5.14.2/msvc2017_64") #台式机的路径
string(CONCAT QT_CMAKE_PATH ${QT_MSVC_PATH} "/lib/cmake/Qt5")
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE_PATH})

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)


find_package(OpenMP REQUIRED)
find_package(Threads)

add_definitions(/D_USE_MATH_DEFINES)
add_definitions(/MP) #启用多线程编译
add_definitions(/D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler /bigobj")
# set(CUDA_NVCC_FLAGS "${CMAKE_CUDA_FLAGS} --compiler-options;/bigobj")

# 设置VCPKG_ROOT所在的位置
set(VCPKG_ROOT "${CMAKE_SOURCE_DIR} D:/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

file (GLOB QRC_SOURCE_FILES Resources/iGameMeshLab.qrc)
file (GLOB UI_SOURCE_FILES 
    GUI/iGameMeshLab.ui
    GUI/SceneModelItem.ui
    )


file (GLOB SHADER_FILES
        Resources/shaders/*.frag
        Resources/shaders/*.vert
        Resources/shaders/*.shader
        )

file (GLOB GUI_FILE
        GUI/main.cpp
        GUI/iGameMeshLab.cpp
        GUI/SceneModelItem.cpp
        GUI/iGameMeshLab.h
		GUI/SceneManger.*
        GUI/SceneModelItem.h
        GUI/MeshRender.*
		GUI/MouseState.*
		GUI/KeyState.*
		GUI/KeyState.*
        )

file (GLOB GL_FILE
        GL/*.*
        )

file (GLOB ACTOR_FILE
        Actors/*.*
        )

file (GLOB MATERIAL_FILE
        GL/Materials/*.*
        )

file (GLOB CINOLIB_MESH_FILE
        Extern/cinolib/meshes/*.h
        )
file (GLOB CINOLIB_GEO_FILE
        Extern/cinolib/geometry/*.h
        )
file (GLOB CINOLIB_IO_FILE
        Extern/cinolib/io/*.h
        )
file (GLOB CINOLIB_TEX_FILE
        Extern/cinolib/textures/*.h
        )
file (GLOB CINOLIB_FILE
        Extern/cinolib/*.h
        )

include_directories(GUI/)
include_directories(GL/)
include_directories(GL/Materials/)
include_directories(Actors/)
include_directories(Extern/)

add_executable(iGameMeshLab
    ${QRC_SOURCE_FILES}
    ${UI_SOURCE_FILES}
    ${GUI_FILE}
    ${GL_FILE}
    ${MATERIAL_FILE}
    ${SHADER_FILES}
    ${ACTOR_FILE}
    ${CINOLIB_MESH_FILE}
    ${CINOLIB_GEO_FILE}
    ${CINOLIB_IO_FILE}
    ${CINOLIB_TEX_FILE}
    ${CINOLIB_FILE}
)


source_group(GUI FILES ${GUI_FILE})
source_group(GL FILES ${GL_FILE})
source_group(GL/Materials FILES ${MATERIAL_FILE})
source_group(GL/Shader FILES ${SHADER_FILES})
source_group(GL/Actors FILES ${ACTOR_FILE})
source_group(Extern/cinolib FILES ${CINOLIB_FILE})
source_group(Extern/cinolib/meshes FILES ${CINOLIB_MESH_FILE})
source_group(Extern/cinolib/geometry FILES ${CINOLIB_GEO_FILE})
source_group(Extern/cinolib/io FILES ${CINOLIB_IO_FILE})
source_group(Extern/cinolib/textures FILES ${CINOLIB_TEX_FILE})


# 配置运行时的环境
string(CONCAT QT_BIN_PATH ${QT_MSVC_PATH} "/bin")
set(CUSTOM_PATH "PATH=%PATH%" ${QT_BIN_PATH})
set_target_properties(iGameMeshLab PROPERTIES VS_DEBUGGER_ENVIRONMENT "${CUSTOM_PATH}")

target_link_libraries(iGameMeshLab 
        Qt5::Widgets
        Qt5::OpenGL
        Qt5::Core
        Qt5::Gui
        OpenMP::OpenMP_CXX
        ${CMAKE_THREAD_LIBS_INIT}
        )
