#pragma once
#include <vector>
#include <string>

// Controllers are really Object Managing Factories MF
// Might need a better names here, something like:

// - MF_DrawFilters
// - MF_DrawPixels
// - MF_Generator
// - MF_SoundDevices
// - MF_VideoDevices
// - MF_Images

class ofApp;

class Managing_Factory {
public:
	ofApp* pixelplotter{ nullptr };
	
	Managing_Factory();

	std::vector<std::string> v_factory_types;
	std::vector<std::string> v_human_names;
	
	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool _bFresh = false;
};