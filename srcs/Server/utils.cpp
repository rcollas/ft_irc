#include "../../incs/ft_irc.h"

int		len(long nb)
{
	int		len;

	len = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}
    if (nb == 0) {
        len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int nb)
{
	char *str;
	long	n;
	int		i;

	n = nb;
	i = len(n);
	if (!(str = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	str[i--] = '\0';
	if (n == 0)
	{
		str[0] = 48;
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n = n * -1;
	}
	while (n > 0)
	{
		str[i] = 48 + (n % 10);
		n = n / 10;
		i--;
	}
	return (str);
}

/*****************
 * I want to parse a string with a delimiter
 * getline allow to take a string from stringstream until the end of the line
 * string will be the string separated by the delimiter ',' **************/
std::vector<std::string>	parseStringGetline(std::string string)
{
	std::stringstream input_stringstream(string);
	std::vector<std::string> result;
	while (std::getline(input_stringstream, string, ','))
		result.push_back(string);
	return (result);
}

std::vector<std::string>	getJoinKeys(Command &command)
{
	std::vector<std::string> keys;
	if (command.params.size() == 2)
		keys = parseStringGetline(command.params[1]);
	return (keys);
}

bool						checkChanName(std::string chanName)
{
	if(chanName[0] == '#' && chanName.size() > 1)
		return true;
	return false;
}

bool	isAllowedMode(std::string str) {

	return (str == "+i" || str == "+k" || str == "-i" || str == "-k");
}


std::string					concatenate_strings(std::string first, std::string second)
{
	first = first.append(" ");
	first = first.append(second);
	return (first);
}
