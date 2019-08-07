#include "Triplet.h"

namespace Core::DataStructure
{
	Triplet::Triplet(uint pos, uint uv, uint norm):
		posIndice {pos},
		uvIndice {uv},
		normIndice {norm}
	{}

	std::string	Triplet::ToString()
	{
		std::string str = std::to_string(posIndice + 1) + "/" +
				std::to_string(uvIndice + 1) + "/" +std::to_string(normIndice + 1);

		return str;
	}

	bool	operator<(const Triplet& trip1, const Triplet& trip2)
	{
		/* sort triplet */
		if (trip1.posIndice < trip2.posIndice)
			return true;

		else if (trip1.posIndice == trip2.posIndice)
		{
			if (trip1.uvIndice < trip2.uvIndice)
				return true;

			else if (trip1.uvIndice == trip2.uvIndice)
			{
				if (trip1.normIndice < trip2.normIndice)
					return true;
			}
		}

		return false;
	}
}
