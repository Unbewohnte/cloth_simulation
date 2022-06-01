#include "cloth.hpp"
#include "connection.hpp"
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
            // if (y == 0 && x == new_cloth->dimensions.x/2) {
            //     frozen_point = true;
            // } else {
            //     frozen_point = false;
            // }
            
            Point* new_point = new Point{
                .x = static_cast<float>(startpos.x + x),
                .y = static_cast<float>(startpos.y + y),
                .mass = 10.0f,
                .frozen = frozen_point,
                .selected = false,
            };
            new_cloth->points.push_back(new_point);

            if (x > 0) {
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

// calculate forces and actually MOVE points 
void cloth_step(Cloth* cloth, Vec2f gravity_vec, float timedelta) {
    for (Point* p : cloth->points) {
        if (p->frozen) {
            continue;
        }
        
        // calculate velocity
        if (p->y == cloth->constraint_bottom_right.y) {
            // if this point's x velocity is too small -> just stop it
            // else -> handle friction
            if (p->velocity.x <= 0.5) {
                p->velocity.x = 0;
            } else {
                p->velocity.x = p->velocity.x*cloth->friction_factor * timedelta;
            }
        } else {
            p->velocity.x = p->velocity.x+(gravity_vec.x/p->mass) * timedelta;
            p->velocity.y = p->velocity.y+(gravity_vec.y/p->mass) * timedelta;
        }

        // move point
        p->x += p->velocity.x;
        p->y += p->velocity.y;
    }
}

// handle constraints, bounces
void satisfy_cloth_constraints(Cloth* cloth) {
    for (Point* p : cloth->points) {
        // bottom
        if (p->y > cloth->constraint_bottom_right.y) {
            p->y = cloth->constraint_bottom_right.y;
            p->velocity.y = -p->velocity.y*cloth->efficiency_factor;
        }

        // right
        if (p->x > cloth->constraint_bottom_right.x) {
            p->x = cloth->constraint_bottom_right.x;
            p->velocity.x = -p->velocity.x*cloth->efficiency_factor;
        }

        // left
        if (p->x < cloth->constraint_top_left.x) {
            p->x = cloth->constraint_top_left.x;
            p->velocity.x = -p->velocity.x*cloth->efficiency_factor;
        }

        // up
        if (p->y < cloth->constraint_top_left.y) {
            p->y = cloth->constraint_top_left.y;
            p->velocity.y = -p->velocity.y*cloth->efficiency_factor;
        }
    }
}