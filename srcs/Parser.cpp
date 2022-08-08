#include "../incs/ft_irc.h"

#define END_OF_CMD "\n"

int		getCmd(std::string cmd, std::vector<std::string> cmdList) {
	for (int i = 0; i < (int)cmdList.size(); i++) {
		if (cmdList[i] == cmd)
			return i;
	}
	return -1;
}

Command *parse(std::vector<std::string> &input, std::vector<std::string> cmdList) {
	Command *res = new Command();
	res->cmd = getCmd(input[0], cmdList);
	input.erase(input.begin());
	while (input.empty() == false) {
		res->params.push_back(*input.begin());
		input.erase(input.begin());
		if (input.begin()[0] == END_OF_CMD) {
			input.erase(input.begin());
			return res;
		}
	}
	return res;
}

void	printCmd(Command &cmdList) {
	std::cout << "CMD = " << cmdList.cmd << std::endl;
	for (int i = 0; i < (int)cmdList.params.size(); i++) {
		std::cout << "Param " << i << ": " << cmdList.params[i] << std::endl;
	}
}

std::vector<std::string>	split(std::string str, std::string const sep) {

	size_t						pos = 0;
	std::string					token;
	std::vector<std::string>	res;

	while ((pos = str.find_first_of(sep)) != str.npos) {
		token = str.substr(0, pos);
		res.push_back(token);
		if (str[pos] == '\r') {
			res.push_back("\n");
			str.erase(0, pos + 2);
		} else {
			str.erase(0, pos + 1);
		}
	}
	return res;
}
