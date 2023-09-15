//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_SPARSITYCONSTRAINT_H_
#define DISCOT_SRC_SPARSITYCONSTRAINT_H_
#include "../Solver/Enums.h"
#include "memory"

namespace Scot {
	class SparsityConstraint
	{
	public:
		virtual ~SparsityConstraint();
		SparsityConstraint(int number_of_non_zeros, double big_m_parameter);
		bool isFeasible() const;
		int getNumberOfNonZeros() const;
		void setNumberOfNonZeros(int new_number_of_non_zeros);
		double getBigMParameter() const;
		void setBigMParameter(double new_big_m_parameter);
		SparsityModelingMethods getModelingMethod() const;
		void setModelingMethod(SparsityModelingMethods new_modeling_method);
	private:
		int number_of_non_zeros_;
		double big_m_parameter_;
		SparsityModelingMethods modeling_method_;
		bool is_feasible_;

	};
	using SparsityConstraintPtr = std::shared_ptr<SparsityConstraint>;
}
#endif //DISCOT_SRC_SPARSITYCONSTRAINT_H_
//TODO: How to set modeling methods?