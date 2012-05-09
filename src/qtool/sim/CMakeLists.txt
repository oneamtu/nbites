
############ qtool

project( sim )
cmake_minimum_required(VERSION 2.8.0)

message(STATUS "")
message(STATUS "...::: Configuring the sim project :::... ")
message(STATUS "")

########### Dependencies

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

######### FLAGS

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

########### Sources

set( sim_srcs main.cpp
              SimView.cpp )

set(sim_headers SimView.h
                PaintWorld.h )


############ Qt4 goodness

Qt4_wrap_cpp(sim_headers_moc ${sim_headers})

######### The executable

add_executable(sim
  ${sim_srcs}
  ${sim_headers_moc})

target_link_libraries(
  sim
  viewer
  image
  noggin
  ${QT_LIBRARIES}
  )
