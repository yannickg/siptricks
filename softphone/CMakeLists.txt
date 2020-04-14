include_directories (${siptricks_SOURCE_DIR}) 

file(GLOB Sources *.cpp)

add_executable (softphone ${Sources}) 

target_link_libraries (softphone siplayer)
target_link_libraries (softphone helper)

IF(WIN32)
	target_link_libraries (softphone Ws2_32)
	target_link_libraries (softphone Dnsapi)
	target_link_libraries (softphone iphlpapi)
	target_link_libraries (softphone winmm)

	set(ResiprocateDir ../../resiprocate/Win32/Debug)

	target_link_libraries (b2bua ${ResiprocateDir}/ares)
	target_link_libraries (b2bua ${ResiprocateDir}/dum)
	target_link_libraries (b2bua ${ResiprocateDir}/popt)
	target_link_libraries (b2bua ${ResiprocateDir}/resiprocate)
	target_link_libraries (b2bua ${ResiprocateDir}/rutil)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries (softphone resip)
	target_link_libraries (softphone dum)
	target_link_libraries (softphone rutil)
	target_link_libraries (softphone popt)
	target_link_libraries (softphone resipares)
ENDIF(WIN32)
