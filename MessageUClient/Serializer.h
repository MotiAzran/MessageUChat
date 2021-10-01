#pragma once

#include <string>
#include <array>
#include <type_traits>
#include "StringUtils.h"

class Serializer
{
public:
	Serializer() = default;
	virtual ~Serializer() = default;

	std::string serialize() const { return _data; }

	template <typename Val>
	void add(Val item)
	{
		_data += StringUtils::to_string(item);
	}

	template <typename... TArgs>
	void add(TArgs... items)
	{
		// Add items to serializer by the parameter order
		(add(items), ...);
	}

	void add(const std::string& str);

private:
	std::string _data;
};
