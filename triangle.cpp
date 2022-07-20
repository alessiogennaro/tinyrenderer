
#include <iostream>

#include <utility>

#include "geometry.h"
#include "line.h"
#include "tgaimage.h"
#include "triangle.h"

void triangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAImage& image, const TGAColor& color) {
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
}

void filled_triangle(Vec2i& t0, Vec2i& t1, Vec2i& t2, TGAImage& image, const TGAColor& color) {
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t2.y > t1.y) std::swap(t2, t1);
    
    Vec2i hy = t1;
    Vec2i lsp = (t0.x < t2.x ? t0 : t2);
    Vec2i rsp = (t0.x > t2.x ? t0 : t2);

    std::cout << "Highest y: " << hy << "\n";
    std::cout << "Left side point: " << lsp << "\n";
    std::cout << "Right side point: " << rsp << "\n";

    // we draw the border
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);

    /*
        Once we have drawn the borders we can start to move two points (say a and b).
        Both a and b start at the highest point (hy), then both step towards the
        left (a) and right (b) ends of the triangle (lsp and rsp).
        Once a step is done we draw a line between the points (a and b).
        This process ends when both a and b reach their ends (lsp and rsp).
    */
    
    Vec2i a = hy;
    Vec2i b = hy;

    while (a != lsp || b != rsp) {
        
        line(a, b, image, color);

        if (a != lsp) {
            a.y = a.y - 1;
            float t = (hy.y - a.y) / (float) (hy.y - lsp.y);
            a.x = hy.x * (1. - t) + lsp.x * t;
        }

        if (b != rsp) {
            b.y = b.y - 1;
            float t = (hy.y - b.y) / (float) (hy.y - rsp.y);
            b.x = hy.x * (1. - t) + rsp.x * t;
        }

    }

    /*
        This can be simplified further by moving only one point.
        Let's say that x is equal to the right side point.
        At the beginning we already drew the line between the highest point and x.
        So we advance x towards the left side point.
        Once we have done se we simply draw a line between the highest point and x.
        We repeat this process until x hasn't reached the left side point
    */

    // for (int x = lsp.x; x <= rsp.x; x++) {
    //     float t = (x - lsp.x) / (float) (rsp.x - lsp.x);
    //     int y = lsp.y * (1. - t) + rsp.y * t;
    //     line(hy, Vec2i{x, y}, image, color);
    // }
    
}
