
#include "triangle_renderer.h"

#include <array>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"


void triangle_renderer(Model const* model, TGAImage& image, Vec3f light_vec) {    
    for (int i = 0; i < model->nfaces(); i++) {
		
        std::vector<int>     face {model->face(i)};
		std::array<Vec2i, 3> screen_coords {};
		std::array<Vec3f, 3> world_coords {};

		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert( face[j] );
			screen_coords[j] = Vec2i{
				(v.x + 1.) * CANVAS_WIDTH / 2.,
				(v.y + 1.) * CANVAS_HEIGHT / 2.
			};
			world_coords[j] = v;
		}
		// Norm: perpendicular vector to the face
        // It's the cross-product of the two sides
        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
		n.normalize();

        // Light intensity: scalar product of the light vector and the norm
		float intensity = n * light_vec;

		if (intensity > 0.) {
			int color = intensity * 255;
			triangle(screen_coords, image, TGAColor(color, color, color, 255));
		}
        // Back-face culling: if intensity <= 0 it means that the light comes from behind
        // so we don't draw that triangle
	
    }

}
