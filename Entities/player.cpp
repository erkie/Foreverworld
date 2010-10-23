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

#include "Entities/world.h"
#include "Entities/player.h"

namespace Entities
{
	Player::Player(const std::string &path): _pos_left(0), _pos_depth(50), _dir(1), _elevation(0)
	{
		_sprite = new Gaem::AnimatedSprite;
		
		Gaem::Config info(path);
		
		_sprite->loadAnimation("running", info.get("running"));
		_sprite->loadAnimation("waiting", info.get("waiting"));
		_sprite->loadAnimation("jumping", info.get("jumping"));
		
		_scale = info.getFloat("scale", 1.0);
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
		
		float scale = 1 * (depth) / world_height + _scale;
		_sprite->getSprite()->SetScale(sf::Vector2f(scale, scale));
	}
	
	bool Player::isActivePlayer()
	{
		return Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer() == this;
	}
	
	void Player::jump()
	{
		if ( _elevation == 0 )
			_velocity.y = 500;
	}
	
	void Player::runLeft()
	{
		_sprite->setAnimation("running");
		_sprite->getSprite()->FlipX(true);
		_dir = -1;
		_pos_left -= 250 * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::runRight()
	{
		_sprite->setAnimation("running");
		_sprite->getSprite()->FlipX(false);
		_dir = 1;
		_pos_left += 250 * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveUp()
	{
		_sprite->setAnimation("running");
		_pos_depth -= 100 * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveDown()
	{
		_sprite->setAnimation("running");
		_pos_depth += 100 * Gaem::Gaem::getInstance()->getTDelta();
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
		
		if ( ! game->getMenuManager()->hasMenus() && isActivePlayer() )
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
			else
			{
				_sprite->setAnimation("waiting");
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
		
		// Draw pos of player(s) in top right corner
		std::stringstream str;
		str << _pos_left;
		sf::String pos(str.str());
		pos.SetColor(sf::Color(0x0E, 0x81, 0xED));
		pos.SetX(Gaem::Gaem::getInstance()->getWidth() - pos.GetRect().GetWidth() - 10);
		pos.SetY(isActivePlayer() ? 10 : (20 + pos.GetRect().GetHeight()));
		window.Draw(pos);
		
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
