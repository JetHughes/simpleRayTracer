#pragma once

#ifndef OBJECT_GROUP_H_INCLUDED
#define OBJECT_GROUP_H_INCLUDED

#include "Object.h"
#include <vector>

/** 
 * \brief A group of Objects with a common Transform
 * 
 * An ObjectGroup allows you to collect together multiple objects.
 * This allows you to apply a Transform to the group as a whole.
 */
class ObjectGroup : public Object {

public:

	/** \brief ObjectGroup constructor */
	ObjectGroup() = default;

	/** \brief ObjectGroup destructor */
	~ObjectGroup() = default;

	/**
	 * \brief ObjectGroup assignment operator.
	 *
	 * \param objectGroup The ObjectGroup to assign to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 */
	ObjectGroup& operator=(const ObjectGroup& objectGroup) = default;

	/**
	 * \brief ObjectGroup-Ray intersection computation.
	 *
	 * The ObjectGroup intersection returns a list of all intersections
	 * with Objects in the group. 
	 * 
	 * \param ray The Ray to intersect with this ObjectGroup.
	 * \return A list (std::vector) of intersections, which may be empty.
	 */
	std::vector<RayIntersection> intersect(const Ray& ray) const;

	/** \brief Add a new Object to the ObjectGroup.
	 *
	 * Note that the ObjectGroup has a collection of Objects, and there is no
	 * way to remove an Object once added.
	 *
	 * \param objectPtr A \c std::shared_ptr to the new Object to add.
	 */
	void addObject(std::shared_ptr<Object> objectPtr);

private:

	std::vector<std::shared_ptr<Object>> objects; //!< The Objects in this ObjectGroup

};

#endif