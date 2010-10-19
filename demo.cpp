// class Entity
// class Sprite

Player::Player()
{
	_sprite->loadAnimation("running", "players/player_1_running/");
	_sprite->loadAnimation("waiting", "players/player_1_waiting/");
}

Player::onKeyPress(Event event)
{
	if ( event.code == Event::LEFT )
	{
		_sprite->flipHorizontal();
		_sprite->setAnimation("running");
	}
	else if ( event.code == Event::RIGHT )
	{
		_sprite->flipHorizontal();
		_sprite->setAnimation("running");
	}
}

Player::onKeyUp(Event event)
{
	_sprite->setAnimation("waiting");
}
