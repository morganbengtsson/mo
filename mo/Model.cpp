/* 
 * File:   Model.cpp
 * Author: morgan
 * 
 * Created on March 5, 2014, 10:29 PM
 */

#include "Model.h"

namespace mo {

    Model::Model(){
        
    }
    Model::Model(std::shared_ptr<Mesh> mesh, 
                 std::shared_ptr<Texture2D> texture,
                 const glm::mat4 transform,
                 const Draw draw,
                 std::shared_ptr<Material> material,
                 std::shared_ptr<Texture2D> lightmap) :
    draw(draw),             
    mesh(mesh),
    texture(texture),
    material(material),
    lightmap(lightmap),
    transform(transform),
    valid_(true),
    color(color){
    }

    Model::~Model() {
    }
   
    glm::vec3 Model::position() {
        return (glm::vec3)(transform*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

}