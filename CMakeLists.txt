project(dbus-c++ CXX)
cmake_minimum_required(VERSION 3.0)

include(FindPkgConfig)

pkg_check_modules(DBUSC REQUIRED dbus-1)

pkg_check_modules(EXPAT expat)

pkg_check_modules(CTEMPL libctemplate_nothreads)

include_directories(
    ${DBUSC_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

message("CC ${CMAKE_CROSSCOMPILING}")

add_library(dbus-c++-1 SHARED
    src/connection.cpp  
    src/debug.cpp       
    src/error.cpp      
    src/eventloop-integration.cpp  
    src/interface.cpp    
    src/introspection.cpp  
    src/pendingcall.cpp  
    src/property.cpp
    src/dispatcher.cpp  
    src/eventloop.cpp  
    src/message.cpp  
    src/object.cpp   
    src/server.cpp    
    src/types.cpp
)

set_target_properties(dbus-c++-1
        PROPERTIES
                VERSION 0
                SOVERSION 0.0.0
)
set_target_properties(dbus-c++-1
    PROPERTIES
        COMPILE_DEFINITIONS DEBUG
)

target_link_libraries(dbus-c++-1
    ${DBUSC_LIBRARIES}
)

install(TARGETS dbus-c++-1 DESTINATION lib)

install( 
    FILES
        include/dbus-c++/api.h  
        include/dbus-c++/connection.h  
        include/dbus-c++/dbus.h  
        include/dbus-c++/debug.h  
        include/dbus-c++/dispatcher.h  
        include/dbus-c++/error.h  
        include/dbus-c++/eventloop.h  
        include/dbus-c++/eventloop-integration.h  
        include/dbus-c++/interface.h  
        include/dbus-c++/introspection.h  
        include/dbus-c++/message.h  
        include/dbus-c++/object.h  
        include/dbus-c++/pendingcall.h  
        include/dbus-c++/property.h  
        include/dbus-c++/refptr_impl.h  
        include/dbus-c++/server.h  
        include/dbus-c++/types.h  
        include/dbus-c++/util.h
    DESTINATION 
        include/dbus-c++-1/dbus-c++
)

#
# Tools
#
message("${CTEMPL_FOUND} AND ${EXPAT_FOUND}")
if (${CTEMPL_FOUND} AND ${EXPAT_FOUND})
    add_executable(dbusxx-xml2cpp
        tools/xml.cpp 
        tools/xml2cpp.cpp
        tools/generate_stubs.cpp
        tools/generator_utils.cpp
    )
    
    set_target_properties(dbusxx-xml2cpp
        PROPERTIES
            COMPILE_DEFINITIONS DATADIR=\"/usr/share/dbus-c++\"
    )
    target_link_libraries(dbusxx-xml2cpp
        dbus-c++-1
        ${CTEMPL_LIBRARIES}
        ${EXPAT_LIBRARIES}
    )
    
    add_executable(dbusxx-introspect
        tools/introspect.cpp
    )
    target_link_libraries(dbusxx-introspect
        dbus-c++-1
    )
    
    
    set(HAS_BUILD_TOOLS NOT ${CMAKE_CROSSCOMPILING})
    message("HAS_BUILD_TOOLS ${HAS_BUILD_TOOLS}")
        
        

    if (${CMAKE_CROSSCOMPILING})
        message("cross build - skip examples")    
        find_program(NXML2CPP dbusxx-xml2cpp)
        message("${NXML2CPP-NOT-FOUND} ${NXML2CPP}")
        set(BUILD_SAMPLES NOT ${NXML2CPP-NOT-FOUND})
        message("${BUILD_SAMPLES}")
    else ()
        message("native build - I will add the targets later")
    endif ()
    # set(BUILD_SAMPLES TRUE)
endif()

#
# Tools
#
if (${BUILD_SAMPLES})
    message(Build samples)
endif()




if (${CMAKE_CROSSCOMPILING})
    message("cross build - skip examples")    
else ()
    message("native build - I will add the targets later")
endif ()
