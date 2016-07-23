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

	typedef bool(__cdecl* RaytraceFunc)(IPhysicsObject* hitObject);
	typedef void(__cdecl* CustomMoveTick)(IPhysicsObject& object, float deltaTime);
	typedef void(__cdecl* CustomMoveResolve)(ICollisionMesh& mesh);
	typedef void(__cdecl* OnInputFunc)(IAbstractInputSystem* inputSystem, int outputCode, bool set);

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

	struct CustomMovement
	{
		int movementValue;
		CustomMoveTick tickFunc;
		CustomMoveResolve resolveFunc;
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

	struct Control
	{
		Control(int inputType, int in, int out)
		{
			this->inputType = inputType;

			inputCode = in;
			outputCode = out;
		}
		int inputType;

		int inputCode;
		int outputCode;

		//microsoft should burn in hell

	};

	struct InputState
	{
		InputState(int out, OnInputFunc func)
		{
			outputCode = out;
			isSet = false;
			prevSet = false;
			inputFunc = func;
		}

		int outputCode;
		bool isSet;
		bool prevSet;
		OnInputFunc inputFunc;
	};

	typedef unsigned __int8 UBYTE;
	typedef signed __int32 INT32;
	typedef unsigned __int32 UINT32;

};