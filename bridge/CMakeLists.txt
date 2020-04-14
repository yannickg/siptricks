include_directories (${siptricks_SOURCE_DIR}) 

file(GLOB Sources *.cpp)

set(IncludeDir /usr/include/sipxtapi /home/yannickg/dev/resiprocate/contrib/asio ${CMAKE_CURRENT_SOURCE_DIR}/../resiprocate .)
include_directories(${IncludeDir})
add_definitions(-DDEFAULT_BRIDGE_MAX_IN_OUTPUTS=10 -D__pingtel_on_posix__ -D_linux -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DRESIP_TOOLCHAIN_GNU -DRESIP_OSTYPE_LINUX -DRESIP_ARCH_X86_64 -DHAVE_sockaddr_in_len -DUSE_CARES -DUSE_SSL -DUSE_IPV6 -DHAVE_EPOLL)

add_executable (bridge ${Sources}) 

IF(WIN32)
	target_link_libraries (bridge Ws2_32)
	target_link_libraries (bridge Dnsapi)
	target_link_libraries (bridge iphlpapi)
	target_link_libraries (bridge winmm)

	set(ResiprocateDir ../../resiprocate/Win32/Debug)

	target_link_libraries (bridge ${ResiprocateDir}/ares)
	target_link_libraries (bridge ${ResiprocateDir}/dum)
	target_link_libraries (bridge ${ResiprocateDir}/popt)
	target_link_libraries (bridge ${ResiprocateDir}/resiprocate)
	target_link_libraries (bridge ${ResiprocateDir}/rutil)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries (bridge resip)
	target_link_libraries (bridge dum)
	target_link_libraries (bridge rutil)
	target_link_libraries (bridge popt)
	target_link_libraries (bridge resipares)
	target_link_libraries (bridge recon)
ENDIF(WIN32)
