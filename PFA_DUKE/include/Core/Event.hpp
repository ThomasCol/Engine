#ifndef __EVENT_H__
#define __EVENT_H__

#include <functional>
#include <list>

#define ARG0
#define ARG1 _1
#define ARG2 _1, _2
#define ARG3 _1, _2, _3
#define ARG4 _1, _2, _3, _4
#define ARG5 _1, _2, _3, _4, _5
#define ARG6 _1, _2, _3, _4, _5, _6

using namespace std::placeholders;

/* Usage:
** To subscribe with a simple function
**		event += function;
**
** To subscribe with a class method
**		event += std::bind(&className::methodName, &object, ARGx);
**															 ^=> none if no arg
**
*/

namespace Core
{
	template<typename... Args>
	class Event
	{
	private:
		std::list<std::function<void(Args...)>>	m_list;

	public:
		Event() = default;
		Event(const Event<Args...>& event): m_list(event.m_list) {}
		Event(Event<Args...>&& event): m_list(std::move(event.m_list)) {}
		~Event() = default;

		inline void operator()(Args... args)
		{
			for (auto it {m_list.begin()}; it != m_list.end();)
			{
				if (*it != nullptr)
					(*it++)(args...);
			}
		}

		inline Event& operator+=(std::function<void(Args...)> func)
		{
			m_list.push_back(func);
			return *this;
		}

		inline Event& operator-=(std::function<void(Args...)> func)
		{
			for (auto it = m_list.begin(); it != m_list.end(); ++it)
				if (func.target_type() == (*it).target_type())
				{
					m_list.erase(it);
					return *this;
				}

			return *this;
		}
	};
}


#endif
