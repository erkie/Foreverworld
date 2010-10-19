/*
 *  main.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "Menus/main.h"

#include "Gaem/gaem.h"

#include "Widgets/passwordfield.h"
#include "Widgets/fixedlabel.h"

namespace Menus
{
	class MainLoginSubmit: public gcn::ActionListener, public ::Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			Gaem::Gaem::getInstance()->getMenuManager()->alert("Sending the credentials over an unsecure connection, please wait.");
			//Gaem::Gaem::getInstance()->getUserManager()->login(username, password);
		}
	};
	
	class MainHideMenu: public gcn::ActionListener, public ::Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			Gaem::Gaem::getInstance()->getMenuManager()->hide();
		}
	};
	
	class MainSignupSubmit: public gcn::ActionListener, public ::Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			Gaem::Gaem::getInstance()->getMenuManager()->alert("Welcome, you douche!");
			// Gaem::Gaem::getInstance()->getUserManager()->signup(username, password, email);
		}
	};
	
	class MainFuckYou: public gcn::ActionListener, public ::Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			system("open http://trololololololololololo.com/");
		}
	};
	
	void Main::init()
	{
		gcn::Window *r = newWidget<gcn::Window>("Foreverworld");
		r->setTitleBarHeight(25);
		r->setDimension(gcn::Rectangle(0, 0, 300, 300));
		r->setPosition(50, 50);
		_root = static_cast<gcn::Container*>(r);
		
		centerRoot();
		
		// Create the tabs
		_tabs = newWidget<gcn::TabbedArea>();
		_tabs->setSize(_root->getChildrenArea().width, _root->getChildrenArea().height);
		_root->add(_tabs);
		
		initLogin();
		initSignup();
		initAbout();
		
		setRoot(_root);
	}
	
	//
	// Login
	//
	
	void Main::initLogin()
	{
		gcn::Container *login_container = makeContainer();
		
		int field_width = login_container->getChildrenArea().width - 10 * 2;
		
		Widgets::FixedLabel *welcome = newWidget<Widgets::FixedLabel>("Welcome traveler! Ready to take on the world once again?");
		welcome->setWidth(field_width);
		welcome->adjustHeight();
		welcome->setPosition(10, 10);
		
		// Username
		gcn::Label *username = newWidget<gcn::Label>("Username:");
		username->setPosition(10, welcome->getY() + welcome->getHeight() + 10);
		
		gcn::TextField *inp_username = newWidget<gcn::TextField>("");
		inp_username->setPosition(10, username->getY() + username->getHeight() + 5);
		inp_username->setWidth(field_width);
		
		// Password
		gcn::Label *password = newWidget<gcn::Label>("Password:");
		password->setPosition(10, inp_username->getY() + inp_username->getHeight() + 10);
		
		Widgets::PasswordField *inp_password = newWidget<Widgets::PasswordField>("");
		inp_password->setPosition(10, password->getY() + password->getHeight() + 5);
		inp_password->setWidth(field_width);
		
		// Submit
		gcn::Button *button = newWidget<gcn::Button>("Log in");
		button->setX(login_container->getWidth() - button->getWidth() - 10);
		button->setY(login_container->getHeight() - button->getHeight() - 10);
		
		// Hide button
		gcn::Button *hide = newWidget<gcn::Button>("Hide");
		hide->setX(button->getX() - hide->getWidth() - 10);
		hide->setY(button->getY());
		
		button->addActionListener(newListener<MainLoginSubmit>());
		hide->addActionListener(newListener<MainHideMenu>());
		
		login_container->add(button);
		login_container->add(hide);
		login_container->add(inp_password);
		login_container->add(inp_username);
		
		login_container->add(password);
		login_container->add(username);
		login_container->add(welcome);
		
		_tabs->addTab("Login", login_container);
	}
	
	//
	// Signup
	//
	
	void Main::initSignup()
	{
		gcn::Container *signup_container = makeContainer();
		
		int field_width = signup_container->getChildrenArea().width - 10 * 2;
		
		// Username
		gcn::Label *username = newWidget<gcn::Label>("Username:");
		username->setPosition(10, 10);

		gcn::TextField *inp_username = newWidget<gcn::TextField>("");
		inp_username->setPosition(10, username->getY() + username->getHeight() + 5);
		inp_username->setWidth(field_width);
		
		// Password
		gcn::Label *password = newWidget<gcn::Label>("Password:");
		password->setPosition(10, inp_username->getY() + inp_username->getHeight() + 10);
		
		Widgets::PasswordField *inp_password = newWidget<Widgets::PasswordField>("");
		inp_password->setPosition(10, password->getY() + password->getHeight() + 5);
		inp_password->setWidth(field_width);
		
		// Repeat password
		gcn::Label *repeat_password = newWidget<gcn::Label>("Repeat password:");
		repeat_password->setPosition(10, inp_password->getY() + inp_password->getHeight() + 10);
		
		Widgets::PasswordField *inp_repeat_password = newWidget<Widgets::PasswordField>("");
		inp_repeat_password->setPosition(10, repeat_password->getY() + repeat_password->getHeight() + 5);
		inp_repeat_password->setWidth(field_width);
		
		// Email
		gcn::Label *email = newWidget<gcn::Label>("Email");
		email->setPosition(10, inp_repeat_password->getY() + inp_repeat_password->getHeight() + 10);
		
		gcn::TextField *inp_email = newWidget<gcn::TextField>("");
		inp_email->setPosition(10, email->getY() + email->getHeight() + 5);
		inp_email->setWidth(field_width);
		
		// Submit
		gcn::Button *submit = newWidget<gcn::Button>("Create account");
		submit->setX(signup_container->getWidth() - submit->getWidth() - 10);
		submit->setY(signup_container->getHeight() - submit->getHeight() - 10);
		
		submit->addActionListener(newListener<MainSignupSubmit>());
		
		signup_container->add(submit);
		signup_container->add(inp_email);
		signup_container->add(inp_repeat_password);
		signup_container->add(inp_password);
		signup_container->add(inp_username);
		
		signup_container->add(username);
		signup_container->add(password);
		signup_container->add(repeat_password);
		signup_container->add(email);
		
		_tabs->addTab("Signup", signup_container);
	}
	
	//
	// About
	//
	
	void Main::initAbout()
	{
		gcn::Container *about_container = makeContainer();
		
		int field_width = about_container->getChildrenArea().width - 10 * 2;
		
		Widgets::FixedLabel *paragraph1 = newWidget<Widgets::FixedLabel>();
		paragraph1->setCaption("Foreverworld is a retarded game made by us. We hate you. Please go away.");
		paragraph1->setPosition(10, 10);
		paragraph1->setWidth(field_width);
		paragraph1->adjustHeight();
		
		Widgets::FixedLabel *paragraph2 = newWidget<Widgets::FixedLabel>();
		paragraph2->setCaption("We will undermine your privacy as much as possible and harvest whatever data possible.");
		paragraph2->setPosition(10, paragraph1->getY() + paragraph1->getHeight() + 10);
		paragraph2->setWidth(field_width);
		paragraph2->adjustHeight();
		
		Widgets::FixedLabel *paragraph3 = newWidget<Widgets::FixedLabel>();
		paragraph3->setCaption("You are an idiot if you want to play this game. We won't play it.");
		paragraph3->setPosition(10, paragraph2->getY() + paragraph2->getHeight() + 10);
		paragraph3->setWidth(field_width);
		paragraph3->adjustHeight();
		
		gcn::Button *stupid = newWidget<gcn::Button>("Fuck you");
		stupid->adjustSize();
		stupid->setX(about_container->getWidth() - stupid->getWidth() - 10);
		stupid->setY(about_container->getHeight() - stupid->getHeight() - 10);
		
		stupid->addActionListener(newListener<MainFuckYou>());
		
		about_container->add(paragraph1);
		about_container->add(paragraph2);
		about_container->add(paragraph3);
		about_container->add(stupid);
		
		_tabs->addTab("About", about_container);
	}
	
	void Main::deinit()
	{
		
	}
	
	gcn::Container *Main::makeContainer()
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
