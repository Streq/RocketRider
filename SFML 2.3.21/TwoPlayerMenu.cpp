#include "TwoPlayerMenu.h"

TwoPlayerMenu::TwoPlayerMenu(GameStack & s, AppContext c)
	:Menu(s,c)
{
	load_levels_from_xml(this->maps, "Assets/Config/config2players.xml");
	GUI::TextBox::ptr titulo(new GUI::TextBox(c,"2 Jugadores", 20u));
	addElement(std::move(titulo),0.f,0.f,false);

}
