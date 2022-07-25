
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <vector>
#include "constants.h"
#include "geometry.h"
#include "tgaimage.h"

class Texture {
private:
    TGAImage texture_;
    std::vector<Vec2f> text_verts_;
    std::vector<std::vector<int> > faces_;
public:
    Texture(const char *texture, const char *obj);
    ~Texture();
    TGAImage texture() const;
    TGAColor get_color(int x, int y);
    int nfaces() const;
    int ntextverts() const;
    Vec2f text_vert(int i) const;
    Vec2f text_vert_scaled(int i) const;
    std::vector<int> face(int idx) const;
};

#endif // __TEXTURE_H__
