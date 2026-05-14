cmake_minimum_required(VERSION 3.26)

    # NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
    # for now, the project name is used as the executable name

##CONFIGURATII PROIECT
set(MAIN_PROJECT_NAME "POO2")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(${MAIN_PROJECT_NAME})
    # set(CMAKE_PROJECT_VERSION_MAJOR 0)
    # set(CMAKE_PROJECT_VERSION_MINOR 0)
    # set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##INCLUDEM HELPERE DIN cmake/
include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

##DESCARCAM SFML FOLOSINF FETCH CONTENT
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
)

##DEZACTIVAM CHESTIILE DIN SFML CARE NU NE TREBUIE
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

##DEFINIRE EXECUTABIL
    # NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
        include/UI.h
        include/GridCell.h
        src/GridCell.cpp
        include/PopUp.h
        src/PopUp.cpp
        include/Button.h
        src/Button.cpp
        include/Key.h
        include/Label.h
        src/Label.cpp
        include/GE.h
        src/GE.cpp
        include/WM.h
        src/WM.cpp
        include/WordleExceptions.h
)
target_sources(${MAIN_EXECUTABLE_NAME}
    PRIVATE
        src/UI.cpp
)

##APLICAM FLAGS DE COMPILARE (DIN INCLUDE)
    # NOTE: Add all defined targets (e.g. executables, libraries, etc. )
    # NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
    # set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
    # where ${FOO} and ${BAR} represent additional executables or libraries
    # you want to compile with the set compiler flags


##LINKS BIBLIOTECI
##MODULELE SFML
    # use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories


target_link_libraries(${MAIN_EXECUTABLE_NAME}
    PRIVATE
           SFML::Graphics
    )
target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE
        ${sfml-graphics_SOURCE_DIR}/include
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
    ext
)

##INSTALARE SI COPIERE RESURSE


    # target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${<SomeLib>_SOURCE_DIR}/include)
    # target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${<SomeLib>_BINARY_DIR}/lib)
    # target_link_libraries(${MAIN_EXECUTABLE_NAME} <SomeLib>)

    # copy binaries to "bin" folder; these are uploaded as artifacts on each release
    # DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

##COPIEM FISIERE DIN RESURSE(IMAGINI, TEXT)
copy_files(FILES tastatura.txt DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
