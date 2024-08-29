# all source files
set(SRC
    Ftp.cpp
    Http.cpp
    IpAddress.cpp
    Packet.cpp
    Socket.cpp
    SocketSelector.cpp
    TcpListener.cpp
    TcpSocket.cpp
    UdpSocket.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    list(APPEND SRC
        Win32/SocketImpl.cpp
    )
else()
    list(APPEND SRC
        Unix/SocketImpl.cpp
    )
endif()

# define the sfml-network target
sfml_add_library(Network
                 SOURCES ${SRC})

# setup dependencies
target_link_libraries(sfml-network PUBLIC SFML::System)
if(SFML_OS_WINDOWS)
    target_link_libraries(sfml-network PRIVATE ws2_32)
endif()
