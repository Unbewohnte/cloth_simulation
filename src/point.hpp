#pragma once

// struct Point {
//     unsigned int x;
//     unsigned int y;
//     unsigned int prev_x;
//     unsigned int prev_y;
//     unsigned int mass;
//     bool selected;
// };

struct Point {
    float x;
    float y;
    float prev_x;
    float prev_y;
    float mass;
    bool frozen;
    bool selected;
};