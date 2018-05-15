#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "BumpMap.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr, BumpMap *bump = nullptr );

	void setMaterial( Material *material );
	Intersection intersect(const Ray & r);
	Material *m_material;
	Primitive *m_primitive;
	BumpMap *m_bump;
	Texture *tex;
};

