# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 

include_directories (${siptricks_SOURCE_DIR}) 

file(GLOB Sources *.cpp)

add_library (siplayer ${Sources})
