
#include "SurfaceCollisionMesh.h"

namespace ginkgo
{
	void SurfaceCollisionMesh::createSurfacesFromMesh()
	{
		vec3 p0, p1, p2, p3, p4, p5, p6, p7;
		vec3 const& xAxis = mesh->getAxis(0);
		vec3 const& yAxis = mesh->getAxis(1);
		vec3 const& zAxis = mesh->getAxis(2);
		float xExt = mesh->getExtent(0);
		float yExt = mesh->getExtent(1);
		float zExt = mesh->getExtent(2);

		p0 = (xAxis * xExt) + (yAxis * yExt) + (zAxis * zExt);
		p1 = (xAxis * xExt) + (yAxis * -yExt) + (zAxis * zExt);
		p2 = (xAxis * xExt) + (yAxis * -yExt) + (zAxis * -zExt);
		p3 = (xAxis * xExt) + (yAxis * yExt) + (zAxis * -zExt);
		p4 = (xAxis * -xExt) + (yAxis * yExt) + (zAxis * zExt);
		p5 = (xAxis * -xExt) + (yAxis * -yExt) + (zAxis * zExt);
		p6 = (xAxis * -xExt) + (yAxis * -yExt) + (zAxis * -zExt);
		p7 = (xAxis * -xExt) + (yAxis * yExt) + (zAxis * -zExt);

		faces[0].makeTwoTriangles(p0, p1, p2, p3);
		faces[1].makeTwoTriangles(p0, p4, p5, p1);
		faces[2].makeTwoTriangles(p4, p7, p6, p5);
		faces[3].makeTwoTriangles(p3, p7, p6, p2);
		faces[4].makeTwoTriangles(p1, p5, p6, p2);
		faces[5].makeTwoTriangles(p0, p4, p7, p3);
	}
}