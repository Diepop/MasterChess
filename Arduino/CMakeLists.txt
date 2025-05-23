
add_library(Arduino STATIC 
	"SerialPort.hpp" "SerialPort.cpp"
	"SimpleRPC.hpp" "SimpleRPC.cpp"
	"Serialization.hpp" "Serialization.cpp"
	"ArduinoChessBoard.hpp" "ArduinoChessBoard.cpp"
	"ArduinoExported.hpp"
	"Matrix8x8.hpp"
 )

find_package(boost_asio REQUIRED CONFIG)
target_link_libraries(Arduino PRIVATE Boost::asio)

