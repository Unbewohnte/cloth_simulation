#include "cloth.hpp"
#include "connection.hpp"
#include <cstddef>
#include <cstdio>
#include <vector>

Cloth* new_cloth(Vec2 startpos, Vec2 dimensions, unsigned int spacing) {
    Cloth* new_cloth = new Cloth;
    new_cloth->startpos = startpos;
    new_cloth->dimensions = dimensions;
    new_cloth->points = std::vector<Point*>();
    new_cloth->connections = std::vector<Connection*>();

    for (unsigned int y = 0; y < dimensions.y; y += spacing) {
        for (unsigned int x = 0; x < dimensions.x; x += spacing) {
            Point* new_point = new Point{static_cast<float>(startpos.x + x), static_cast<float>(startpos.y + y), startpos.x + x, startpos.y + y, false};
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

void compute_cloth_forces(Cloth* cloth, Vec2f gravity_vec) {
    for (Point* p : cloth->points) {
        p->x += gravity_vec.x;
        p->y += gravity_vec.y;    
    }
}