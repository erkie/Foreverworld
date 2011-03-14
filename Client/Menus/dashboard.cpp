/*
 *  dashboard.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-25.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */
 
#include "Gaem/gaem.h"
#include "Gaem/user.h"
#include "Gaem/menumanager.h"
#include "Gaem/entitymanager.h"
#include "Gaem/networkmanager.h"
#include "Gaem/resourcemanager.h"

#include "Common/messages.h"

#include "Widgets/animationdemo.h"
#include "Widgets/fixedlabel.h"
#include "Widgets/superfuntime.h"

#include "Menus/dashboard.h"

namespace Menus
{
	// Fetch all available animations
	class DashboardSelectModel: public gcn::ListModel
	{
	public:
		DashboardSelectModel(): gcn::ListModel() {}
		
		std::string getElementAt(int i)
		{
			return Gaem::Gaem::getInstance()->getEntityManager()->getCharacters()[i].name;
		}
		
		inet::Character getCharacterAt(int i)
		{
			return Gaem::Gaem::getInstance()->getEntityManager()->getCharacters()[i];
		}

		int getIndexById(int32_t id)
		{
			Gaem::character_vector v = Gaem::Gaem::getInstance()->getEntityManager()->getCharacters();
			int i = 0;
			for ( Gaem::character_vector::iterator iter = v.begin(); iter != v.end(); iter++, i++ )
			{
				if ( (*iter).id == id )
					return i;
			}
			return -1;
		}

		int getNumberOfElements() { return Gaem::Gaem::getInstance()->getEntityManager()->getCharacters().size(); }
	};
	
	// Set active animation
	class DashboardSetCharacter: public gcn::SelectionListener, public Gaem::Listener
	{
	public:
		void valueChanged(const gcn::SelectionEvent &event)
		{
			gcn::ListBox *list = static_cast<gcn::ListBox*>(event.getSource());
			int index = list->getSelected();
			
			inet::Character character = static_cast<DashboardSelectModel*>(list->getListModel())->getCharacterAt(index);
			
			std::stringstream ss;
			ss << "resources/players/" << character.slug << "/" << character.slug << "_walking/info.txt";
			std::string name = ss.str();
			
			static_cast<Widgets::AnimationDemo*>(Dashboard::instance->get("choose_player_demo"))->setAnimation(name);
		}
	};
	
	// Save active character
	class DashboardSaveCharacter: public gcn::ActionListener, public Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			// AHAHAHAHAHAHAHAHA, C++, you be trollin'
			// Get the listbox for the character selection (and cast it to the correct type)
			// Then get the selected character at the position which is selected
			gcn::ListBox *listbox = static_cast<gcn::ListBox*>(Dashboard::instance->get("player_select"));
			int32_t id = static_cast<DashboardSelectModel*>(listbox->getListModel())->getCharacterAt(listbox->getSelected()).id;
			
			Gaem::Gaem::getInstance()->getUser()->saveCharacter(id);
		}
	};
	
	Dashboard *Dashboard::instance = NULL;
	
	void Dashboard::init()
	{
		Dashboard::instance = this;
		
		// The window
		gcn::Window *base = newWidget<gcn::Window>();
		base->setTitleBarHeight(25);
		base->setSize(400, 400);
		
		// The tabs
		_tabs = newWidget<gcn::TabbedArea>();
		_tabs->setSize(base->getChildrenArea().width, base->getChildrenArea().height);
		
		base->add(_tabs);
		
		initHome();
		initStats();
		initSettings();
		initAbout();
		
		setRoot(base);
		centerRoot();
	}
	
	void Dashboard::show()
	{
		centerRoot();
		
		static_cast<gcn::Window*>(_root)->setCaption("Welcome, " + Gaem::Gaem::getInstance()->getUser()->getUsername());
		
		gcn::ListBox* listbox = static_cast<gcn::ListBox*>(get("player_select"));
		DashboardSelectModel *model = static_cast<DashboardSelectModel*>(listbox->getListModel());
		listbox->setSelected(model->getIndexById(Gaem::Gaem::getInstance()->getUser()->getMember().character_id));
		
		Widgets::FixedLabel *news = static_cast<Widgets::FixedLabel*>(get("news_box"));
		news->setCaption(Gaem::Gaem::getInstance()->getNetworkManager()->getNewsText());
		news->adjustHeight();
	}
	
	void Dashboard::initHome()
	{
		gcn::Container *container = makeContainer();
		
		int field_width = container->getChildrenArea().width - 10 * 2;
			
		Widgets::FixedLabel *intro = newWidget<Widgets::FixedLabel>("Welcome back! Who do you want to play as today?");
		intro->setWidth(field_width);
		intro->adjustHeight();
		intro->setPosition(10, 10);
		
		Widgets::AnimationDemo *demo = newNamedWidget<Widgets::AnimationDemo>("choose_player_demo");
		demo->setWidth(field_width - 150 - 10);
		demo->setHeight(150);
		demo->setX(container->getChildrenArea().width - demo->getWidth() - 10);
		demo->setY(intro->getBottom() + 10);
		demo->setShowFrames(false);
		
		// Select player list box
		gcn::ScrollArea *scroll = newWidget<gcn::ScrollArea>();
		scroll->setWidth(150);
		scroll->setHeight(150);
		scroll->setPosition(demo->getX() - scroll->getWidth() - 10, intro->getBottom() + 10);
		scroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
		
		gcn::ListModel *model = new DashboardSelectModel;
		
		gcn::ListBox *listbox = newNamedWidget<gcn::ListBox>("player_select");
		listbox->setListModel(model);
		listbox->setWidth(scroll->getWidth() - scroll->getScrollbarWidth());
		listbox->addSelectionListener(newListener<DashboardSetCharacter>());
		
		scroll->setContent(listbox);
		
		// Change player button
		gcn::Button *button = newWidget<gcn::Button>("Change character");
		button->setX(demo->getRight() - button->getWidth() - 10);
		button->setY(demo->getBottom() - button->getHeight() - 10);
		button->addActionListener(newListener<DashboardSaveCharacter>());
		
		// News box
		gcn::ScrollArea *newsbox = newNamedWidget<gcn::ScrollArea>("news_scroll");
		newsbox->setX(10);
		newsbox->setY(demo->getBottom() + 10);
		newsbox->setWidth(field_width);
		newsbox->setOpaque(false);
		newsbox->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
		newsbox->setHeight(container->getChildrenArea().height - newsbox->getBottom() - 10);
		
		// News
		Widgets::FixedLabel *news = newNamedWidget<Widgets::FixedLabel>("news_box");
		news->setCaption("hej");
		news->setWidth(field_width - 2*20);
		news->setX(10);
		news->setY(10);
		news->adjustHeight();

		newsbox->setContent(news);
		
		container->add(intro);
		container->add(demo);
		container->add(scroll);
		container->add(button);
		container->add(newsbox);
		
		_tabs->addTab("Home", container);
	}
	
	void Dashboard::initStats()
	{
		gcn::Container *container = makeContainer();
		
		//int field_width = container->getChildrenArea().width - 10 * 2;
		gcn::Label *label = newWidget<gcn::Label>("Not yet, sorry");
		label->setPosition(10, 10);
		container->add(label);
		
		_tabs->addTab("Statistics", container);
	}
	
	void Dashboard::initSettings()
	{
		gcn::Container *container = makeContainer();
		
		//int field_width = container->getChildrenArea().width - 10 * 2;
		gcn::Label *label = newWidget<gcn::Label>("Hello world.");
		label->setPosition(10, 10);
		container->add(label);
		
		_tabs->addTab("Settings", container);
	}
	
	void Dashboard::initAbout()
	{
		gcn::Container *container = makeContainer();
		
		int field_width = container->getChildrenArea().width - 10 * 2;
		
		Widgets::SuperFunTime *fun = newWidget<Widgets::SuperFunTime>();
		fun->setWidth(field_width);
		fun->setHeight(container->getChildrenArea().height - 10 * 2);
		fun->setPosition(10, 10);
		
		for ( int i = 0; i < 5; i++ )
		{
			fun->addImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/wow.png")));
			fun->addImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/wow2.png")));
			fun->addImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/wow3.png")));
			fun->addImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/wow4.png")));
			fun->addImage(new gcn::SFMLImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/wow5.png")));
		}
		
		container->add(fun);
		
		_tabs->addTab("About", container);
	}
	
	gcn::Container *Dashboard::makeContainer()
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
