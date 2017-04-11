#include "TextHolder.h"

void TextHolder::load(TextFile::ID id, const std::string& filename){
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string* contents=new std::string();
		
		//go to the first 0 bytes counting from the end of file
		in.seekg(0, std::ios::end);
		
		//reserve memory equivalent to the position we currently are on in the file, essentially the size of the file
		contents->reserve(in.tellg());
		
		//go to the beginning of the file
		in.seekg(0, std::ios::beg);
		
		//assign the file's contents to the allocated string, avoiding reallocations since they were made upfront
		contents->assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		
		in.close();
		mResourceMap.insert(
			std::make_pair(
				id,
				std::unique_ptr<std::string>(contents)
			)
		);

	}else{
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

}


std::string& TextHolder::get(TextFile::ID id){
	return (*mResourceMap.at(id));
}

const std::string& TextHolder::get(TextFile::ID id) const{
	return (*mResourceMap.at(id));
}
