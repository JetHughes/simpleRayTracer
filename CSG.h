#pragma once

#ifndef CSG_H_INCLUDED
#define CSG_H_INCLUDED

#include "Object.h"
#include <memory>

/**
 * \file
 * \brief CSG class header file.
 */

/**
 * \brief Class for CSG operations on Objects.
 *
 * This class provides an Object which is a combination of two other Objects.
 * The two objects can be combined as the Union, Intersection, or Difference of the two input Objects.
 * Note that Difference is not symmetric, so \f$A-B\f$ is different from \f$B-A\f$.
 * Examples of these operations are illustrated below:
 * 
 * \image html CSG.png
 */
class CSG : public Object {

public:
	/**
	 * \brief Enumeration of CSG operations.
	 * 
	 * Provides easy values to refer to Union, Intersection, and Difference operations.
	 */
	enum Operation {
		UNION, INTERSECTION, DIFFERENCE
	};

	/**
	 * \brief CSG constructor.
	 * 
	 * \param operation The CSG Operation to apply.
	 */
	CSG(Operation operation);
	
	/**
	 * \brief CSG destructor.
	 */
	~CSG() = default;

	/**
	 * \brief CSG assignment operator.
	 *
	 * \param csg The CSG to assign to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 */
	CSG& operator=(const CSG& csg) = default;

	/**
	 * \brief CSG-Ray intersection computation.
	 * 
	 * \todo The CSG-Ray intersection needs to be implemented as part of the assignment.
	 * 
	 * \param ray The Ray to intersect with this CSG Object.
	 * \return A list (std::vector) of intersections, which may be empty.
	 */
	std::vector<RayIntersection> intersect(const Ray& ray) const;

	/** \brief Set the left-hand Object of the CSG operation
	 *
	 * \param obj The first Object in the CSG operation.
	 */
	void setLeft(std::shared_ptr<Object> obj) { left = obj; }

	/** \brief Set the right-hand Object of the CSG operation
	 * 
	 * \param obj The second Object in the CSG operation.
	 */
	void setRight(std::shared_ptr<Object> obj) { right = obj; }

private:

	Operation operation; //!< The type of CSG Operation
	std::shared_ptr<Object> left; //!< The first Object in the CSG Operation
	std::shared_ptr<Object> right; //!< The second Object in the CSG Operation

};

#endif#pragma once
