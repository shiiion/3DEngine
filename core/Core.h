#pragma once

#include "CoreReource.h"

#ifdef COMP_DLL_CORE
#include "World.h"
#include "MovementStateCallbackManager.h"
#endif

struct GLFWwindow;

namespace ginkgo
{
	class IWorld;
	class ICharacter;
	class IAbstractInputSystem;

#ifdef COMP_DLL_CORE
	class Core
	{
	private:
		static long entityIDBase;

		unsigned long long startTick;

		bool running;
		float tickTime;
		World* world;

		float lastTickTime;

		MovementStateCallbackManager manager;

		vector<IAbstractInputSystem*> inputSystemList;

	public:
		static Core core;
		Core();

		vector<IAbstractInputSystem*> const& getInputSystemList()
		{
			return inputSystemList;
		}

		void coreTick(float timeScale);
		void physicsTick(float elapsedTime);

		void processInput();
		void sleep();

		void setTickTime(float time);
		float getTickTime() const;

		float getEngineTime() const;

		IWorld* getWorld() const;

		static long generateID();
		static void startCore();
		static void stopCore();
		static void setupInput(GLFWwindow* window);
		static void registerInputSystem(IAbstractInputSystem* input, ICharacter* controller);

	};
#endif
	DECLSPEC_CORE float getEngineTime();
	DECLSPEC_CORE void setTickTime(float time);
	DECLSPEC_CORE float getTickTime();
	DECLSPEC_CORE void startCore();
	DECLSPEC_CORE void stopCore();
	DECLSPEC_CORE IWorld* getWorld();

	DECLSPEC_CORE void tickCore(float timeScale);

	DECLSPEC_CORE void sleepTickTime();

	DECLSPEC_CORE void registerInputSystem(IAbstractInputSystem* input, ICharacter* controller);
	DECLSPEC_CORE vector<IAbstractInputSystem*> const& getAllInputSystems();

	DECLSPEC_CORE int registerMovementState(const std::string& name, const CheckIfMovementState& CheckMovementState, const DoOnMovementState& OnMovementState, const OnMovementStateEnabled& OnStateEnabled, const OnMovementStateDisabled& OnStateDisabled);
	DECLSPEC_CORE int getMovementState(const std::string& name);

	DECLSPEC_CORE void setupInput(GLFWwindow* window);
}