#pragma once
#include "utils.h"
#include "vendor/Eigen/Dense"

namespace sam
{
	class mat4f : public Eigen::Matrix4f
	{
	public:
		mat4f(void) :Eigen::Matrix4f() {}

		// This constructor allows you to construct MyVectorType from Eigen expressions
		template<typename OtherDerived>
		mat4f(const Eigen::MatrixBase<OtherDerived>& other)
			: Eigen::Matrix4f(other){}

		// This method allows you to assign Eigen expressions to MyVectorType
		template<typename OtherDerived>
		mat4f& operator=(const Eigen::MatrixBase <OtherDerived>& other)
		{
			this->Eigen::Matrix4f::operator=(other);
			return *this;
		}

		static mat4f Ortho(float xMin, float xMax,
			float yMin, float yMax,
			float zMin, float zMax);
	};
}



