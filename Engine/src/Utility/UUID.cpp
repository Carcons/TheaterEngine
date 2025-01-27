#include <random>
#include <cstdint>

#include <Theater/Engine/Utility/UUID.hpp>

namespace Theater
{
	namespace UUID
	{
		uint64_t GenerateUUID()
		{
			static std::random_device RandomDevice;
			static std::mt19937_64 RandomEngine(RandomDevice());
			static std::uniform_int_distribution<uint64_t> UDistribution;

			return UDistribution(RandomEngine);
		}
	}
}