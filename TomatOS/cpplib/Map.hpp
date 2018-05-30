#ifndef CPPLIB_MAP_HPP
#define CPPLIB_MAP_HPP

#include <Vector.hpp>
#include <Pair.hpp>

namespace cpplib {

	template<class Key, class Data>
	class MapCursor;

	template<class Key_, class Data_>
	class Map {
	public:
		typedef Data_ Data;
		typedef Key_ Key;

	protected:
		Vector<Data> info;

	public:


	};

}

#endif