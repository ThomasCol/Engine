#ifndef _LEVEL_H_
#define	_LEVEL_H_

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "Mat4.h"

using namespace Core::Maths;

class		Level
{
	private:
		unsigned int			m_len;
	public:
		enum class		Case
		{
			WALL = 0,
			END = 1,
			NOTHING = 2,
			WALL2 = 3,
			END2 = 4,
			ENEMY = 5,
			HEAL = 6,
			AMMO = 7,
			WALLTAP1 = 8,
			WALLTAP2 = 9,
		};
		Case**					m_map;
		float					m_x;
		float					m_y;
		std::string				m_file = "resources/map/map3";

		Level();
		Level(const Level& level);
		Level(Level&& level) = default;
		~Level();
		void			SetFile(std::string file) {m_file = file;};
		std::string		GetFile() const {return m_file;};
		unsigned int	GetLen() const {return m_len;};
		Level&			SetMap();
		Level&			operator=(const Level& level);
};

#endif // !_LEVEL_H_
