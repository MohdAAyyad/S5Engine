#include "FrustumCullingHandle.h"
#include "../Core/CoreEngine.h"

std::unique_ptr<FrustumCullingHandle>FrustumCullingHandle:: instance = nullptr;

FrustumCullingHandle::FrustumCullingHandle()
{

}

void FrustumCullingHandle::CalculateFrustumPlanes(glm::mat4 modelMatrix_)
{
	//Get the matrix
	Camera* cam = CoreEngine::GetInstance()->GetCamera();
	glm::mat4 mvpMat = cam->GetPerspective() * cam->GetView() * modelMatrix_;

	//Left
	vfPlanes[0].a = mvpMat[0][3] + mvpMat[0][0];
	vfPlanes[0].b = mvpMat[1][3] + mvpMat[1][0];
	vfPlanes[0].c = mvpMat[2][3] + mvpMat[2][0];
	vfPlanes[0].d = mvpMat[3][3] + mvpMat[3][0];

	//Right
	vfPlanes[1].a = mvpMat[0][3] - mvpMat[0][0];
	vfPlanes[1].b = mvpMat[1][3] - mvpMat[1][0];
	vfPlanes[1].c = mvpMat[2][3] - mvpMat[2][0];
	vfPlanes[1].d = mvpMat[3][3] - mvpMat[3][0];

	//Bottom
	vfPlanes[2].a = mvpMat[0][3] + mvpMat[0][1];
	vfPlanes[2].b = mvpMat[1][3] + mvpMat[1][1];
	vfPlanes[2].c = mvpMat[2][3] + mvpMat[2][1];
	vfPlanes[2].d = mvpMat[3][3] + mvpMat[3][1];

	//Top
	vfPlanes[3].a = mvpMat[0][3] - mvpMat[0][1];
	vfPlanes[3].b = mvpMat[1][3] - mvpMat[1][1];
	vfPlanes[3].c = mvpMat[2][3] - mvpMat[2][1];
	vfPlanes[3].d = mvpMat[3][3] - mvpMat[3][1];

	//Near
	vfPlanes[4].a = mvpMat[0][3] + mvpMat[0][2];
	vfPlanes[4].b = mvpMat[1][3] + mvpMat[1][2];
	vfPlanes[4].c = mvpMat[2][3] + mvpMat[2][2];
	vfPlanes[4].d = mvpMat[3][3] + mvpMat[3][2];

	//Far
	vfPlanes[5].a = mvpMat[0][3] - mvpMat[0][2];
	vfPlanes[5].b = mvpMat[1][3] - mvpMat[1][2];
	vfPlanes[5].c = mvpMat[2][3] - mvpMat[2][2];
	vfPlanes[5].d = mvpMat[3][3] - mvpMat[3][2];

	//Normalize the planes
	for (int i = 0; i < 6; i++)
	{
		vfPlanes[i].NormalizePlane();
	}
}
bool FrustumCullingHandle::CheckModelCulling(glm::mat4 modelMatrix_, BoundingBox box_)
{
	CalculateFrustumPlanes(modelMatrix_);

	glm::vec3 axisVert = glm::vec3();
	glm::vec3 planeNormal = glm::vec3();

	//Find the vertext that is furthest from the plane in the direction of the plane's normal
	for (int i = 0; i < 6; i++)
	{
		planeNormal = glm::vec3(vfPlanes[i].a, vfPlanes[i].b, vfPlanes[i].c);

		//Left and right
		if (vfPlanes[i].a < 0.0f)
			axisVert.x = box_.minVert.x;
		else
			axisVert.x = box_.maxVert.x;

		//Top and bottom
		if (vfPlanes[i].b < 0.0f)
			axisVert.y = box_.minVert.y;
		else
			axisVert.y = box_.maxVert.y;

		//Near and far
		if (vfPlanes[i].c < 0.0f)
			axisVert.z = box_.minVert.z;
		else
			axisVert.z = box_.maxVert.z;

		//Check the distance
		float distance = glm::dot(planeNormal, axisVert) + vfPlanes[i].d;
		if (distance < 0) //Object is outside the frustum
			return false;
	}
	return true;
}
FrustumCullingHandle* FrustumCullingHandle::GetInstance()
{
	if (!instance)
		instance.reset(new FrustumCullingHandle);
	return instance.get();
}
