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
	if (!(atoi(port.c_str()) > 1023 && atoi(port.c_str()) < 65500)) {
		std::cerr << "Error: port number should be between 1023 and 65000" << std::endl; //le port est code sur 16 bits donc valeur max = 65536 et les 1023 premiers bits sont consideres comme speciaux par l'OS et requierent le statut d'admin
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
