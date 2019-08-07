#include "Level.h"

using namespace	std;

Level::Level()
{
	m_len = 40;
	m_x = 0.5;
	m_y = -0.5;
	m_map = new Case*[m_len];
	for (unsigned int i = 0; i < m_len; i++)
	{
		m_map[i] = new Case[m_len];
	}
}

Level::Level(const Level& level)
{
	m_len = level.m_len;
	m_file = level.m_file;
	for (unsigned int i = 0; i < m_len; i++)
	{
		for (unsigned int j = 0; j < m_len; j++)
			m_map[i][j] = level.m_map[i][j];
	}
}

Level::~Level()
{
	for (unsigned int i = 0; i < m_len; i++)
		delete m_map[i];
	delete m_map;
}

Level&		Level::SetMap()
{
	ifstream file {m_file};

	if(file.is_open())
	{
		const char*		str;
		std::string		line;
		unsigned int	cpt = 0;
		unsigned int	row = 0;

		while (!file.eof())
		{
			getline(file, line);
			str = line.c_str();
			if (row < m_len)
			{
				for (unsigned int j = 0; j < m_len; j++)
				{
					if (str[cpt] == '0')
						m_map[row][j] = Case::WALL;
					else if (str[cpt] == '1')
						m_map[row][j] = Case::END;
					else if (str[cpt] == '2')
						m_map[row][j] = Case::NOTHING;
					else if (str[cpt] == '3')
						m_map[row][j] = Case::WALL2;
					else if (str[cpt] == '4')
						m_map[row][j] = Case::END2;
					else if (str[cpt] == '5')
						m_map[row][j] = Case::ENEMY;
					else if (str[cpt] == '6')
						m_map[row][j] = Case::HEAL;
					else if (str[cpt] == '7')
						m_map[row][j] = Case::AMMO;
					else if (str[cpt] == '8')
						m_map[row][j] = Case::WALLTAP1;
					else
						m_map[row][j] = Case::WALLTAP2;
					cpt++;
				}
			}
			row++;
			cpt = 0;
		}
		file.close();
	}
	return *this;
}

Level&		Level::operator=(const Level& level)
{
	for (unsigned int i = 0; i < level.m_len; i++)
		for (unsigned int j = 0; j < level.m_len; j++)
			m_map[i][j] = level.m_map[i][j];
	return *this;
}
