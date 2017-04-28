#pragma once
#include <SFML/Graphics/Text.hpp>

inline void centerOrigin(sf::Text& text) {
	auto bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

inline std::string textWrap(const std::string& str, const size_t lineWidth) {
	std::string result{ str };
	const auto strlen = result.length(); 
	//Begin parse
	//i: Current position on the string
	//lineDist: Current distance from last line break ('\n')
	//lineWidth: Desired max line width
	for (size_t i = 0, lineDist = 0, lastSpace = SIZE_MAX; i < strlen; i++, lineDist++) {
		switch (result[i]) {
			case '\n':{//if it's a new line we reset lineDist and clear lastSpace since it's no longer useful
				lineDist = 0;
				lastSpace = SIZE_MAX;
			}break;
			case ' ': {//if it's a space we save its position
				lastSpace = i;
			}break;
		}
		//if we are at or past the line limit and we have previously found a space
		// we replace said space with a line break
		if (lineDist >= lineWidth && lastSpace != SIZE_MAX) {
			result[lastSpace] = '\n';
			lineDist = i - lastSpace; //recalculate lineDist
			lastSpace = SIZE_MAX;//clear lastSpace
		}
	}
	return std::move(result);
	
}