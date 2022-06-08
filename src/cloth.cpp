#include "cloth.hpp"
#include "connection.hpp"
#include "mouse.hpp"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

Cloth* new_cloth(
    Vec2 startpos,
    Vec2 dimensions,
    unsigned int spacing,
    float efficiency_factor,
    float friction_factor,
    Vec2 constraint_top_left, Vec2 constraint_bottom_right
    ) {

    Cloth* new_cloth = new Cloth;
    new_cloth->efficiency_factor = efficiency_factor;
    new_cloth->friction_factor = friction_factor;
    new_cloth->startpos = startpos;
    new_cloth->dimensions = dimensions;
    new_cloth->constraint_top_left = constraint_top_left;
    new_cloth->constraint_bottom_right = constraint_bottom_right;
    new_cloth->points = std::vector<Point*>();
    new_cloth->connections = std::vector<Connection*>();

    bool frozen_point = false;
    for (unsigned int y = 0; y < dimensions.y; y += spacing) {        
        for (unsigned int x = 0; x < dimensions.x; x += spacing) {
            Point* new_point = new Point{
                .pos = Vec2f{static_cast<float>(startpos.x + x), static_cast<float>(startpos.y + y)},
                .mass = 10.0f,
                .frozen = frozen_point,
                .selected = false,
                .radius = 2.5,
            };
            new_cloth->points.push_back(new_point);

            if (x != 0) {
                Point* p0_left = new_cloth->points[new_cloth->points.size() - 2];
                Connection* new_conn = new Connection{*p0_left, *new_point};
                new_cloth->connections.push_back(new_conn);
            }

            if (y != 0) {
                Point* p0_up = new_cloth->points[new_cloth->points.size()-1 - dimensions.x/spacing];
                Connection* new_conn = new Connection{*p0_up, *new_point};
                new_cloth->connections.push_back(new_conn);
            }
        }
    }

    return new_cloth;
}

void destroy_cloth(Cloth* cloth) {
    if (cloth == NULL) {
        return;
    }

    for (unsigned int i = 0; i < cloth->points.size(); i++) {
        delete cloth->points[i];
    }


    for (unsigned int i = 0; i < cloth->connections.size(); i++) {
        delete cloth->connections[i];
    }

    cloth->points.clear();
    cloth->connections.clear();
    
    delete cloth;
}

// calculate forces, move points and satisfy constraints 
void cloth_step(Cloth* cloth, Mouse* mouse, Vec2f gravity_vec, float timedelta) {
    for (Point* p : cloth->points) {
        // check if this point is in pointer's area 
        // ((x0 - x1)^2 + (y0 - y1)^2)^0.5
        float distance_to_point = std::sqrt(pow(p->pos.x - mouse->pos.x, 2.0f)+pow(p->pos.y - mouse->pos.y, 2.0f));
        if (distance_to_point <= mouse->cursor_radius_size) {
            p->selected = true;
        } else {
            p->selected = false;
        }

        // skip any calculations if this point is frozen
        if (p->frozen) {
            continue;
        }

        // calculate velocity
        p->acceleration.x = gravity_vec.x / p->mass;
        p->acceleration.y = gravity_vec.y / p->mass;

        if (p->selected && mouse->left_button_clicked) {
            Vec2f difference = {
                (float) (mouse->pos.x - p->pos.x) * cloth->efficiency_factor,
                (float) (mouse->pos.y - p->pos.y) * cloth->efficiency_factor,
            };

            p->acceleration.x = difference.x / p->mass;
            p->acceleration.y = difference.y / p->mass;
        }

        // apply calculated velocity
        p->velocity.x = p->velocity.x + p->acceleration.x * timedelta;
        p->velocity.y = p->velocity.y + p->acceleration.y * timedelta;

        // now handle constraints
        // bottom
        if (p->pos.y >= cloth->constraint_bottom_right.y) {
            p->pos.y = cloth->constraint_bottom_right.y; 
            p->velocity.x = p->velocity.x * cloth->friction_factor; // friction on the floor
            p->velocity.y = -p->velocity.y * cloth->efficiency_factor;
        }

        // right
        if (p->pos.x >= cloth->constraint_bottom_right.x) {
            p->pos.x = cloth->constraint_bottom_right.x;
            p->velocity.x = -p->velocity.x * cloth->efficiency_factor;
        }

        // left
        if (p->pos.x <= cloth->constraint_top_left.x) {
            p->pos.x = cloth->constraint_top_left.x;
            p->velocity.x = -p->velocity.x * cloth->efficiency_factor;
        }

        // up
        if (p->pos.y <= cloth->constraint_top_left.y) {
            p->pos.y = cloth->constraint_top_left.y;
            p->velocity.y = -p->velocity.y * cloth->efficiency_factor;
        }
        
        // move point
        p->pos.x += p->velocity.x;
        p->pos.y += p->velocity.y;
    }
}