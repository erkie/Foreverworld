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
#include "Gaem/user.h"
#include "Gaem/entitymanager.h"
#include "Gaem/menumanager.h"
#include "Gaem/resourcemanager.h"

#include "Common/messages.h"

#include "Entities/world.h"
#include "Entities/player.h"

namespace Entities
{
	void Player::init()
	{
		_speed = 250;
		_speed_up = 100;
		_can_jump = true;
		_user = NULL;
		_pos_left = 0;
		_pos_depth = 50;
		_elevation = 0;
		_state = inet::STATE_WAITING;
		_dir[0] = 1;
		_dir[1] = 0;
		_velocity.x = 0;
		_velocity.y = 0;
		
		_sprite = new Gaem::AnimatedSprite;
	}

	Player::Player(const std::string &path)
	{
		init();
		
		Gaem::Config info(path);
		
		_sprite->loadAnimation("running", info.get("running"));
		_sprite->loadAnimation("waiting", info.get("waiting"));
		_sprite->loadAnimation("jumping", info.get("jumping"));
		_sprite->loadAnimation("attack1", info.get("attack1"));
		_sprite->loadAnimation("attack2", info.get("attack2"));
		_sprite->loadAnimation("defence", info.get("defence"));
		
		_scale = info.getFloat("scale", 1.0);
		_speed = info.getFloat("speed", 250);
		_speed_up = info.getFloat("up speed", 100);
		_can_jump = info.isTrue("can jump");
		_name = info.get("name");
		
		_sprite->setAnimation("waiting");
		_sprite->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _sprite->getWidth()/2);
		
		setDepth(_pos_depth);
	}
	
	Player::Player(const inet::Character &character)
	{
		init();
		
		setCharacter(character);
		
		_sprite->setAnimation("waiting");
		_sprite->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _sprite->getWidth()/2);
		
		setDepth(_pos_depth);
	}
	
	Player::~Player()
	{
		delete _sprite;
	}
	
	Gaem::User *Player::getUser()
	{
		return _user;
	}
	
	void Player::setUser(Gaem::User *user)
	{
		_user = user;
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
	
	void Player::setCharacter(const inet::Character &character)
	{
		_scale = character.scale;
		_speed = character.speed;
		_speed_up = character.up_speed;
		_can_jump = character.can_jump;
		_name = std::string(character.name);
		
		std::string slug = character.slug;
		
		_sprite->loadAnimation("running", "resources/players/" + slug + "/" + slug + "_walking/info.txt");
		_sprite->loadAnimation("waiting", "resources/players/" + slug + "/" + slug + "_waiting/info.txt");
		_sprite->loadAnimation("jumping", "resources/players/" + slug + "/" + slug + "_jumping/info.txt");
	}
	
	bool Player::isActivePlayer()
	{
		return Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer() == this;
	}
	
	void Player::jump()
	{
		if ( _elevation == 0 && _can_jump )
		{
			_velocity.y = 500;
			_sprite->setAnimation("jumping");
			_sprite->getAnimation()->reset();
		}
	}
	
	void Player::runLeft()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_sprite->flip(true);
		_dir[0] = -1;
		_pos_left -= _speed * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::runRight()
	{
		if ( _elevation == 0 )
			_sprite->setAnimation("running");
		_sprite->flip(false);
		_dir[0] = 1;
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
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
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
				if ( input.IsKeyDown(sf::Key::Left) && _dir[0] != -1 )
				{
					_dir[0] = -1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Set right running sprite and move player
				if ( input.IsKeyDown(sf::Key::Right) && _dir[0] != 1 )
				{
					_dir[0] = 1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Move up
				if ( input.IsKeyDown(sf::Key::Up) && _dir[1] != 1 )
				{
					_dir[1] = 1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Move down
				if ( input.IsKeyDown(sf::Key::Down) && _dir[1] != -1 )
				{
					_dir[1] = -1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				if ( ! input.IsKeyDown(sf::Key::Down) && ! input.IsKeyDown(sf::Key::Up) && _dir[1] != 0 )
				{
					_dir[1] = 0;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				if ( ! input.IsKeyDown(sf::Key::Left) && ! input.IsKeyDown(sf::Key::Right) && _dir[0] != 0 )
				{
					_dir[0] = 0;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
			}
			else if ( _dir[0] != 0 || _dir[1] != 0 )
			{
				_dir[0] = 0;
				_dir[1] = 0;
				Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
			}
		}
		
		if ( _dir[0] != 0 )
		{
			if ( _dir[0] == -1 )
				runLeft();
			else
				runRight();
		}
		
		if ( _dir[1] == 1 )
		{
			moveUp();
		}
		else if ( _dir[1] == -1 )
		{
			moveDown();
		}
		
		if ( _dir[0] == 0 && _dir[1] == 0 && _elevation <= 0 && _velocity.y <= 0 )
		{
			_sprite->setAnimation("waiting");
		}
		
		clampPos();
		
		// Update velocity
		if ( _elevation > 0 || _velocity.y > 0 )
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
		float real_old_x = _sprite->getX(), x;
		x = real_old_x - _sprite->getWidth() / 2;
		_sprite->setX(x);
		window.Draw(*_sprite->getSprite());
		
		// Draw name string
		if ( _user )
		{
			sf::String name(_user->getUsername(), *Gaem::Gaem::getInstance()->getResourceManager()->getFont("resources/main_font.ttf", 15), 15);
			name.SetX(_sprite->getX() - name.GetRect().GetWidth()/2 + _sprite->getWidth()/2);
			name.SetY(_sprite->getY() - name.GetRect().GetHeight());
			
			window.Draw(name);
		}
		
		// Draw two more versions of the player, one outside the field to the left
		// and one outside to the right, this so it does not disappear when the position
		// is clamped
		int scroll = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getScrollLeft();
		int width = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getWidth();
		
		float old_x = _sprite->getX();
		// Offset to the left
		_sprite->setX(_pos_left - width - scroll);
		window.Draw(*_sprite->getSprite());
		_sprite->setX(old_x);
		
		old_x = _sprite->getX();
		// Offset to the right
		_sprite->setX(_pos_left + width - scroll);
		window.Draw(*_sprite->getSprite());
		_sprite->setX(old_x);
		
		_sprite->setX(real_old_x);
	}
	
	void Player::setDir(int dir_x, int dir_y)
	{
		_dir[0] = dir_x;
		_dir[1] = dir_y;
	}
	
	void Player::setLeft(int left)
	{
		_pos_left = left;
	}
	
	void Player::setElevation(float elevation)
	{
		_elevation = elevation;
	}
	
	void Player::setVelocity(float x, float y)
	{
		_velocity.x = x;
		_velocity.y = y;
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
		return _pos_left + 100;
	}
	
	float Player::getDepth()
	{
		return _pos_depth;
	}
	
	inet::PlayerState Player::getPlayerStruct()
	{
		inet::PlayerState player;
		player.dir[0] = _dir[0];
		player.dir[1] = _dir[1];
		player.left = _pos_left;
		player.depth = _pos_depth;
		player.elevation = _elevation;
		player.velocity[0] = _velocity.x;
		player.velocity[1] = _velocity.y;
		
		if ( player.elevation > 0 || _velocity.y > 0 )
			player.state = inet::STATE_JUMPING;
		else if ( _dir[0] != 0 || _dir[1] != 0 )
			player.state = inet::STATE_RUNNING;
		else
			player.state = inet::STATE_WAITING;
		
		return player;
	}
}
