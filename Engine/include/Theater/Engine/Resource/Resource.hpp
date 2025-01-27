#pragma once

#include <string>
#include <memory>

namespace Theater
{
	class Resource
	{
	public:

		std::string m_ID;

	public:

		virtual ~Resource() = default;

	public:

		virtual void OnLoad() = 0;

	};
}