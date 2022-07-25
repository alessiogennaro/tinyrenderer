#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec2f> text_verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char *filename);
	~Model();
	int nverts() const;
	int nfaces() const;
	int ntextverts() const;
	Vec3f vert(int i) const;
	Vec2f text_vert(int i) const;
	std::vector<int> face(int idx) const;
};

#endif //__MODEL_H__
