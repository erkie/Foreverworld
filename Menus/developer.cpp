/*
 *  developer.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-27.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <algorithm>

#include <Gaem/gaem.h>
#include <guichan/guichan.hpp>

#include "Menus/developer.h"
#include "My/myutilities.h"

#include "Widgets/fixedlabel.h"
#include "Widgets/animationdemo.h"

namespace Menus
{
	class DevAnimationModel: public gcn::ListModel
	{
	public:
		file_list _files;
		
		bool operator()(const std::string &name)
		{
			return file_exists("resources/players/" + name + "/info.txt");
		}
		
		DevAnimationModel(): gcn::ListModel()
		{
			list_files("resources/players", _files);
			_files.erase(std::remove_if(_files.begin(), _files.end(), *this));
		}
		
		std::string getElementAt(int i) { return _files[i]; }
		int getNumberOfElements() { return _files.size(); }
	};
	
	class DevPlayerModel: public gcn::ListModel
	{
	public:
		file_list _files;
		
		bool operator()(const std::string &name)
		{
			return name.substr(-4) == ".txt";
		}
		
		DevPlayerModel(): gcn::ListModel()
		{
			list_files("resources/players", _files);
			_files.erase(std::remove_if(_files.begin(), _files.end(), *this));
		}
		
		std::string getElementAt(int i) { return _files[i]; }
		int getNumberOfElements() { return _files.size(); }
	};
	
	class DevAnimationSelection: public gcn::ActionListener, public Gaem::Listener
	{
	public:
		Widgets::AnimationDemo *_demo;
		
		void action(const gcn::ActionEvent &event)
		{
			gcn::ListBox *list = static_cast<gcn::ListBox*>(event.getSource());
			int index = list->getSelected();
			
			std::string selected = list->getListModel()->getElementAt(index);
			std::string name = "resources/players/" + selected + "/info.txt";
			
			if ( ! file_exists(name) )
				return;
			
			_demo->setAnimation(name);
		}
		
		void setDemo(Widgets::AnimationDemo *demo)
		{
			_demo = demo;
		}
	};
	
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
	
	void Developer::initHome()
	{
		gcn::Container *home_container = makeContainer();
		
		Widgets::FixedLabel *welcome = newWidget<Widgets::FixedLabel>();
		welcome->setCaption("Welcome woeful developer. Be sure to keep your party hat on and start rocking out new features for our game. Herp derp, derp de derp herp derp. Don't ever stop herping!");
		welcome->setWidth(home_container->getWidth() - 20);
		welcome->adjustHeight();
		welcome->setX(10);
		welcome->setY(10);
		
		home_container->add(welcome);
		
		_tabs->addTab("Home", home_container);
	}
	
	void Developer::initAnimations()
	{
		gcn::Container *anim_container = makeContainer();
		
		int field_width = anim_container->getChildrenArea().width - 10 * 2;
		
		// Explanatory text
		Widgets::FixedLabel *paragraph1 = newWidget<Widgets::FixedLabel>();
		paragraph1->setPosition(10, 10);
		paragraph1->setCaption("Pick an animation to try out. Go on, have go.");
		paragraph1->setWidth(field_width);
		paragraph1->adjustHeight();
		
		// == Toolbar ==
		
		// Reverse sprite button
		gcn::Button *reverse_button = newWidget<gcn::Button>("Reverse");
		reverse_button->setX(10);
		reverse_button->setY(paragraph1->getBottom() + 10);
		
		// Next frame button
		gcn::Button *next_button = newWidget<gcn::Button>("Next");
		next_button->setX(field_width - next_button->getWidth());
		next_button->setY(reverse_button->getY());
		
		// Play button
		gcn::Button *play_button = newWidget<gcn::Button>("Play");
		play_button->setX(next_button->getX() - 10 - play_button->getWidth());
		play_button->setY(reverse_button->getY());
		
		// Play once
		gcn::CheckBox *play_once = newWidget<gcn::CheckBox>("Play once");
		play_once->adjustSize();
		play_once->setX(play_button->getX() - 10 - play_once->getWidth());
		play_once->setY(reverse_button->getY() + play_once->getHeight()/4);
		
		// Create select list
		gcn::ScrollArea *scroll = newWidget<gcn::ScrollArea>();
		scroll->setX(10);
		scroll->setY(reverse_button->getBottom() + 10);
		scroll->setWidth(150);
		scroll->setHeight(180);
		scroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
		
		gcn::ListModel *listmodel = _model = new DevAnimationModel; // ADD DESTRUCTION
		
		gcn::ListBox *listbox = newWidget<gcn::ListBox>();
		listbox->setListModel(listmodel);
		listbox->setWidth(scroll->getWidth() - scroll->getScrollbarWidth());
		
		scroll->setContent(listbox);
		
		// Demo as animation
		Widgets::AnimationDemo *demo = newWidget<Widgets::AnimationDemo>();
		demo->setWidth(140);
		demo->setHeight(140);
		demo->setX(scroll->getRight() + (anim_container->getChildrenArea().width - listbox->getWidth() - 20)/2 - demo->getWidth() / 2);
		demo->setY(scroll->getY());
		
		DevAnimationSelection *event = newListener<DevAnimationSelection>();
		event->setDemo(demo);
		listbox->addActionListener(event);
		
		// Create slider for scale
		gcn::Slider *slider = newWidget<gcn::Slider>();
		slider->setScale(1.0, 10.0);
		slider->setValue(1.0);
		slider->setWidth(demo->getWidth());
		slider->setHeight(20);
		slider->setX(demo->getX());
		slider->setY(demo->getBottom() + 10);
		
		anim_container->add(paragraph1);
		anim_container->add(scroll);
		anim_container->add(reverse_button);
		anim_container->add(next_button);
		anim_container->add(play_once);
		anim_container->add(play_button);
		anim_container->add(demo);
		anim_container->add(slider);
		
		_tabs->addTab("Animations", anim_container);
	}
	
	void Developer::initPlayers()
	{
		gcn::Container *players_container = makeContainer();
		
		_tabs->addTab("Players", players_container);
	}
	
	void Developer::show()
	{
		centerRoot();
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
