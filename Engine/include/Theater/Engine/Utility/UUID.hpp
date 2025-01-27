#pragma once

#include <cstdint>


#include "../EngineAPI.hpp"

namespace Theater
{
	namespace UUID
	{
		ENGINE_API uint64_t GenerateUUID();
	}
}

using uuid = uint64_t;