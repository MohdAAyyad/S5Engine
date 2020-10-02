#ifndef FRUSTUMCULLINGHANDLE_H
#define FRUSTUMCULLINGHANDLE_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Math/BoundingBox.h"

struct Plane
{
	float a, b, c, d;
	inline void NormalizePlane()
	{
		float length = sqrt((a*a) + (b * b) + (c * c) + (d * d));
		a /= length;
		b /= length;
		c /= length;
		d /= length;
	}
};

class FrustumCullingHandle
{
public:
	FrustumCullingHandle(const FrustumCullingHandle&) = delete; //Disable the copy constructor
	FrustumCullingHandle(FrustumCullingHandle&&) = delete; //Disable the move constructor
	FrustumCullingHandle& operator=(const FrustumCullingHandle&) = delete; //Disable operator = for copy and move
	FrustumCullingHandle& operator=(FrustumCullingHandle&&) = delete;

	static FrustumCullingHandle* GetInstance();
	bool CheckModelCulling(glm::mat4 modelMatrix_, BoundingBox box_);

private:

	FrustumCullingHandle();

	Plane vfPlanes[6];

	void CalculateFrustumPlanes(glm::mat4 modelMatrix_);
	static std::unique_ptr<FrustumCullingHandle> instance;
	friend std::default_delete<FrustumCullingHandle>;
};

#endif

