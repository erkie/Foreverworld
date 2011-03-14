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
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/user.h"
#include "Gaem/entitymanager.h"
#include "Gaem/menumanager.h"
#include "Gaem/resourcemanager.h"

#include "Common/messages.h"
#include "GetTime.h"

#include "Entities/world.h"
#include "Entities/player.h"

#include "Entities/Attacks/EmanonAttack.h"
#include "Entities/Attacks/WarbirdAttack.h"
#include "Entities/Attacks/GhostAttack.h"
#include "Entities/Attacks/KooriAttack.h"
#include "Entities/Attacks/MaarnAttack.h"

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
		_flyingdir = 0;
		_velocity.x = 0;
		_velocity.y = 0;
		_defence = false;
		_hp = 1;
		_current_attack = NULL;
		_ghost_hit = false;
		
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
		_sprite->loadAnimation("damaged", info.get("damaged"));
		
		_scale = info.getFloat("scale", 1.0);
		_speed = info.getFloat("speed", 250);
		_speed_up = info.getFloat("up speed", 100);
		_can_jump = info.isTrue("can jump");
		_name = info.get("name");
		
		_sprite->setAnimation("waiting");
		_sprite->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _sprite->getWidth()/2);
		
		initAttacks();
		setDepth(_pos_depth);
	}
	
	Player::Player(const inet::Character &character)
	{
		init();
		
		setCharacter(character);
		
		_sprite->setAnimation("waiting");
		_sprite->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _sprite->getWidth()/2);
		
		initAttacks();
		setDepth(_pos_depth);
	}
	
	Player::~Player()
	{
		delete _sprite;
		
		for ( attack_map::iterator iter = _attacks.begin(); iter != _attacks.end(); iter++ )
		{
			delete (*iter).second;
		}
	}
	
	void Player::initAttacks()
	{
		// Remove old attacks
		for ( attack_map::iterator iter = _attacks.begin(); iter != _attacks.end(); iter++ )
			delete (*iter).second;
		_attacks.erase(_attacks.begin(), _attacks.end());
	
		Gaem::Attack *attack1 = new Gaem::Attack(_sprite->getAnimtion("attack1")->getConfig().get("attack"), this);
		attack1->setAnimation("attack1");
		
		std::string spec_attack_id = _sprite->getAnimtion("attack2")->getConfig().get("attack id");
		Gaem::Attack *attack2;
		
		if (spec_attack_id == "warbird_attack2")
		{
			attack2 = new Attacks::Warbird(_sprite->getAnimtion("attack2")->getConfig().get("attack"), this);
		}
		else if (spec_attack_id == "koori_attack2")
		{
			attack2 = new Attacks::Koori(_sprite->getAnimtion("attack2")->getConfig().get("attack"), this);
		}
		else if (spec_attack_id == "emanon_attack2")
		{
			attack2 = new Attacks::Emanon(_sprite->getAnimtion("attack2")->getConfig().get("attack"), this);
		}
		else if (spec_attack_id == "maarn_attack2")
		{
			attack2 = new Attacks::Maarn(_sprite->getAnimtion("attack2")->getConfig().get("attack"), this);
		}
		else if (spec_attack_id == "ghost_attack2")
		{
			attack2 = new Attacks::Ghost(_sprite->getAnimtion("attack2")->getConfig().get("attack"), this);
		}
		else
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not find special attack id");
		}
		
		attack2->setAnimation("attack2");
		
		_attacks[attack1->getID()] = attack1;
		_attacks[attack2->getID()] = attack2;
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
		_sprite->loadAnimation("attack1", "resources/players/" + slug + "/" + slug + "_attack1/info.txt");
		_sprite->loadAnimation("attack2", "resources/players/" + slug + "/" + slug + "_attack2/info.txt");
		_sprite->loadAnimation("defence", "resources/players/" + slug + "/" + slug + "_defence/info.txt");
		_sprite->loadAnimation("damaged", "resources/players/" + slug + "/" + slug + "_damaged/info.txt");
		_sprite->loadAnimation("ghost_hit", "resources/players/ghost/ghost_attack2/" + slug + "/info.txt");
		
		initAttacks();
	}
	
	bool Player::isActivePlayer()
	{
		return Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer() == this;
	}
	
	void Player::jump()
	{
		if ( _elevation == 0 && _can_jump )
		{
			if ( _current_attack && _current_attack->abortByJump() )
			{
				_current_attack->end();
				setCurrentAttack("");
			}
			
			_velocity.y = 500;
			_sprite->setAnimation("jumping");
			_sprite->getAnimation()->reset();
		}
	}
	
	void Player::runLeft()
	{
		if ( _elevation == 0 && (! _current_attack || _current_attack->hasOwnAnimation()) )
			_sprite->setAnimation("running");
		_sprite->flip(true);
		_dir[0] = -1;
		_pos_left -= _speed * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::runRight()
	{
		if ( _elevation == 0 && (! _current_attack || _current_attack->hasOwnAnimation()) )
			_sprite->setAnimation("running");
		_sprite->flip(false);
		_dir[0] = 1;
		_pos_left += _speed * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveUp()
	{
		if ( _elevation == 0 && (! _current_attack || _current_attack->hasOwnAnimation()) )
			_sprite->setAnimation("running");
		_pos_depth -= _speed_up * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::moveDown()
	{
		if ( _elevation == 0 && (! _current_attack || _current_attack->hasOwnAnimation()) )
			_sprite->setAnimation("running");
		_pos_depth += _speed_up * Gaem::Gaem::getInstance()->getTDelta();
	}
	
	void Player::iDied()
	{
		
	}
	
	void Player::hitBy(Player *player, Gaem::Attack *attack)
	{
		// Get length away from player to determine how hard he hit
		float deltaPosition = fabs(player->getLeft() - this->getLeft());
		float strength = 1;
		
		// 10 pixels away
		if ( deltaPosition < 20 )
		{
			strength = 0.75;
		}
		
		_hp -= attack->getDamage() * strength;
		if ( _hp < 0 )
		{
			iDied();
		}
		
		// Ghost has speciell hit characteristics
		if ( attack->getID() == "ghost_attack2" )
		{
			_ghost_hit = true;
			return;
		}
		
		_flyingdir = (player->getFacingDir() > 0 ? -1 : 1);
		_velocity.x = 300 * _flyingdir;
		_velocity.y = 200;
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
				
				if ( _elevation <= 0 )
				{
					for ( attack_map::iterator iter = _attacks.begin(); iter != _attacks.end(); iter++ )
					{
						if ( (*iter).second->handleAttack(event.Key.Code) )
						{
							setCurrentAttack((*iter).second->getID());
						}
					}
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
				input.IsKeyDown(sf::Key::A) || input.IsKeyDown(sf::Key::D)
				|| input.IsKeyDown(sf::Key::S) || input.IsKeyDown(sf::Key::W)
			) {
				// Set left running sprite and move player
				if ( input.IsKeyDown(sf::Key::A) && _dir[0] != -1 )
				{
					_dir[0] = -1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Set right running sprite and move player
				if ( input.IsKeyDown(sf::Key::D) && _dir[0] != 1 )
				{
					_dir[0] = 1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Move up
				if ( input.IsKeyDown(sf::Key::W) && _dir[1] != 1 )
				{
					_dir[1] = 1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				// Move down
				if ( input.IsKeyDown(sf::Key::S) && _dir[1] != -1 )
				{
					_dir[1] = -1;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				if ( ! input.IsKeyDown(sf::Key::S) && ! input.IsKeyDown(sf::Key::W) && _dir[1] != 0 )
				{
					_dir[1] = 0;
					Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
				}
				
				if ( ! input.IsKeyDown(sf::Key::A) && ! input.IsKeyDown(sf::Key::D) && _dir[0] != 0 )
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
			else if ( input.IsKeyDown(sf::Key::K) )
			{
				_sprite->setAnimation("defence");
				_defence = true;
				Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
			}
			
			if ( ! input.IsKeyDown(sf::Key::K) )
			{
				_defence = false;
			}
		}
		
		// If my direction right/left has been set to something else than 0, move me
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
		
		if ( _dir[0] == 0 && _dir[1] == 0 && _elevation <= 0 && _velocity.y <= 0 && ! _ghost_hit && ! _defence && (! _current_attack || _current_attack->hasOwnAnimation()) )
		{
			_sprite->setAnimation("waiting");
		}
		
		clampPos();
		
		// Update elevation and velocity based on gravity
		if ( _elevation > 0 || _velocity.y > 0 )
		{
			// == What is this doing here? ==
			// == why not???? ==
			// == because it is in jump()? ==
			if ( _current_attack && _current_attack->abortByJump()  )
			{
				_current_attack->end();
				setCurrentAttack("");
			}
			if ( _flyingdir == 0 )
				_sprite->setAnimation("jumping");
			_velocity.y -= 1000 * tdelta;
		}
		
		// Update hit flying
		if ( _flyingdir != 0 && ! _ghost_hit)
		{
			_velocity.x += 0;
			_sprite->setAnimation("damaged");
		}
		else if ( _ghost_hit )
		{
			_sprite->setAnimation("ghost_hit");
			
			if ( _sprite->getAnimation()->isDone() )
			{
				_ghost_hit = false;
			}
		}
		
		// Update position up based on velocity
		_elevation += _velocity.y * tdelta;
		if ( _elevation <= 0 )
		{
			_elevation = 0;
			_velocity.y = 0;
			
			if ( _flyingdir != 0 )
			{
				_flyingdir = 0;
				_velocity.x = 0;
			}
		}
		
		// Update position left/right based on velocity
		_pos_left += _velocity.x * tdelta;
		
		// Handle attacking
		if ( _current_attack )
		{
			_current_attack->step();
			
			// If the attack is still ongoing
			if ( ! _sprite->getAnimation()->isDone() || ! _current_attack->isDone() )
			{
				Gaem::player_hitlist hit_player = _current_attack->isHit(_sprite->getAnimation()->getFrameNum());
				if ( hit_player.size() )
				{
					// We hit somebody
					for ( Gaem::player_hitlist::iterator iter = hit_player.begin(); iter != hit_player.end(); iter++ )
					{
						(*iter)->hitBy(this, _current_attack);
						Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
					}
				}
			}
			else
			{
				_current_attack->end();
				setCurrentAttack("");
			}
		}
		
		// Increase HP
		if ( _hp < 1 )
		{
			_hp += 0.1 * tdelta;
			if ( _hp > 1 )
				_hp = 1;
		}
		
		setDepth(_pos_depth);
		
		_sprite->setY(game->getHeight() - world_height + _pos_depth - _sprite->getHeight() - _elevation);
		_sprite->setX(_pos_left - game->getEntityManager()->getWorld()->getScrollLeft());
		
		// Step animation
		_sprite->step();
	}
	
	void Player::draw(sf::RenderWindow &window)
	{
		float real_old_x = _sprite->getX();
		float x = real_old_x - _sprite->getWidth() / 2;
		
		// Draw two more versions of the player, one outside the field to the left
		// and one outside to the right, this so it does not disappear when the position
		// is clamped
		int scroll = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getScrollLeft();
		int width = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getWidth();
		
		float current_x = x - width - scroll;
		
		for ( int i = 0; i < 3; i++ )
		{
			_sprite->setX(current_x + scroll);
			
			if ( _current_attack )
				_current_attack->drawAttackAtBefore(_sprite->getRect(), window);
			window.Draw(*_sprite->getSprite());
			if ( _current_attack )
				_current_attack->drawAttackAtAfter(_sprite->getRect(), window);
			
			if ( _user )
			{
				sf::String name(_user->getUsername(), *Gaem::Gaem::getInstance()->getResourceManager()->getFont("resources/main_font.ttf", 15), 15);
				name.SetX(_sprite->getX() + _sprite->getWidth()/2 - name.GetRect().GetWidth()/2);
				name.SetY(_sprite->getY() - name.GetRect().GetHeight());
				
				window.Draw(name);
			}
			
			current_x += width;
		}
		
		_sprite->setX(real_old_x);
	}
	
	void Player::setAttack(std::string id, int RTT)
	{
		if ( id != "" )
		{
			bool is_current = _current_attack && _current_attack->getID() == id;
			if ( is_current ) return;
			
			_current_attack = _attacks[id];
			
			// UPDATE setAttack 2()
			if ( _current_attack->getID() != "warbird_attack2" )
				_sprite->setAnimation(_current_attack->getAnimation());

			if ( ! is_current )
			{
				_sprite->getAnimation()->setElapsedTime(RTT);
				_current_attack->setElapsedTime(RTT);
			}
		}
	}
	
	void Player::setCurrentAttack(const std::string &id)
	{
		if ( id != "" )
		{
			if ( _current_attack )
				_current_attack->end();
			
			_current_attack = _attacks[id];
			
			// UPDATE setAttack()
			if ( id != "warbird_attack2" )
				_sprite->setAnimation(_current_attack->getAnimation());
		}
		else
		{
			_current_attack = NULL;
		}
		
		Gaem::Gaem::getInstance()->getEntityManager()->sendUpdates();
	}
	
	void Player::setDir(int dir_x, int dir_y)
	{
		_dir[0] = dir_x;
		_dir[1] = dir_y;
	}
	
	void Player::setFlyingDir(int dir)
	{
		_flyingdir = dir;
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
	
	void Player::setHP(float hp)
	{
		_hp = hp;
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
	
	float Player::getWidth()
	{
		return _sprite->getWidth();
	}
	
	int Player::getDirX()
	{
		return _dir[0];
	}
	
	int Player::getDirY()
	{
		return _dir[1];
	}
	
	float Player::getHP()
	{
		return _hp;
	}
	
	int Player::getFacingDir()
	{
		return _sprite->isFlipped();
	}
	
	bool Player::isOn(sf::Vector2i pos, float depth)
	{
		return pos.x > getLeft() && pos.x < getRight() && (getDepth() - depth < 10 && getDepth() - depth > - 10);
	}
	
	inet::PlayerState Player::getPlayerStruct()
	{
		inet::PlayerState player;
		player.dir[0] = _dir[0];
		player.dir[1] = _dir[1];
		player.flyingdir = _flyingdir;
		player.left = _pos_left;
		player.depth = _pos_depth;
		player.elevation = _elevation;
		player.velocity[0] = _velocity.x;
		player.velocity[1] = _velocity.y;
		player.hp = _hp;
		strcpy(player.attackid, _current_attack ? _current_attack->getID().c_str() : "");
		
		if ( player.elevation > 0 || _velocity.y > 0 )
			player.state = inet::STATE_JUMPING;
		else if ( _dir[0] != 0 || _dir[1] != 0 )
			player.state = inet::STATE_RUNNING;
		else
			player.state = inet::STATE_WAITING;
		
		return player;
	}
	
	int Player::getZIndex()
	{
		return _pos_depth;
	}
	
	Gaem::AnimatedSprite *Player::getSprite()
	{
		return _sprite;
	}
}
