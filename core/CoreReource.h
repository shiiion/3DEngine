#pragma once

#ifdef COMP_DLL_CORE
#define DECLSPEC_CORE __declspec(dllexport)
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
	typedef void(__cdecl* OnInputFunc)(IAbstractInputSystem* inputSystem, int outputCode);

	struct Material
	{
		float collisionTime;
		float friction;
		//fraction of velocity gained when bouncing
		//0 = no bounce, 1 = 100% bounce
		float reboundFraction;
	};

	enum EntityType
	{
		entity = 1, renderable = 2, physicsObject = 3
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

	struct CollisionInfo
	{
		CollisionInfo(ICollisionMesh* t, ICollisionMesh* o) : thisMesh(t), otherMesh(o) {}

		ICollisionMesh* otherMesh;
		ICollisionMesh* thisMesh;

		float collisionTime;

		glm::vec3 lastSeparatingAxis;

		glm::vec3 intersectionPoint;

		glm::vec3 collisionNormal;

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

	struct Control
	{
		Control(int in, int out)
		{
			inputCode = in;
			outputCode = out;
		}

		int inputCode;
		int outputCode;

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