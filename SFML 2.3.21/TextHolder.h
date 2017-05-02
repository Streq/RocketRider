#pragma once
#include "ResourceHolder.h"
#include <string>
#include <fstream>
namespace TextFile{
enum ID{
	MAP_DEF,
	CONFIG,
	HOWTOPLAY,
	CREDITS
};
}


typedef ResourceHolder<std::string,TextFile::ID> TextHolder;
template<>
void TextHolder::load(TextFile::ID id, const std::string& filename);

template<>
std::string& TextHolder::get(TextFile::ID id);

template<>
const std::string& TextHolder::get(TextFile::ID id) const;