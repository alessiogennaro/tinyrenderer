
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "model.h"


Model::Model(const char *filename) : verts_(), faces_(), uv_(), norms_(), diffusemap_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char ctrash;
        
        if (!line.compare(0, 2, "v ")) {
            iss >> ctrash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> ctrash >> ctrash;
            Vec2f uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uv_.push_back(uv);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> ctrash >> ctrash;
            Vec3f n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 2, "f ")) {
            iss >> ctrash;
            std::vector<Vec3i> f;
            Vec3i temp;
            while (iss >> temp[0] >> ctrash >> temp[1] >> ctrash >> temp[2]) {
                for (int i = 0; i < 3; i++) temp[i]--;
                f.push_back(temp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
}

int Model::nverts() const {
    return (int)verts_.size();
}

int Model::nfaces() const {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) const {
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i].x);
    return face;
}

Vec3f Model::vert(int i) const {
    return verts_[i];
}

void Model::load_texture(std::string filename, std::string suffix, TGAImage& image) {
    std::string textfile(filename);
    std::size_t dot = textfile.find_last_of(".");
    if (dot != std::string::npos) {
        textfile = textfile.substr(0, dot) + std::string(suffix);
        std::cerr << "Texture file " << textfile << " loading "
            << (image.read_tga_file(textfile.c_str()) ? "DONE" : "FAILED")
            << std::endl;
        image.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2i uv) {
    return diffusemap_.get(uv.x, uv.y);
}

Vec2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    return Vec2i{
        uv_[idx].x * diffusemap_.get_width(),
        uv_[idx].y * diffusemap_.get_height()
    };
}

Vec3f Model::norm(int iface, int nvert) {
    int idx = faces_[iface][nvert][2];
    return norms_[idx].normalize();
}
