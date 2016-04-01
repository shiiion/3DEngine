
#include "Surface.h"

namespace ginkgo
{
	Surface::Surface(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
	{
		makeTwoTriangles(v1, v2, v3, v4);
	}

	void Surface::makeTwoTriangles(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
	{
		//TODO: check if points are in clockwise order
		glm::vec3 diagonalPoint, nonDiag1, nonDiag2;

		float l1 = (v2 - v1).length();
		float l2 = (v3 - v1).length();
		float l3 = (v4 - v1).length();

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
	}
	
	void Surface::getNormal(const Triangle& t, glm::vec3& normOut) const
	{
		normOut = glm::normalize(glm::cross((t1.P2 - t1.P1), (t1.P3 - t1.P2)));
	}

	bool Surface::intersectsWith(const Triangle& t, const Ray& ray, float distance) const
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

	bool Surface::doesIntersect(const Ray& ray, float distance) const
	{
		Ray normRay = ray;
		normRay.direction = glm::normalize(normRay.direction);
		return (intersectsWith(t1, normRay, distance) || (intersectsWith(t2, normRay, distance)));
	}

	float Surface::getIntersectionValue(const Ray& ray) const
	{
		Ray normRay = ray;
		normRay.direction = glm::normalize(normRay.direction);
		glm::vec3 normal, intersect;
		float d, intersectionDistance;
		getNormal(t1, normal);

		d = -glm::dot(t1.P1, normal);

		return -(d + glm::dot(normal, normRay.point)) / glm::dot(normal, normRay.direction);
	}

}