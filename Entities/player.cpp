/*
 *  player.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"

#include "My/myentitymanager.h"
#include "Entities/world.h"
#include "Entities/player.h"

namespace Entities
{
	Player::Player(): _pos_left(0), _pos_depth(50), _dir(1), _elevation(0)
	{
		_sprite = new Gaem::AnimatedSprite;
		
		_sprite->loadAnimation("running", "resources/players/player_1_running/info.txt");
		_sprite->loadAnimation("waiting", "resources/players/player_1_waiting/info.txt");
		_sprite->loadAnimation("jumping", "resources/players/player_1_jumping/info.txt");
		
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
		
		float scale = 1 * (depth) / world_height + 4;
		_sprite->getSprite()->SetScale(sf::Vector2f(scale, scale));
	}
	
	void Player::handleEvent(const sf::Event &event)
	{
		switch (event.Type) {
			case sf::Event::KeyPressed: {
				if ( event.Key.Code == sf::Key::Space )
				{
					_velocity.y = 300;
				}
				break;
			}
		}
	}
	
	void Player::logic()
	{
		Gaem::Gaem *game = Gaem::Gaem::getInstance();
		const sf::Input &input = game->getWindow()->GetInput();
		
		int viewport_w = game->getWidth();
		int world_width = game->getEntityManager()->getWorld()->getWidth();
		int world_height = game->getEntityManager()->getWorld()->getHeight();
		
		float tdelta = game->getTDelta();
		
		// If any move key is pressed set the running sprite and move the player
		if (
			input.IsKeyDown(sf::Key::Left) || input.IsKeyDown(sf::Key::Right)
			|| input.IsKeyDown(sf::Key::Down) || input.IsKeyDown(sf::Key::Up)
		) {
			_sprite->setAnimation("running");
			
			// Set left running sprite and move player
			if ( input.IsKeyDown(sf::Key::Left) )
			{
				_sprite->getSprite()->FlipX(true);
				_dir = -1;
				_pos_left -= 250 * tdelta;
			}
			
			// Set right running sprite and move player
			if ( input.IsKeyDown(sf::Key::Right) )
			{
				_sprite->getSprite()->FlipX(false);
				_dir = 1;
				_pos_left += 250 * tdelta;
			}
			
			// Move up
			if ( input.IsKeyDown(sf::Key::Up) )
			{
				_pos_depth -= 100 * tdelta;
			}
			
			// Move down
			if ( input.IsKeyDown(sf::Key::Down) )
			{
				_pos_depth += 100 * game->getTDelta();
			}
			
			bool did_enter_new_world = false, was_left = false;
			
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
			
			if ( did_enter_new_world )
				game->getEntityManager()->getWorld()->reverseScrollLeft(was_left);
			
			setDepth(_pos_depth);
		}
		else
		{
			_sprite->setAnimation("waiting");
		}
		
		// Update velocity
		if ( _elevation != 0 )
		{
			_sprite->setAnimation("jumping");
			_velocity.y -= 1000 * tdelta;
		}
		
		_elevation += _velocity.y * tdelta;
		if ( _elevation <= 0 )
		{
			_elevation = 0;
			_velocity.y = 0;
		}
		
		_sprite->setY(game->getHeight() - world_height + _pos_depth - _sprite->getHeight() - _elevation);
		_sprite->setX(_pos_left - game->getEntityManager()->getWorld()->getScrollLeft());
		
		// Step animation
		_sprite->step();
	}
	
	void Player::draw(sf::RenderWindow &window)
	{
		window.Draw(*_sprite->getSprite());
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
}
