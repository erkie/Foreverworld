/*
 *  developer.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-27.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <algorithm>
#include <guichan/guichan.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/entitymanager.h"

#include "Entities/player.h"
#include "Menus/developer.h"
#include "My/myutilities.h"

#include "Widgets/fixedlabel.h"
#include "Widgets/animationdemo.h"
#include "Widgets/image.h"
#include "Widgets/informationtable.h"

namespace Menus
{
	class DevHide: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			Gaem::Gaem::getInstance()->getMenuManager()->hide();
		}
	};

	/*
	 ===
		Animation Tab
	 ===
	*/

	// Fetch all available animations
	class DevAnimationModel: public gcn::ListModel
	{
	public:
		file_list _files;

		bool operator()(const std::string &name)
		{
			return !file_exists("resources/players/" + name + "/info.txt");
		}

		DevAnimationModel(): gcn::ListModel()
		{
			list_files("resources/players", _files);
			_files.erase(std::remove_if(_files.begin(), _files.end(), *this), _files.end());
		}

		std::string getElementAt(int i) { if ( i < 0 || i >= (int)_files.size() ) return ""; return _files[i]; }
		int getNumberOfElements() { return _files.size(); }
	};

	// Change the animation demo when clicked the list
	class DevAnimationSelection: public gcn::SelectionListener, public Gaem::Listener
	{
	public:
		Widgets::AnimationDemo *_demo;

		void valueChanged(const gcn::SelectionEvent &event)
		{
			gcn::ListBox *list = static_cast<gcn::ListBox*>(event.getSource());
			int index = list->getSelected();

			std::string selected = list->getListModel()->getElementAt(index);
			std::string name = "resources/players/" + selected + "/info.txt";

			if ( selected == "" || ! file_exists(name) )
				return;

			_demo->setAnimation(name);
		}

		void setDemo(Widgets::AnimationDemo *demo)
		{
			_demo = demo;
		}
	};

	// Change the scale of the animationdemo
	class DevScaleSlider: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			double scale = static_cast<gcn::Slider*>(event.getSource())->getValue();
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->setScale(scale);
		}
	};

	// Pause animations in the animationdemo
	class DevStopButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->stop();
		}
	};

	// Play paused animations in animationdemo
	class DevPlayButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->start();
		}
	};
	
	// Next frame for a paused animationdemo
	class DevNextButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->step();
		}
	};
	
	// Previous frame for a paused animationdemo
	class DevPrevButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->rewind();
		}
	};

	// Go to beginning for paused animationdemo
	class DevResetButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->reset();
		}
	};

	// Flip animation
	class DevFlipButton: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			static_cast<Widgets::AnimationDemo*>(Developer::instance->get("animation_demo"))->flip();
		}
	};

	/*
	 ===
		Player tab
	 ===
	*/
	
	// Fetch all available players
	class DevPlayerModel: public gcn::ListModel
	{
	public:
		file_list _files;
		
		bool operator()(const std::string &name)
		{
			return name.substr(name.length() - 4, name.length()) != ".txt";
		}
		
		DevPlayerModel(): gcn::ListModel()
		{
			list_files("resources/players", _files);
			_files.erase(std::remove_if(_files.begin(), _files.end(), *this), _files.end());
		}
		
		std::string getElementAt(int i) { if ( i < 0 || i >= (int)_files.size() ) return ""; return _files[i]; }
		int getNumberOfElements() { return _files.size(); }
	};

	// Change selected player when clicked the list
	class DevPlayerSelection: public gcn::SelectionListener, public Gaem::Listener
	{
	public:
		void valueChanged(const gcn::SelectionEvent &event)
		{
			gcn::ListBox *list = static_cast<gcn::ListBox*>(event.getSource());
			int index = list->getSelected();
			
			std::string selected = list->getListModel()->getElementAt(index);
			if ( selected == "" )
				return;
			
			Gaem::Config config("resources/players/" + selected);
			
			Widgets::InformationTable *table = static_cast<Widgets::InformationTable*>(Developer::instance->get("player_info"));
			table->setRow("Name", config.get("name"));
			table->setRow("Scale", config.get("scale"));
			table->setRow("Can jump?", config.get("can jump"));
			table->setRow("Speed", config.get("speed") + " px/sec");
			table->setRow("Up speed", config.get("up speed") + " px/sec");
			
			Developer::instance->setCurrentPlayer("resources/players/" + selected);
		}
	};
	
	class DevPlayerApply: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void action(const gcn::ActionEvent &event)
		{
			std::string path = Developer::instance->getCurrentPlayer();
			
			Gaem::EntityManager *manager = Gaem::Gaem::getInstance()->getEntityManager();
			
			manager->remove(manager->getCurrentPlayer());
			
			Entities::Player *player = new Entities::Player(path);
			manager->add(player);
			manager->setCurrentPlayer(player);
		}
	};

	/*
	 ===
		The menu itself
	 ===
	*/

	Developer* Developer::instance = NULL;

	Developer::~Developer()
	{
		delete _model;
	}

	void Developer::init()
	{
		instance = this;

		// Create base
		gcn::Window *base = newWidget<gcn::Window>("Hello, developer");
		base->setTitleBarHeight(25);
		base->setWidth(400);
		base->setHeight(400);

		// Global exit button
		gcn::Button *hide = newWidget<gcn::Button>("Hide");
		hide->setX(base->getChildrenArea().width - hide->getWidth() - 10);
		hide->setY(base->getChildrenArea().height - hide->getHeight() - 10);
		hide->addActionListener(newListener<DevHide>());
		base->add(hide);

		// Create tabs
		_tabs = newWidget<gcn::TabbedArea>();
		_tabs->setSize(base->getChildrenArea().width, base->getChildrenArea().height - hide->getHeight() - 20);
		base->add(_tabs);

		initHome();
		initAnimations();
		initPlayers();

		setRoot(base);
		centerRoot();
	}
	
	void Developer::show()
	{
		centerRoot();
	}

	void Developer::initHome()
	{
		gcn::Container *home_container = makeContainer();

		int field_width = home_container->getChildrenArea().width - 10 * 2;

		Widgets::FixedLabel *welcome = newWidget<Widgets::FixedLabel>();
		welcome->setCaption("Welcome woeful developer. Be sure to keep your party hat on and start rocking out new features for our game. Herp derp, derp de derp herp derp. Don't ever stop herping!");
		welcome->setWidth(home_container->getWidth() - 20);
		welcome->adjustHeight();
		welcome->setX(10);
		welcome->setY(10);

		Widgets::Image *image = newWidget<Widgets::Image>();
		image->setImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/win.jpg")));
		image->setX(10);
		image->setY(welcome->getBottom() + 10);
		image->setWidth(field_width);
		image->setHeight(300);

		home_container->add(welcome);
		home_container->add(image);

		_tabs->addTab("Home", home_container);
	}

	void Developer::initAnimations()
	{
		gcn::Container *anim_container = makeContainer();

		int field_width = anim_container->getChildrenArea().width - 10 * 2;

		// Explanatory text
		Widgets::FixedLabel *paragraph1 = newWidget<Widgets::FixedLabel>();
		paragraph1->setPosition(10, 10);
		paragraph1->setCaption("Pick an animation to try out. Go on, have a go.");
		paragraph1->setWidth(field_width);
		paragraph1->adjustHeight();

		// Create select list
		gcn::ScrollArea *scroll = newWidget<gcn::ScrollArea>();
		scroll->setX(10);
		scroll->setY(paragraph1->getBottom() + 10);
		scroll->setWidth(150);
		scroll->setHeight(225);
		scroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

		gcn::ListModel *listmodel = _model = new DevAnimationModel;

		gcn::ListBox *listbox = newWidget<gcn::ListBox>();
		listbox->setListModel(listmodel);
		listbox->setWidth(scroll->getWidth() - scroll->getScrollbarWidth());

		scroll->setContent(listbox);

		// Demo as animation
		Widgets::AnimationDemo *demo = newNamedWidget<Widgets::AnimationDemo>("animation_demo");
		demo->setWidth((anim_container->getChildrenArea().width - scroll->getWidth() - 30));
		demo->setHeight(197);
		demo->setX(scroll->getRight() + 10);
		demo->setY(scroll->getY());

		DevAnimationSelection *event = newListener<DevAnimationSelection>();
		event->setDemo(demo);
		listbox->addSelectionListener(event);

		// Create slider for scale
		gcn::Slider *slider = newNamedWidget<gcn::Slider>("scale_slider");
		slider->setScale(1.0, 10.0);
		slider->setValue(1.0);
		slider->setWidth(demo->getWidth());
		slider->setHeight(15);
		slider->setX(demo->getX());
		slider->setY(demo->getBottom() + 10);
		slider->addActionListener(newListener<DevScaleSlider>());


		// == Toolbar ==

		int toolbar_y = scroll->getBottom() + 10;

		// Reverse sprite button
		gcn::Button *reverse_button = newWidget<gcn::Button>("Flip");
		reverse_button->setX(anim_container->getChildrenArea().width - reverse_button->getWidth() - 10);
		reverse_button->setY(toolbar_y);
		reverse_button->addActionListener(newListener<DevFlipButton>());

		// Reset frames button
		gcn::Button *reset_button = newWidget<gcn::Button>("Reset");
		reset_button->setX(reverse_button->getX() - 10 - reset_button->getWidth());
		reset_button->setY(toolbar_y);
		reset_button->addActionListener(newListener<DevResetButton>());

		// Next frame button
		gcn::Button *next_button = newWidget<gcn::Button>("Next");
		next_button->setX(reset_button->getX() - 10 - next_button->getWidth());
		next_button->setY(toolbar_y);
		next_button->addActionListener(newListener<DevNextButton>());
		
		// Previous frame button
		gcn::Button *prev_button = newWidget<gcn::Button>("Prev");
		prev_button->setX(next_button->getX() - 10 - prev_button->getWidth());
		prev_button->setY(toolbar_y);
		prev_button->addActionListener(newListener<DevPrevButton>());

		// Stop button
		gcn::Button *stop_button = newWidget<gcn::Button>("Stop");
		stop_button->setX(prev_button->getX() - 10 - stop_button->getWidth());
		stop_button->setY(toolbar_y);
		stop_button->addActionListener(newListener<DevStopButton>());

		// Play button
		gcn::Button *play_button = newWidget<gcn::Button>("Play");
		play_button->setX(stop_button->getX() - 10 - play_button->getWidth());
		play_button->setY(toolbar_y);
		play_button->addActionListener(newListener<DevPlayButton>());

		anim_container->add(paragraph1);
		anim_container->add(scroll);
		anim_container->add(reverse_button);
		anim_container->add(reset_button);
		anim_container->add(next_button);
		anim_container->add(prev_button);
		anim_container->add(stop_button);
		anim_container->add(play_button);
		anim_container->add(demo);
		anim_container->add(slider);

		_tabs->addTab("Animations", anim_container);
	}

	void Developer::initPlayers()
	{
		gcn::Container *players_container = makeContainer();

		int field_width = players_container->getChildrenArea().width - 2 * 10;
		int field_height = players_container->getChildrenArea().height;

		Widgets::FixedLabel *text = newWidget<Widgets::FixedLabel>("Try out your players. (Make sure they rock the house)");
		text->setWidth(field_width);
		text->setX(10);
		text->setY(10);
		text->adjustHeight();
		players_container->add(text);
		
		// Create select list
		gcn::ScrollArea *scroll = newWidget<gcn::ScrollArea>();
		scroll->setX(10);
		scroll->setY(text->getBottom() + 10);
		scroll->setWidth(150);
		scroll->setHeight(field_height - text->getBottom() - 20);
		scroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
				
		gcn::ListModel *model = new DevPlayerModel;
		gcn::ListBox *listbox = newWidget<gcn::ListBox>();
		listbox->setListModel(model);
		listbox->setWidth(scroll->getWidth() - scroll->getScrollbarWidth());
		listbox->addSelectionListener(newListener<DevPlayerSelection>());
		
		// Create set active player button
		gcn::Button *set_player = newWidget<gcn::Button>("Set player");
		set_player->setX(scroll->getRight() + 10);
		set_player->setY(scroll->getBottom() - set_player->getHeight());
		set_player->addActionListener(newListener<DevPlayerApply>());
		
		// Create table for information
		Widgets::InformationTable *table = newNamedWidget<Widgets::InformationTable>("player_info");
		table->setX(scroll->getRight() + 10);
		table->setY(scroll->getY());
		table->setWidth(field_width - scroll->getRight());
		table->setRow("Name", "");
		table->setRow("Scale", "");
		table->setRow("Can jump?", "");
		table->setRow("Speed", "");
		table->setRow("Up speed", "");
		table->adjustHeight();
		
		scroll->setContent(listbox);
		
		players_container->add(scroll);
		players_container->add(set_player);
		players_container->add(table);

		_tabs->addTab("Players", players_container);
	}
	
	void Developer::setCurrentPlayer(const std::string &path)
	{
		_current_player = path;
	}
	
	std::string Developer::getCurrentPlayer()
	{
		return _current_player;
	}

	gcn::Container *Developer::makeContainer()
	{
		// Get height of tabs because getChildrenArea is faulty in gcn::TabbedArea
		gcn::Tab temp;
		temp.setCaption("Foo");
		temp.adjustSize();

		int height = temp.getHeight();

		gcn::Container *container = newWidget<gcn::Container>();
		container->setSize(_tabs->getChildrenArea().width, _tabs->getChildrenArea().height - height);
		return container;
	}
}
