/*
 *  error.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <SFML/Network.hpp>
#include <guichan/guichan.hpp>

#include "Gaem/gaem.h"
#include "Gaem/menumanager.h"

#include "Menus/error.h"

namespace Menus
{
	class ErrorOK: public gcn::ActionListener, public Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			sf::Http conn;
			conn.SetHost("spel.glonk.se");
			
			// Send crash report
			sf::Http::Request request;
			request.SetMethod(sf::Http::Request::Post);
			request.SetURI("/crashreport.php");
			
			request.SetBody(Gaem::Gaem::getInstance()->getErrorPost());
			conn.SendRequest(request, 15).GetBody();
			
			// Kill application
			Gaem::Gaem::getInstance()->quitError();
		}
	};
	
	class ErrorCancel: public gcn::ActionListener, public Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			// Kill application
			Gaem::Gaem::getInstance()->quitError();
		}
	};

	Error::Error()
	{
		_message = "An error has occured. Send a crash report (along with some additional data about the crash) so we can analyze it? Please?";
		_oktext = "Yes";
		_canceltext = "No";
	}
	
	void Error::init()
	{
		Confirm::init();
		
		gcn::Button *ok, *cancel;
		ok = get<gcn::Button>("ok_button");
		cancel = get<gcn::Button>("cancel_button");
		
		ok->addActionListener(new ErrorOK());
		cancel->addActionListener(new ErrorCancel());
		
		static_cast<gcn::Window*>(_root)->setCaption("Oops");
	}
}
