#ifndef _TRIPLET_H_
#define _TRIPLET_H_

#include <string>

namespace Core::DataStructure
{
	typedef unsigned int uint;
	/* class triplet for index */
	struct Triplet
	{
		/* class instance */
		uint	posIndice;
		uint	uvIndice;
		uint	normIndice;

		/* constructor & destructor */
		Triplet() = delete;
		Triplet(const Triplet& trip) = default;
		Triplet(Triplet&& trip) = delete;
		Triplet(uint pos, uint uv, uint norm);
		~Triplet() = default;

		/* Debug func */
		std::string		ToString();
	};

	/* operator for mapping triplet */
	bool				operator<(const Triplet& trip1, const Triplet& trip2);
}

#endif //_TRIPLET_H_
