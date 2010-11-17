/*
 *  player.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/entitymanager.h"
#include "Gaem/menumanager.h"

#include "Common/messages.h"

#include "Entities/world.h"
#include "Entities/player.h"

namespace Entities
{
	Player::Player(const std::string &path):
		_speed(250), _speed_up(100), _can_jump(true),
		_dir(1),_pos_left(0), _pos_depth(50),
		_elevation(0), _state(inet::STATE_WAITING), _is_running(false)
	{
		_sprite = new Gaem::AnimatedSprite;
		
		Gaem::Config info(path);
		
		_sprite->loadAnimation("running", info.get("running"));
		_sprite->loadAnimation("waiting", info.get("waiting"));
		_sprite->loadAnimation("jumping", info.get("jumping"));
		
		_scale = info.getFloat("scale", 1.0);
		_speed = info.getFloat("speed", 250);
		_speed_up = info.getFloat("up speed", 100);
		_can_jump = info.isTrue("can jump");
		_name = info.get("name");
		
		_sprite->setAnimation("waiting");
		
		_sprite->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _sprite->getWidth()/2);
		
		setDepth(_pos_depth);
		
		_velocity.x = 0;
		_velocity.y = 0;
	}
	
	Player::~Player()
	{
		delete _sprite;
	}
	
	void Player::setDepth(float depth)
	{
		int world_height = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getHeight();
		
		// clamp depth position
		depth = depth > world_height ? world_height : depth;
		depth = depth < 0 ? 0 : depth;
		
		_pos_depth = depth; 
		
		float factor = 0.3 * (world_height - depth) / world_height;
		float scale = (1 - factor) * _scale;
		_sprite->getSprite()->SetScale(sf::Vector2f(scale, scale));
	}
	
	bool Player::isActivePlayer()
	{
		return Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer() == this;
	}
	
	void Player::jump()
	{
		if ( _elevation == 0 && _can_jump )
			_velocity.y = 500;
	}
	
	void Player::runLeft()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_is_running = true;
		_sprite->flip(true);
		_dir = -1;
		_pos_left -= _speed * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::runRight()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_is_running = true;
		_sprite->flip(false);
		_dir = 1;
		_pos_left += _speed * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveUp()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_pos_depth -= _speed_up * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveDown()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_pos_depth += _speed_up * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::clampPos()
	{
		bool did_enter_new_world = false, was_left = false;
		
		Gaem::Gaem *game = Gaem::Gaem::getInstance();
		int world_width = game->getEntityManager()->getWorld()->getWidth();
		
		// Moved right
		if ( _pos_left % world_width != _pos_left )
			did_enter_new_world = true;
		
		// Clamp the player position to the width of the playing field
		_pos_left = _pos_left % world_width;
		
		// To the left
		if ( _pos_left < 0 )
		{
			_pos_left = world_width - _pos_left;
			did_enter_new_world = was_left = true;
		}
		
		if ( did_enter_new_world && isActivePlayer() )
			game->getEntityManager()->getWorld()->reverseScrollLeft(was_left);
	}
	
	void Player::handleEvent(const sf::Event &event)
	{
		if ( Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer() != this )
			return;
		
		switch (event.Type) {
			case sf::Event::KeyPressed: {
				if ( event.Key.Code == sf::Key::Space )
				{
					jump();
				}
				break;
			}
			
			default: break;
		}
	}
	
	void Player::logic()
	{
		Gaem::Gaem *game = Gaem::Gaem::getInstance();
		const sf::Input &input = game->getWindow()->GetInput();
		
		int world_height = game->getEntityManager()->getWorld()->getHeight();
		
		float tdelta = game->getTDelta();
		
		// If I am the player (and the menus are down)
		if ( isActivePlayer() && ! game->getMenuManager()->hasMenus() )
		{
			// If any move key is pressed set the running sprite and move the player
			if (
				input.IsKeyDown(sf::Key::Left) || input.IsKeyDown(sf::Key::Right)
				|| input.IsKeyDown(sf::Key::Down) || input.IsKeyDown(sf::Key::Up)
			) {
				// Set left running sprite and move player
				if ( input.IsKeyDown(sf::Key::Left) )
					runLeft();
				
				// Set right running sprite and move player
				if ( input.IsKeyDown(sf::Key::Right) )
					runRight();
				
				// Move up
				if ( input.IsKeyDown(sf::Key::Up) )
					moveUp();
				
				// Move down
				if ( input.IsKeyDown(sf::Key::Down) )
					moveDown();
			}
			else if ( _elevation == 0 )
			{
				_sprite->setAnimation("waiting");
				_is_running = false;
			}
		}
		else if ( ! isActivePlayer() )
		{
			if ( _state == inet::STATE_RUNNING )
			{
				if ( _dir == -1 )
					runLeft();
				else
					runRight();
			}
		}
		
		clampPos();
		
		// Update velocity
		if ( _elevation != 0 )
		{
			_sprite->setAnimation("jumping");
			_velocity.y -= 1000 * tdelta;
		}
		
		// Update position based on velocity
		_elevation += _velocity.y * tdelta;
		if ( _elevation <= 0 )
		{
			_elevation = 0;
			_velocity.y = 0;
		}
		
		setDepth(_pos_depth);
		
		_sprite->setY(game->getHeight() - world_height + _pos_depth - _sprite->getHeight() - _elevation);
		_sprite->setX(_pos_left - game->getEntityManager()->getWorld()->getScrollLeft());
		
		// Step animation
		_sprite->step();
	}
	
	void Player::draw(sf::RenderWindow &window)
	{
		window.Draw(*_sprite->getSprite());
		
		// Draw two more versions of the player, one outside the field to the left
		// and one outside to the right, this so it does not disappear when the position
		// is clamped
		int scroll = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getScrollLeft();
		int width = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getWidth();
		
		float old_x = _sprite->getX();
		_sprite->setX(_pos_left - width - scroll);
		window.Draw(*_sprite->getSprite());
		_sprite->setX(old_x);
		
		old_x = _sprite->getX();
		_sprite->setX(_pos_left + width - scroll);
		window.Draw(*_sprite->getSprite());
		_sprite->setX(old_x);
	}
	
	void Player::setDir(int dir)
	{
		_dir = dir;
	}
	
	void Player::setLeft(int left)
	{
		_pos_left = left;
	}
	
	void Player::setElevation(float elevation)
	{
		_elevation = elevation;
	}
	
	void Player::setState(inet::PlayerActionState state)
	{
		_state = state;
	}
	
	float Player::getElevation()
	{
		return _elevation;
	}
	
	int Player::getLeft()
	{
		return _pos_left;
	}
	
	int Player::getRight()
	{
		return _pos_left + _sprite->getWidth();
	}
	
	float Player::getDepth()
	{
		return _pos_depth;
	}
	
	inet::PlayerState Player::getPlayerStruct()
	{
		inet::PlayerState player;
		player.dir = _dir;
		player.left = _pos_left;
		player.depth = _pos_depth;
		player.elevation = _elevation;
		
		if ( player.elevation > 0 )
			player.state = inet::STATE_JUMPING;
		else if ( _is_running )
			player.state = inet::STATE_RUNNING;
		else
			player.state = inet::STATE_WAITING;
		
		return player;
	}
}
