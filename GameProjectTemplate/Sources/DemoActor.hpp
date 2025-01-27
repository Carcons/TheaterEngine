#include <Theater/GameplayAPI/GameplayAPI.hpp>

class GAMEPLAY_API DemoActor : public Theater::Actor
{
public:

	DemoActor() = default;
	~DemoActor() {};
	
public:

	void OnUpdate(float dt) override;

private:

};