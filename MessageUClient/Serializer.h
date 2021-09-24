#pragma once

#include <string>
#include <type_traits>
#include "StringUtils.h"

class Serializer
{
public:
	Serializer() = default;
	virtual ~Serializer() = default;

	std::string serialize() const { return _data; }
	uint32_t get_serialized_data_size() const { return _data.size(); }

	template <typename T, typename = std::enable_if_t<std::is_pod_v<T>>>
	void add(T item)
	{
		_data += StringUtils::to_string(item);
	}

	template <typename... TArgs>
	void add(TArgs... items)
	{
		(add(items), ...);
	}

	void add(const std::string& str);

private:
	std::string _data;
};
