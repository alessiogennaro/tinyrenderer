
#include "wireframe_renderer.h"

#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"


void wireframe_renderer(Model const* model, TGAImage& image) {
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face {model->face(i)};

        // every face is made of three vertices
        for (int j = 0; j < 3; j++) {

            // we draw a line between each of the three vertices
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);

            int x0 = (v0.x + 1.) * (CANVAS_WIDTH / 2.0);
            int y0 = (v0.y + 1.) * (CANVAS_HEIGHT / 2.0);

            int x1 = (v1.x + 1.) * (CANVAS_WIDTH / 2.0);
            int y1 = (v1.y + 1.) * (CANVAS_HEIGHT / 2.0);

            line(x0, y0, x1, y1, image, white);
        }
    }

}
