#ifndef MO_BOX_HPP
#define MO_BOX_HPP

#include <array>
#include <glm/glm.hpp>
#include <utility>
#include <algorithm>
#include "ray.hpp"
#include "vertex.hpp"

namespace mo
{

class Box
{
public:
    template<class VertexIt>
    Box(VertexIt begin, VertexIt end, const glm::mat4 & transform) : transform(transform){
        glm::vec3 min, max;

        if (begin != end){
            auto x_extremes = std::minmax_element(begin, end,
                                                  [](const Vertex& left, const Vertex& right) {
                return left.position.x < right.position.x;
            });

            auto y_extremes = std::minmax_element(begin, end,
                                                  [](const Vertex& left, const Vertex& right) {
                return left.position.y < right.position.y;
            });

            auto z_extremes = std::minmax_element(begin, end,
                                                  [](const Vertex& left, const Vertex& right) {
                return left.position.z < right.position.z;
            });

            min = glm::vec3(x_extremes.first->position.x, y_extremes.first->position.y, z_extremes.first->position.z);
            max = glm::vec3(x_extremes.second->position.x, y_extremes.second->position.y, z_extremes.second->position.z);
        }
        parameters[0] = (glm::vec3)(transform * glm::vec4(min, 1.0f));
        parameters[1] = (glm::vec3)(transform * glm::vec4(max, 1.0f));
    }

    Box();
    Box(const glm::vec3 & min, const glm::vec3 & max, const glm::mat4 & transform);
    bool intersect(const Ray &, float t0, float t1) const;
    glm::vec3 parameters[2];
    glm::mat4 transform;
    std::array<glm::vec3, 8> positions() const;

    bool intersect(glm::vec3 point1, glm::vec3 point2, glm::vec3 &Hit)
    {
        glm::vec3 min = parameters[0];
        glm::vec3 max = parameters[1];
        if (point2.x < min.x && point1.x < min.x) return false;
        if (point2.x > max.x && point1.x > max.x) return false;
        if (point2.y < min.y && point1.y < min.y) return false;
        if (point2.y > max.y && point1.y > max.y) return false;
        if (point2.z < min.z && point1.z < min.z) return false;
        if (point2.z > max.z && point1.z > max.z) return false;
        if (point1.x > min.x && point1.x < max.x &&
                point1.y > min.y && point1.y < max.y &&
                point1.z > min.z && point1.z < max.z)
        {Hit = point1;
            return true;}
        if ( (GetIntersection( point1.x-min.x, point2.x-min.x, point1, point2, Hit) && InBox( Hit, min, max, 1 ))
             || (GetIntersection( point1.y-min.y, point2.y-min.y, point1, point2, Hit) && InBox( Hit, min, max, 2 ))
             || (GetIntersection( point1.z-min.z, point2.z-min.z, point1, point2, Hit) && InBox( Hit, min, max, 3 ))
             || (GetIntersection( point1.x-max.x, point2.x-max.x, point1, point2, Hit) && InBox( Hit, min, max, 1 ))
             || (GetIntersection( point1.y-max.y, point2.y-max.y, point1, point2, Hit) && InBox( Hit, min, max, 2 ))
             || (GetIntersection( point1.z-max.z, point2.z-max.z, point1, point2, Hit) && InBox( Hit, min, max, 3 )))
            return true;

        return false;
    }

private:
    bool inline GetIntersection( float fDst1, float fDst2, glm::vec3 P1, glm::vec3 P2, glm::vec3 &Hit) {
        if ( (fDst1 * fDst2) >= 0.0f) return false;
        if ( fDst1 == fDst2) return false;
        Hit = P1 + (P2-P1) * ( -fDst1/(fDst2-fDst1) );
        return true;
    }

    int inline InBox( glm::vec3 Hit, glm::vec3 B1, glm::vec3 B2, const int Axis) {
        if ( Axis==1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
        if ( Axis==2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
        if ( Axis==3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
        return 0;
    }

};
}
#endif // MO_BOX_HPP
