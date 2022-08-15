#include "../incs/Server/Server.hpp"

 void	printDebug(std::string msg, bool print) {
	if (print)
		std::cout << msg << std::endl;
}

void	checkArgs(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Usage ./ft_irc <port> <password>" << std::endl;
		exit(EXIT_FAILURE) ;
	}
	std::string port = av[1];
	std::string passwd = av[2];
	if (!(atoi(port.c_str()) > 1023 && atoi(port.c_str()) < 50000)) {
		std::cerr << "Error: port number should be between 1023 and 50000" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (passwd.length() != 4 || !(passwd.find_first_not_of("0123456789") == std::string::npos)) {
		std::cerr << "Error: password must be 4 digits" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av) {
	checkArgs(ac, av);
	Server server(av[1], av[2]);
	server.init();
	server.run();
}
