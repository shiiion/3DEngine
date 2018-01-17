#pragma once
#ifdef COMP_DLL_CORE

#ifdef _WIN64
//THIS IS USED FOR JNA
#define DECLSPEC_CORE extern "C" __declspec(dllexport)
#else
#define DECLSPEC_CORE __declspec(dllexport)
#endif

#else
#define DECLSPEC_CORE __declspec(dllimport)
#endif
#include <vector>
#include <functional>
#include <condition_variable>
#include <memory>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#define SETFLAG(r, f) ((r) |= (f))
#define RESFLAG(r, f) ((r) &= (~(f)))
#define TOGFLAG(r, f) ((r) ^= (f))
#define PI 3.14159265358979324f
#define DEG_TO_RAD(d) ((d) * (PI / 180.0f))
#define RAD_TO_DEG(r) ((r) * (180.0f / PI))
#define MIN_THRESHOLD 0.000000001f

// make this value modifiable
#define BAUMGARTE 0.2f

namespace ginkgo
{
	using std::vector;
	using std::unique_ptr;
	using glm::vec3;
	using glm::vec4;
	using glm::quat;

	class IPhysicsComponent;
	class ICollisionMesh;
	class IAbstractInputSystem;
	class ICharacter;

	typedef bool(__cdecl* RaytraceFunc)(IPhysicsComponent* hitObject);
	typedef std::function<bool(const ICharacter&, float)> CheckIfMovementState;
	typedef std::function<void(ICharacter&, float)> DoOnMovementState;
	typedef std::function<void(ICharacter&, float)> OnMovementStateEnabled;
	typedef std::function<void(ICharacter&, float)> OnMovementStateDisabled;

	//requires no extra parameters (used for things like button states)
	typedef void(__cdecl* OnInputFunc)(IAbstractInputSystem* inputSystem, int outputCode, bool set);

	//takes in two parameters
	typedef void(__cdecl* OnInput2fFunc)(IAbstractInputSystem* inputSystem, int outputCode, float a, float b);

	typedef unsigned __int8 UBYTE;
	typedef signed __int32 INT32;
	typedef unsigned __int32 UINT32;


	struct PhysMaterial
	{
		PhysMaterial()
			: friction(0), elasticity(0)
		{}

		float friction;
		float elasticity;
	};

	enum EntityType
	{
		entity = 1, physicsComponent = 3, character = 4
	};

	struct Triangle
	{
		vec3 P1;
		vec3 P2;
		vec3 P3;
	};

	struct Ray
	{
		vec3 point;
		vec3 direction;
	};

	struct Segment
	{
		vec3 start;
		vec3 end;
	};

	struct MoveInfo
	{
		vec3 centerStart;
		vec3 centerEnd;

		vec3 velStart;
		vec3 velEnd;

		vec3 angleStart;
		vec3 angleEnd;

		vec3 angleVelStart;
		vec3 angleVelEnd;

		float deltaTime;
	};

	class IPhysicsComponent;

	// Collision manifold
	struct ManifoldData
	{
		ManifoldData(IPhysicsComponent* t, IPhysicsComponent* o)
			: thisMesh(t), otherMesh(o) {}

		IPhysicsComponent* otherMesh;
		IPhysicsComponent* thisMesh;
		
		vec3 overlapNormal;
		float overlapDist;

		vec3 contactA, contactB;

		// engine time when this Manifold was first generated
		float collisionTime;

		// bool resting;
	};

	struct SurfaceData
	{
		SurfaceData(UINT32 thisID, UINT32 otherID, vec3 const& normal)
		{
			this->thisID = thisID;
			this->otherID = otherID;
			surfaceNormal = normal;
		}

		UINT32 thisID;
		UINT32 otherID;
		vec3 surfaceNormal;

		bool compareSurface(SurfaceData const& otherSurface)
		{
			if ((thisID == otherSurface.thisID) && (otherID == otherSurface.otherID))
			{
				return (surfaceNormal.x == otherSurface.surfaceNormal.x) &&
					(surfaceNormal.y == otherSurface.surfaceNormal.y) &&
					(surfaceNormal.z == otherSurface.surfaceNormal.z);
			}
			return false;
		}
	};

	struct MoveResult
	{
		MoveResult(vec3 finalPos = vec3(), vec3 finalVel = vec3(), vec3 finalAng = vec3(), vec3 finalAngVel = vec3())
			: finalPos(finalPos), finalVel(finalVel), finalAng(finalAng), finalAngVel(finalAngVel)
		{}
		vec3 finalPos;
		vec3 finalVel;
		vec3 finalAng;
		vec3 finalAngVel;
	};

	struct RaytraceResult
	{
		bool didHit;

		IPhysicsComponent* firstCollision;
		float collisionDist;

		Ray ray;
		float rayDist;
	};

	struct RaytraceParams
	{
		vector<IPhysicsComponent*> ignoreList;
		RaytraceFunc func;
	};

	struct RegisteredMovementState {
		RegisteredMovementState(const std::string& name, const CheckIfMovementState& CheckMovementState, const DoOnMovementState& OnMovementState, 
			const OnMovementStateEnabled& onStateEnabled, const OnMovementStateDisabled& onStateDisabled) :
			name(name),
			CheckMovementState(CheckMovementState),
			OnMovementState(OnMovementState),
			OnStateEnabled(onStateEnabled),
			OnStateDisabled(onStateDisabled)
		{}

		std::string name;
		CheckIfMovementState CheckMovementState;
		DoOnMovementState OnMovementState;
		OnMovementStateEnabled OnStateEnabled;
		OnMovementStateDisabled OnStateDisabled;

		static RegisteredMovementState GetNullMovementState()
		{
			return RegisteredMovementState("NullMovementState", [](const ICharacter&, float) {return false; }, [](const ICharacter&, float) {return; }, [](const ICharacter&, float) { return; }, [](const ICharacter&, float) { return; });
		}
	};

	struct Prism
	{
		Prism(float x = 0, float y = 0, float z = 0, float w = 0, float h = 0, float l = 0)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->l = l;
			this->w = w;
			this->h = h;
		}

		float x, y, z;
		float l, w, h;
	};

#pragma region Input

#define INPUTTYPE_INVALID	-1
#define INPUTTYPE_USER		1
#define INPUTTYPE_OTHER		2

	struct Bind
	{
		Bind(int inputType, int in, int out)
		{
			this->inputType = inputType;

			inputCode = in;
			outputCode = out;
		}
		int inputType;

		int inputCode;
		int outputCode;

	};

	//if needed, expand this
	enum CommandParams
	{
		NO_PARAMS,
		FLOAT_2,
	};

#define OUTCODE_INVALID -1
#define INCODE_MOUSE -1

	struct Command
	{
	protected:
		Command(int out, CommandParams type)
			: outputCode(out), type(type)
		{}
	public:

		CommandParams type;
		int outputCode;

		virtual Command* clone() const = 0;
	};

	struct Command2f : Command
	{
		Command2f(int out,  OnInput2fFunc func)
			: Command(out, FLOAT_2), onInput(func), a(0), b(0)
		{}

		Command* clone() const override
		{
			return new Command2f(outputCode, onInput);
		}

		OnInput2fFunc onInput;
		float a, b;
	};

	struct CommandSetReset : Command
	{
		CommandSetReset(int out, OnInputFunc func)
			: Command(out, NO_PARAMS), onInput(func)
		{}

		Command* clone() const override
		{
			return new CommandSetReset(outputCode, onInput);
		}

		OnInputFunc onInput;
	};

	struct CommandState
	{
		CommandState(Command const& command)
		{
			this->command = command.clone();

			isSet = false;
			prevSet = false;
		}

		~CommandState()
		{
			delete command;
		}

		bool isSet;
		bool prevSet;
		Command* command;
	};

#pragma endregion

};