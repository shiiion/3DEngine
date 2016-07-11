
#include "Surface.h"
#include <glm/gtx/projection.hpp>

namespace ginkgo
{
	Surface::Surface(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
	{
		makeTwoTriangles(v1, v2, v3, v4);
	}

	Surface::Surface()
	{

	}

	void Surface::makeTwoTriangles(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
	{
		
		//TODO: check if points are in clockwise order
		//possible solution: if normal direction (based on P2-P1 X P3-P2) pl projected onto P2 is , swap P2 and P3
		glm::vec3 diagonalPoint, nonDiag1, nonDiag2;

		float l1 = glm::length(v2 - v1);
		float l2 = glm::length(v3 - v1);
		float l3 = glm::length(v4 - v1);

		if (l1 > l2)
		{
			if (l1 > l3)
			{
				diagonalPoint = v2;
				nonDiag1 = v3;
				nonDiag2 = v4;
			}
			else
			{
				diagonalPoint = v4;
				nonDiag1 = v2;
				nonDiag2 = v3;
			}
		}
		else //l2 >= l1
		{
			if (l2 > l3)
			{
				diagonalPoint = v3;
				nonDiag1 = v2;
				nonDiag2 = v4;
			}
			else
			{
				diagonalPoint = v4;
				nonDiag1 = v2;
				nonDiag2 = v3;
			}
		}
		t1.P1 = v1; t1.P2 = nonDiag1; t1.P3 = diagonalPoint;
		t2.P1 = v1; t2.P2 = nonDiag2; t2.P3 = diagonalPoint;
		glm::vec3 v, proj;
		getNormal(t1, v);
		proj = glm::proj(v, t1.P2);
		if (glm::length(proj + t1.P2) < glm::length(t1.P2))
		{
			v = t1.P2;
			t1.P2 = t1.P3;
		}

		getNormal(t2, v);
		proj = glm::proj(v, t2.P2);
		if (glm::length(proj + t2.P2) < glm::length(t2.P2))
		{
			v = t2.P2;
			t2.P2 = t2.P3;
		}
	}
	
	void Surface::getNormal(const Triangle& t, glm::vec3& normOut) const
	{
		normOut = glm::normalize(glm::cross((t.P2 - t.P1), (t.P3 - t.P2)));
	}

	bool Surface::testIntersection(const Triangle& t, const Ray& ray, float distance) const
	{
		UINT32 posI;
		UINT32 posF;
		glm::vec3 normal, intersectPoint, ip1, ip2, ip3;
		float d, p, intersectionDistance, tsum;

		getNormal(t, normal);

		d = -glm::dot(t.P1, normal);

		p = glm::dot(normal, ray.point) + d;

		if (p > 0.f)
			posI = 1;//front
		else if (p < 0.f)
			posI = 2;//behind
		else
			posI = 3;//on top

		p = glm::dot(normal, ray.point + (ray.direction * distance) + d);

		if (p > 0.f)
			posF = 1;//front
		else if (p < 0.f)
			posF = 2;//behind
		else
			posF = 3;//on top

		if (posI == posF)
			return false;
		intersectionDistance = -(d + glm::dot(normal, ray.point)) / glm::dot(normal, ray.direction);
		intersectPoint = ray.point + (ray.direction * intersectionDistance);

		ip1 = glm::normalize(intersectPoint - t.P1);
		ip2 = glm::normalize(intersectPoint - t.P2);
		ip3 = glm::normalize(intersectPoint - t.P3);

		tsum = (glm::acos(glm::dot(ip1, ip2)) + glm::acos(glm::dot(ip2, ip3)) + glm::acos(glm::dot(ip3, ip1)));

		if (fabs(tsum - (2 * PI)) < 0.1f)
			return true;
		return false;
	}

	bool Surface::intersectsWithSurface(const Ray& ray, float distance) const
	{
		Ray normRay = ray;
		normRay.direction = glm::normalize(normRay.direction);
		return (testIntersection(t1, normRay, distance) || (testIntersection(t2, normRay, distance))); //TODO: possible optimization, test intersection with square rather than 2 Tris
	}

	float Surface::getIntersectionValue(const Ray& ray) const
	{
		Ray normRay = ray;
		normRay.direction = glm::normalize(normRay.direction);
		glm::vec3 normal, intersect;
		float d;
		getNormal(t1, normal);

		d = -glm::dot(t1.P1, normal);

		return -(d + glm::dot(normal, normRay.point)) / glm::dot(normal, normRay.direction);
	}

	void Surface::translateSurface(const glm::vec3& translation)
	{
		t1.P1 += translation;
		t1.P2 += translation;
		t1.P3 += translation;

		t2.P1 += translation;
		t2.P2 += translation;
		t2.P3 += translation;
	}

	ISurface* createSurface(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
	{
		return new Surface(v1, v2, v3, v4);
	}
}