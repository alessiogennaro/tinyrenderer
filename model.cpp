#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
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
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> ctrash;
            while (iss >> idx >> ctrash >> itrash >> ctrash >> itrash) {
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
    std::cerr << "#MODEL v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << text_verts_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() const {
    return (int)verts_.size();
}

int Model::nfaces() const {
    return (int)faces_.size();
}

int Model::ntextverts() const {
    return (int)text_verts_.size();
}

std::vector<int> Model::face(int idx) const {
    return faces_[idx];
}

Vec3f Model::vert(int i) const {
    return verts_[i];
}

Vec2f Model::text_vert(int i) const {
    return text_verts_[i];
}
