
#include <array>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "random.h"
#include "tgaimage.h"
#include "triangle.h"
#include "wireframe_renderer.h"


std::unique_ptr<Model> model {};


int main(int argc, char** argv) {
	
	if (argc == 2) {
		model = std::make_unique<Model>(argv[1]);
	} else {
		model = std::make_unique<Model>("obj/african_head.obj");
	}

	TGAImage image(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

	/* -------------------------------------------------------------- */

	/*
	// 1st triangle (red)
	std::array<Vec2i, 3> t0 = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
	// 2nd triangle (white)
	std::array<Vec2i, 3> t1 = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
	// 3rd triangle (green)
	std::array<Vec2i, 3> t2 = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
	
	filled_triangle(t0[0], t0[1], t0[2], image, red);
	filled_triangle(t1[0], t1[1], t1[2], image, white); 
	filled_triangle(t2[0], t2[1], t2[2], image, green);
	*/

	/*
	std::array<Vec2i, 3> pts = {Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160)};
	filled_triangle(pts, image, red);
	*/

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		std::array<Vec2i, 3> screen_coords {};
		for (int j = 0; j < 3; j++) {
			Vec3f world_coords = model->vert( face[j] );
			screen_coords[j] = Vec2i{
				(world_coords.x + 1.) * CANVAS_WIDTH / 2.,
				(world_coords.y + 1.) * CANVAS_HEIGHT / 2.
			};
		}
		filled_triangle(screen_coords, image, TGAColor(
			deterministic::random_int(0, 255),
			deterministic::random_int(0, 255),
			deterministic::random_int(0, 255),
			255
		) );
	}

	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the image
	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;
}
