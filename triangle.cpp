
#include <iostream>

#include <cassert>
#include <utility>

#include "colors.h"
#include "geometry.h"
#include "line.h"
#include "tgaimage.h"
#include "triangle.h"

void triangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAImage& image, const TGAColor& color) {
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
}

void filled_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, const TGAColor& color) {
    if (t0.y > t1.y) std::swap(t0, t1);
    // t1.y > t0.y
    if (t0.y > t2.y) std::swap(t0, t2);
    // t1.y >? t2.y > t0.y
    if (t1.y > t2.y) std::swap(t1, t2);
    
    // t2.y > t1.y > t0.y

    assert(t2.y > t1.y && t1.y > t0.y && t2.y > t0.y);

    // line(t0, t1, image, green); // boundary B
    // line(t1, t2, image, green); // boundary B
    // line(t2, t0, image, red);   // boundary A: between the highest and lowest point

    int total_height = t2.y - t0.y;

    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        
        if (segment_height != 0. && total_height != 0.) {
            float alpha = (float) (y - t0.y) / total_height;
            float beta  = (float) (y - t0.y) / segment_height;

            Vec2i A = t0 + (t2 - t0) * alpha;
            Vec2i B = t0 + (t1 - t0) * beta;
            
            if (A.x > B.x) std::swap(A, B);

            for (int j = A.x; j <= B.x; j++)
                image.set(j, y, color);
        }
    }

    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height = t2.y - t1.y + 1;
        
        if (segment_height != 0. && total_height != 0.) {
            float alpha = (float) (y - t0.y) / total_height;
            float beta  = (float) (y - t1.y) / segment_height;

            Vec2i A = t0 + (t2 - t0) * alpha;
            Vec2i B = t1 + (t2 - t1) * beta;

            if (A.x > B.x) std::swap(A, B);

            for (int j = A.x; j <= B.x; j++)
                image.set(j, y, color);
        }
    }
}
