//
// Created by alireza on 25/05/22.
//

#include "SparsityConstraint.h"
namespace scot
{

	SparsityConstraint::SparsityConstraint(int number_of_non_zeros, double big_m_parameter) : number_of_non_zeros_(
		number_of_non_zeros), big_m_parameter_(big_m_parameter)
	{
	}
	SparsityConstraint::~SparsityConstraint() = default;
	int SparsityConstraint::getNumberOfNonZeros() const
	{
		return number_of_non_zeros_;
	}
	void SparsityConstraint::setNumberOfNonZeros(int new_number_of_non_zeros)
	{
		SparsityConstraint::number_of_non_zeros_ = new_number_of_non_zeros;
	}
	double SparsityConstraint::getBigMParameter() const
	{
		return big_m_parameter_;
	}
	void SparsityConstraint::setBigMParameter(double new_big_m_parameter)
	{
		SparsityConstraint::big_m_parameter_ = new_big_m_parameter;
	}
	SparsityModelingMethods SparsityConstraint::getModelingMethod() const
	{
		return modeling_method_;
	}
	void SparsityConstraint::setModelingMethod(SparsityModelingMethods new_modeling_method)
	{
		SparsityConstraint::modeling_method_ = new_modeling_method;
	}
	bool SparsityConstraint::isFeasible() const
	{
		return is_feasible_;
	}
}