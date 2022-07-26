#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "geometry.h"
#include "tgaimage.h"


class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_;
    std::vector<Vec2f> uv_;
    std::vector<Vec3f> norms_;
    TGAImage diffusemap_;
    void load_texture(std::string filename, std::string suffix, TGAImage& image);
public:
    Model(const char *filename);
    ~Model();
    int nverts() const;
    int nfaces() const;
    Vec3f vert(int i) const;
    TGAColor diffuse(Vec2i uv);
    Vec2i uv(int iface, int nvert);
    std::vector<int> face(int idx) const;
};

#endif //__MODEL_H__
