#include "../../incs/ft_irc.h"
#include "../../incs/Server/Server.hpp"

#define JOIN_INVITE_WITHOUT_KEY (chan->getInviteMode() == true && checkWaitingList == true && chan->getKeyExist() == false)
#define JOIN_INVITE_WITH_KEY (chan->getInviteMode() == true && checkWaitingList == true && chan->getKeyExist() == true && checkKeyChanExist(chan, command, i) == true)
#define JOIN_WITHOUT_INVIT_KEY chan->getInviteMode() == false && chan->getKeyExist() == false
#define JOIN_WITH_KEY_WITHOUT_INVITE chan->getKeyExist() == true && checkKeyChanExist(chan, command, i) == true && chan->getInviteMode() == false
/*
**==========================
**    CHANNEL COMMAND
		JOIN FUNCTION
**==========================
*/

void	WelcomeTopicJoinMessage(Channel *chan, Command &command, User &user)
{
	user.servInfo->printWelcomeMessage(user.get_fd(), user, command, chan);
	if (chan->TopicIsSet() == true)
		sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
	else
		sendMsg(user.get_fd(), RPL_NOTOPIC(chan->getChannelName()));
}

/***************** 
 * here I :
 * - Parse the channel with the comma delimiter
 * - I create the channel if they begin by "#"
 * - If wrong I send to the client need more params **************/
std::vector<std::string>	CreateChan(Command &command, User &user)
{
	std::vector<std::string> chans;
	std::vector<std::string> goodChanNames;
	chans = parseStringGetline(command.params[0]);
	for (unsigned long i = 0; i < chans.size(); i++)
	{
		if (checkChanName(chans[i]) == true)
		{
			user.servInfo->createChannel(user, chans[i]);
			goodChanNames.push_back(chans[i]);
		}
		else
			sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
	return (goodChanNames);
}

bool	checkKeyChanExist(Channel *channel, Command &command, unsigned long i)
{
	std::vector<std::string> keys;
	keys = getJoinKeys(command);
	if (keys.size() < i || keys.empty() == true)
		return false;
	else if (channel->getKeyName() == keys[i])
		return true;
	else
		return false;
}

/***************** JOIN allows
 * To create a channel if not created
 * allow to join the channel
 * if only in invite mode can't join the channel 
 * if a key is set need to put the key **************/
void	Command::join(Command &command, User &user) {
	
	if (emptyCommand == false)
	{
		std::vector<std::string> chanNames;
		chanNames = CreateChan(command, user);
		for (unsigned long i = 0 ; i < chanNames.size() ; i++)
		{
			Channel *chan = &user.servInfo->getChannel(chanNames[i]);
			if (JOIN_INVITE_WITHOUT_KEY || JOIN_INVITE_WITH_KEY)
			{
				WelcomeTopicJoinMessage(chan, command, user);
			}
			else if (JOIN_WITHOUT_INVIT_KEY) 
			{
				chan->addUserToChannel(user.get_fd(), &user);
				WelcomeTopicJoinMessage(chan, command, user);
			}
			else if (JOIN_WITH_KEY_WITHOUT_INVITE)
			{
				chan->addUserToChannel(user.get_fd(), &user);
				WelcomeTopicJoinMessage(chan, command, user);
			}
			else
				sendMsg(user.get_fd(), ERR_INVITEONLYCHAN(user.getNickName(), chan->getChannelName()));
		}
	}
}


/*
**==========================
**    CHANNEL COMMAND
		TOPIC FUNCTION
**==========================
*/

/***************** I set up the topic of the channel
 *  I set the topic here to send to the client 
 * I need to have at least 3 param TOPIC <channel> <message> **************/
void	SetTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	if (command.params.size() >= 2 && checkUserInchannel == true)
	{
		std::string topic;
		for (unsigned long i = 1; i <= command.params.size(); i++)
			topic = concatenate_strings(topic, command.params[i]);
		chan->changeTopic(topic);
		sendMsg(user.get_fd(), RPL_TOPIC_MSG);
	}
}


void	sendTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	if (command.params.size() == 1 
		&& checkUserInchannel == true && chan->TopicIsSet() == true)
		sendMsg(user.get_fd(), RPL_TOPIC_MSG);
	else if (command.params.size() == 1 && checkUserInchannel == true && chan->TopicIsSet() == false)
		sendMsg(user.get_fd(), RPL_NOTOPIC_MSG);
}

/***************** Topic allows
 * to handle a topic for a dedicated channel **************/
void	Command::topic(Command &command, User &user) {
	if (emptyCommand == false)
	{
		if (user.servInfo->channelExist(command.params[0]) == true) //I verify if param[1](chanName exist)
		{
				SetTopic(command, user);
				sendTopic(command, user);
		}
		else if (user.servInfo->channelExist(command.params[0]) == false) // if the channel doesn't exist = error
			sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]));
	}
	else 
	{
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}

/***************** PART allows 
 * temove a user from one+ channels 
 * first condition : I check if channel exist and i am in the channel
 * second condition : I check if channel exist and i am not in the channel
 * third condition the channel doesn't exist **************/


/*
**==========================
**    CHANNEL COMMAND
		PART FUNCTION
**==========================
*/

void	Command::part(Command &command, User &user) {
	if (emptyCommand == false)
	{
		for (unsigned long i = 0 ; i < command.params.size()  ; i++)
		{
			if (chanExist == true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				if (checkUserInchannel == true)
				{
					chan->printChannelUsers(user.get_fd(), &user, command.params[i]); // to remove
					chan->removeUserChannel(user.get_fd(), &user);
					chan->printChannelUsers(user.get_fd(), &user, command.params[i]); //to remove
				}
				else if (checkUserInchannel == false)
					sendMsg(user.get_fd(), ERR_NOTOCHANNEL(user.getNickName(), chan->getChannelName()));

			}
			else 
				sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL_MSG);
		}
	}
	else 
	{
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}

/*
**==========================
**    CHANNEL COMMAND
		NAMES FUNCTION
**==========================
*/

/***************** NAMES allows
 * display the list of users of specified channels **************/
void	Command::names(Command &command, User &user) {
	if (emptyCommand == false)
	{
		for (unsigned long i = 0 ; i < command.params.size() ; i++)
		{
			if (chanExist== true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				chan->printChannelUsers(user.get_fd(), &user, command.params[i]);
			}
			else
				sendMsg(user.get_fd(), RPL_ENDOFNAMES(command.params[i]));
		}
	}
	else 
		user.servInfo->printAllChannelsUsers(user);
}


/*
**==========================
**    CHANNEL COMMAND
		LIST FUNCTION
**==========================
*/

/***************** ListMinUser 
 * check if I enter "LIST >3" for example,
 *  I display the channels with at least 3 members **************/
bool	listMinUser(Command &command, User &user)
{
	(void) user;
	if (command.params[0][0] == '>' && command.params.size() == 1)
	{
		if (command.params[0].size() == 1) // Means we only have ">"
			return (false);
		for (int i = 1; command.params[0][i] ; i++) // I check here if I only have digits
		{
			if (isdigit(command.params[0][i]) == false)
				return false;
		}
		user.servInfo->displayListMinUser(user, atoi(command.params[0].substr(1).c_str()) );
	}
	return (true);
}

/***************** LIST allows 
 * to show the channels
 * The numbers of users connected to the channel ont it
 * the TOPIC is displayed **************/
void	Command::list(Command &command, User &user)
{
	if (emptyCommand == false)
	{
		if (listMinUser(command, user) == true)
			return;
		for (unsigned long i = 0 ; i < command.params.size()  ; i++)
		{
			if (chanExist == true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				sendMsg(user.get_fd(), RPL_LIST_MSG);
			}
			else
				sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL_MSG);
		}
		sendMsg(user.get_fd(), RPL_LISTEND(user.getNickName()));
	}
	else
		user.servInfo->printListChannels(user);
}

/*
**==========================
**    CHANNEL COMMAND
		INVITE FUNCTION
**==========================
*/

/***************** errors check
 *  * Conditions are the following ones: 
 * channel must exists
 * User invited must exists
 * The one who invites has to be on channel
 * The one who is invited must to not be on channel
 *  **************/
void	inviteErrorscheck(Command &command, User &user)
{
	if (user.servInfo->channelExist(command.params[1]) == true)
	{
		Channel *chan = &user.servInfo->getChannel(command.params[1]);
		if (checkUserInchannel== true)
		{
			if (user.servInfo->userExist(command.params[0]) == true)
			{
				User *nick = &user.servInfo->nickToUserFd(command.params[0]);
				if (chan->userInChannel(nick->get_fd(), chan->getUsersList()) == false)
				{
					chan->addUserToWitingList(nick->get_fd(), nick);
					sendMsg(nick->get_fd(), INVITE_WELCOME_MESSAGE(user.getNickName(), chan->getChannelName()));
					sendMsg(user.get_fd(), RPL_INVITING(nick->getNickName(), chan->getChannelName()));
				}
				else
					sendMsg(user.get_fd(), ERR_USERONCHANNEL(nick->getNickName(), chan->getChannelName()));
			}
			else
				sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME());
		}
		else
			sendMsg(user.get_fd(), ERR_NOTOCHANNEL(user.getNickName(), chan->getChannelName()));
	}
	else
		sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[1]));
}

/***************** INVITE allows
 * To invite someone else to a channel you are already in
 * Conditions are the following ones: 
 * at lease 2 parameters : nickname and channel
 *  **************/
void	Command::invite(Command &command, User &user)
{
	if (command.params.size() == 2)
		inviteErrorscheck(command, user);
	else
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
}

/*
**==========================
**    CHANNEL COMMAND
		KICK FUNCTION
**==========================
*/

/***************** Errors checks for KICK
 * 1: channel doesn't exist
 * 2: User kicked doesn't exist
 * 3: User kicked exists but not on the channel **************/
void	kickErrorCheck(Command &command, User &user)
{
	if (user.servInfo->channelExist(command.params[0]) == true)
	{
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		if (user.servInfo->userExist(command.params[1]) == true)
		{
			User *nick = &user.servInfo->nickToUserFd(command.params[1]);
			if (checkUserInchannel == true)
			{
				chan->removeUserChannel(nick->get_fd(), nick);
				if (command.params.size() == 3)
				{
					std::string message = command.params[2];
					sendMsg(nick->get_fd(), KICK__MESSAGE(user.getNickName(), chan->getChannelName(), message));
				}
				else if (command.params.size() == 2)
				{
					chan->removeUserChannel(nick->get_fd(), nick);
					sendMsg(nick->get_fd(), KICK__MESSAGE(user.getNickName(), chan->getChannelName(), "the host doesn't like you"));
				}
			}
			else
				sendMsg(user.get_fd(), ERR_USERNOTINCHANNEL(nick->getNickName(), chan->getChannelName()));
		}
		else
			sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME());
	}
	else
		sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]));
}

/***************** KICK allors
 * Need at least 2 params, the channel and the nickname
 * An optional message otherwise a default is set
 * having the right to kick **************/
void	Command::kick(Command &command, User &user)
{
	if (command.params.size() >= 2 && command.params.size() <= 3)
		kickErrorCheck(command, user);
	else
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
}

/*
**==========================
**    CHANNEL COMMAND
		NOTICE FUNCTION
**==========================
*/

/***************** NOTICE allows
 * to send a privatemessage to nick or channel
 * RPL specifies to not send error message if the command fails **************/
void	Command::notice(Command &command, User &user)
{
	if (command.params.size() >= 2)
	{
		std::string nickNameOrChannel = command.params[0];
		std::string message;
		for (unsigned long i = 1; i <= command.params.size(); i++)
			message = concatenate_strings(message, command.params[i]);
		if (user.servInfo->channelExist(command.params[0]) == true)
			sendMessageToChannel(user, nickNameOrChannel, message);
		if(user.servInfo->nicknameExists(command.params[0]) == false)
			sendPrivateMessage(user, nickNameOrChannel, message);
	}
	else
		return;
}
