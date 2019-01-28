
#include "player.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

Player::Player(LobbyPlayer const *player)
: name(player->name), score(0), team(player->team), computer(player->computer), owner(false)
{
}

Player::Player()
: score(0), team(0), computer(NULL), owner(false)
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void Player::send_full(class CL_OutputSource *output, bool owner)
{
	output->write_string(name);
	output->write_int32(team);
	output->write_int32(score);
	output->write_int32(ID);
	output->write_bool8(owner);
}

void Player::recv_full(class CL_InputSource *input)
{
	name = input->read_string();
	team = input->read_int32();
	score = input->read_int32();
	ID = input->read_int32();
	owner = input->read_bool8();
}

void Player::send_partial(class CL_OutputSource *output)
{
	output->write_int32(score);
}

void Player::recv_partial(class CL_InputSource *input)
{
	score = input->read_int32();
}
