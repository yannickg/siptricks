include_directories (${siptricks_SOURCE_DIR}) 

file(GLOB Sources *.cpp)

add_executable (b2bua ${Sources}) 

target_link_libraries (b2bua siplayer)
target_link_libraries (b2bua helper)
target_link_libraries (b2bua cnode)

IF(WIN32)
	target_link_libraries (b2bua Ws2_32)
	target_link_libraries (b2bua Dnsapi)
	target_link_libraries (b2bua iphlpapi)
	target_link_libraries (b2bua winmm)

	set(ResiprocateDir ../../resiprocate/Win32/Debug)

	target_link_libraries (b2bua ${ResiprocateDir}/ares)
	target_link_libraries (b2bua ${ResiprocateDir}/dum)
	target_link_libraries (b2bua ${ResiprocateDir}/popt)
	target_link_libraries (b2bua ${ResiprocateDir}/resiprocate)
	target_link_libraries (b2bua ${ResiprocateDir}/rutil)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries (b2bua resip)
	target_link_libraries (b2bua dum)
	target_link_libraries (b2bua rutil)
	target_link_libraries (b2bua popt)
	target_link_libraries (b2bua resipares)
	target_link_libraries (b2bua erl_interface)
	target_link_libraries (b2bua ei)
ENDIF(WIN32)
