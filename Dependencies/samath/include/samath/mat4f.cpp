#include "mat4f.h"

using namespace sam;
mat4f mat4f::Ortho(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	mat4f v;
	v <<
		2/(xMax-xMin), 0, 0, -(xMax+xMin)/(xMax-xMin),
		0, 2/(yMax-yMin), 0, -(yMax+yMin)/(yMax-yMin),
		0, 0, -2/(zMax-zMin), -(zMax+zMin)/(zMax-zMin),
		0, 0, 0, 1;
	return v;
}
