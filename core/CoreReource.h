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
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#define SETFLAG(r, f) ((r) |= (f))
#define RESFLAG(r, f) ((r) &= (~(f)))
#define TOGFLAG(r, f) ((r) ^= (f))
#define PI 3.14159265358979324f
#define DEG_TO_RAD(d) ((d) * (PI / 180.0f))
#define RAD_TO_DEG(r) ((r) * (180.0f / PI))
#define MIN_THRESHOLD 0.000000001f

namespace ginkgo
{
	using std::vector;

	class IPhysicsObject;
	class ICollisionMesh;
	class IAbstractInputSystem;
	class ICharacter;

	typedef bool(__cdecl* RaytraceFunc)(IPhysicsObject* hitObject);
	typedef std::function<bool(const ICharacter&, float)> CheckIfMovementState;
	typedef std::function<void(ICharacter&, float)> DoOnMovementState;
	typedef std::function<void(ICharacter&, float)> OnMovementStateEnabled;
	typedef std::function<void(ICharacter&, float)> OnMovementStateDisabled;

	typedef void(__cdecl* OnInputFunc)(IAbstractInputSystem* inputSystem, int outputCode, bool set);

	typedef unsigned __int8 UBYTE;
	typedef signed __int32 INT32;
	typedef unsigned __int32 UINT32;


	struct Material
	{
		float friction;
		//fraction of rebound velocity (restitution)
		//minimum restitution is chosen in calculations
		//0 = no bounce, 1 = 100% bounce
		float reboundFraction;
	};

	enum EntityType
	{
		entity = 1, physicsObject = 3, character = 4
	};

	struct Triangle
	{
		glm::vec3 P1;
		glm::vec3 P2;
		glm::vec3 P3;
	};

	struct Ray
	{
		glm::vec3 point;
		glm::vec3 direction;
	};

	struct Segment
	{
		glm::vec3 start;
		glm::vec3 end;
	};

	struct MoveInfo
	{
		glm::vec3 centerStart;
		glm::vec3 centerEnd;

		glm::vec3 velStart;
		glm::vec3 velEnd;

		glm::vec3 accel;

		float deltaTime;
	};

	class ICollisionMesh;

	//Collision manifold between two meshes
	struct CollisionInfo
	{
		CollisionInfo(ICollisionMesh* t, ICollisionMesh* o) : thisMesh(t), otherMesh(o) {}

		ICollisionMesh* otherMesh;
		ICollisionMesh* thisMesh;

		float collisionTime;

		int lastSeparatingAxisType;
		int intersectSide;
		glm::vec3 lastSeparatingAxis;

		glm::vec3 intersectionPoint;

		glm::vec3 collisionNormal;

	};

	struct SurfaceData
	{
		SurfaceData(UINT32 thisID, UINT32 otherID, glm::vec3 const& normal)
		{
			this->thisID = thisID;
			this->otherID = otherID;
			surfaceNormal = normal;
		}

		UINT32 thisID;
		UINT32 otherID;
		glm::vec3 surfaceNormal;

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

	struct CollisionStationary
	{
		CollisionStationary(ICollisionMesh* t, ICollisionMesh* o) : thisMesh(t), otherMesh(o) {}

		ICollisionMesh* otherMesh;
		ICollisionMesh* thisMesh;

		glm::vec3 normal;

		float overlapDist;
	};

	struct MoveResult
	{
		MoveResult(glm::vec3 finalPos = glm::vec3(), glm::vec3 finalVel = glm::vec3())
			: finalPos(finalPos), finalVel(finalVel)
		{}
		glm::vec3 finalPos;
		glm::vec3 finalVel;
	};

	struct RaytraceResult
	{
		bool didHit;

		IPhysicsObject* firstCollision;
		float collisionDist;

		Ray ray;
		float rayDist;
	};

	struct RaytraceParams
	{
		vector<IPhysicsObject*> ignoreList;
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

	struct Command
	{
		Command(int out, OnInputFunc func)
		{
			outputCode = out;
			onCommand = func;
		}

		int outputCode;
		OnInputFunc onCommand;
	};

	struct CommandState
	{
		CommandState(Command const& command) : command(command)
		{
			isSet = false;
			prevSet = false;
		}

		bool isSet;
		bool prevSet;
		Command command;
	};
};