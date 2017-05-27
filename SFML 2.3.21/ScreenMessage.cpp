#include "ScreenMessage.h"
#include <SFML/Graphics/RenderTarget.hpp>
void ScreenMessage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	
	target.draw(this->m_text,states);
}

void ScreenMessage::updateText()
{
	
	std::string formated = m_string;
	/*
	if (!m_string.empty()) {
		size_t covered_string = 0;
		size_t next_break;
		size_t line_length;
		do {
			//find line break
			next_break = formated.find("\n", covered_string);
			//calculate the length of the line
			line_length = (next_break == std::string::npos ? formated.size() : next_break) - covered_string;
			
			//if the line is bigger than it should be
			if(line_length>m_line_width){
				while (line_length > m_line_width) {
					//look for last space in the line (to check if we can break the line there)
					size_t previous_space = formated.find_last_of(' ', line_length + covered_string - 1);
					//if it exists and is in the current line
					if (previous_space != std::string::npos && previous_space > covered_string) {
						//assign the line length to it
						line_length = previous_space - covered_string;
					}
					else {//if it doesn't exist or belongs to a previous line then fuck it

						break;
					}
				}
				//break line on the space found (or line break if we found nothin)
				formated[covered_string + line_length] = '\n';
			}
			covered_string += line_length;//the line length is covered
		} while (line_length + covered_string != formated.size());// while a line break exists
	}
	*/
	m_text.setString(formated);
	
}

void ScreenMessage::updateBox()
{
	auto bounds = m_text.getGlobalBounds();
	centerOrigin(m_text);
	//Stars0.setSize(sf::Vector2f(bounds.width + 20, bounds.height + 20));
}
