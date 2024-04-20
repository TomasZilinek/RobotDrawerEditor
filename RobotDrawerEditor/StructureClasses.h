#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "FunctionHolder.h"
#include <vector>
#include <memory>

#ifndef STRUCTURECLASSES_H_
#define STRUCTURECLASSES_H_

using namespace std;


							///////////////////////
							// TimeManager Class //
							///////////////////////


class TimeManager {
public:
	TimeManager();
	void setTimeChange(sf::Time new_time_lapsed);

	template<typename F>				void setTimeout(F function, unsigned int milliseconds);
	template<typename F, typename A>	void setTimeout(F function, A argument, unsigned int milliseconds);
	template<typename C>				void setTimeout(void(C::*function)(), C* object, unsigned int milliseconds);
	template<typename C, typename A>	void setTimeout(void(C::*function)(A), C* object, A argument, unsigned int milliseconds);
private:
	sf::Time				time_lapsed;
	vector<BaseFunction*>	all_holders;
	vector<sf::Time>		times_remaining;
};


template<typename F> void TimeManager::setTimeout(F function, unsigned int milliseconds) {
	all_holders.push_back(new Functor<F>(function));
	times_remaining.push_back(time_lapsed + sf::milliseconds(milliseconds));
}

template<typename F, typename A> void TimeManager::setTimeout(F function, A argument, unsigned int milliseconds) {
	all_holders.push_back(new FunctionWithArg<F, A>(function));
	times_remaining.push_back(time_lapsed + sf::milliseconds(milliseconds));
}

template<typename C> void TimeManager::setTimeout(void(C::*function)(), C* object, unsigned int milliseconds) {
	all_holders.push_back(new ClassFunction<C>(function, object));
	times_remaining.push_back(time_lapsed + sf::milliseconds(milliseconds));
}

template<typename C, typename A> void TimeManager::setTimeout(void(C::*function)(A), C* object, A argument, unsigned int milliseconds) {
	all_holders.push_back(new ClassFunctionWithArg<C, A>(function, object, argument));
	times_remaining.push_back(time_lapsed + sf::milliseconds(milliseconds));
}


							//////////////////////////
							// ResourceHolder Class //
							//////////////////////////

namespace Textures {
	enum ID { menuBg, selectTool, moveTool, editPathsTool, rectanglesTool, ellipsesTool, polygonsTool, spiralsTool,
			  freehandTool, curvesTool, textTool, cross_close
	};
}

namespace Fonts {
	enum ID { Arial, };
}

namespace Sounds {
	enum ID { buttonHoverSound, buttonClickSound };
}

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
	ResourceHolder() {}
	~ResourceHolder() {}

	void load(Identifier ID, const string& filepath);
	Resource& get(Identifier ID);
private:
	map<Identifier, unique_ptr<Resource>> resource_map;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier ID, const string& filepath) {
	unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(filepath))
		throw std::runtime_error("ResourceHolder::load - Failed to load from " + filepath);

	resource_map.insert(make_pair(ID, move(resource)));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier ID) {
	auto found = resource_map.find(ID);

	if (found != resource_map.end())
		return *(found->second);
	else
		throw std::runtime_error("ResourceHolder::get - resource does not exist");
}


						////////////////////////////
						// ResourceManager Struct //
						////////////////////////////


struct ResourceManager {
public:
	ResourceHolder<sf::Texture, Textures::ID>		textureHolder;
	ResourceHolder<sf::SoundBuffer, Sounds::ID>		soundHolder;
	ResourceHolder<sf::Font, Fonts::ID>				fontHolder;
};

#endif