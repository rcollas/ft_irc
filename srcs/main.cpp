#include "../incs/Server/Server.hpp"

void	printDebug(std::string msg, bool print) {
	if (print)
		std::cout << msg << std::endl;
}

int	main() {
	Server server;
	server.init();
	server.run();
}
