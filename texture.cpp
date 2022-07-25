
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "texture.h"
#include "tgaimage.h"

Texture::Texture(const char *texture, const char *obj): texture_() {
    bool res = texture_.read_tga_file(texture);
    texture_.flip_vertically();
    if (!res) return;
    
    std::ifstream in;
    in.open (obj, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char ctrash;
        
        if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> ctrash;
            //               x         /       y        /         z
            while (iss >> itrash >> ctrash >> idx >> ctrash >> itrash) {
                // in wavefront obj all indices start at 1, not zero
                idx--;
                f.push_back(idx);
            }
            faces_.push_back(f);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> ctrash >> ctrash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt.raw[i];
            text_verts_.push_back(vt);
        }
    }
    std::cerr << "#TEXTURE f# "  << faces_.size() << " vt# " << text_verts_.size() << std::endl;
}

Texture::~Texture() {
}

TGAImage Texture::texture() const {
    return texture_;
}

TGAColor Texture::get_color(int x, int y) {
    return texture_.get(x, y);
}

int Texture::nfaces() const {
    return (int)faces_.size();
}

int Texture::ntextverts() const {
    return (int)text_verts_.size();
}

Vec2f Texture::text_vert(int i) const {
    return text_verts_[i];
}

Vec2f Texture::text_vert_scaled(int i) const {
    Vec2f vert = text_vert(i);
    vert.u = vert.u * CANVAS_WIDTH;
    vert.v = vert.v * CANVAS_HEIGHT;
    return vert;
}

std::vector<int> Texture::face(int idx) const {
    return faces_[idx];
}
