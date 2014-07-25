/* 
 * File:   Assets.h
 * Author: morgan
 *
 * Created on June 12, 2014, 2:43 PM
 */

#ifndef MO_ASSETS_H
#define	MO_ASSETS_H

#include <string>
#include <map>
#include <memory>
#include <tuple>
#include "Mesh.h"
#include "Texture2D.h"
#include "Sound.h"
#include "Character.h"

namespace mo {
    class Assets {
    public:
        typedef std::map<std::string, std::shared_ptr<mo::Mesh>> MeshMap;
        typedef std::map<std::string, std::shared_ptr<mo::Texture2D>> TextureMap;
        typedef std::pair<std::string, std::shared_ptr<mo::Mesh>> MeshPair;
        typedef std::pair<std::string, std::shared_ptr<mo::Texture2D>> TexturePair;
        typedef std::map<std::string, std::shared_ptr<mo::Sound>> SoundMap;
        typedef std::pair<std::string, std::shared_ptr<mo::Sound>> SoundPair;
                
        
        virtual ~Assets(){};

        /**
         * Load a mesh and texture from file simultaneously.
         * 
         * Loads a mesh from an *.obj file and corresponding texture and caches them internally.
         * @param File path
         * @return Tuple of shared pointer to cached Mesh object and cached Texture2D object. 
         */
        //virtual std::tuple<std::shared_ptr<Mesh>, std::shared_ptr<Texture2D>> mesh_texture(std::string path) = 0;
        
        /**
         * Load a mesh from file
         * 
         * Loads a mesh from an *.obj file, and caches it internally.
         * @param File path
         * @return Shared pointer to cached Mesh object. 
         */
        virtual std::shared_ptr<Mesh> mesh(std::string path) = 0;
        
        /**
         * Load a texture from file.
         * 
         * Loads a texture from a PNG image file, and caches it internaly.
         * @param Path to the file
         * @return Shared pointer to cached Texture2D object
         */
        virtual std::shared_ptr<Texture2D> texture(std::string path) = 0;
        /**
         * Loads sound from a file.
         * 
         * Loads a sound buffer from a raw data file, containing 16 bit integer data. Cached internally.
         * @param The file path
         * @return Shared pointer to cached Sound object
         */
        virtual std::shared_ptr<Sound> sound(std::string path) = 0;
        
        /**
         * Loads text from file.
         * 
         * Loads text from a file, Not cached internally.
         * @param The file path
         * @return String object
         */
        virtual std::string text(std::string path) = 0;
        
        
        /**
         * Loads a charactermap from an xml definition.
         * 
         * @param The file path
         * @return A map of chars, with opposed Character object.
         */
        virtual std::map<char, Character> characterMap(std::string path) = 0;
        
    protected:
        MeshMap models_;
        TextureMap textures_;
        SoundMap sounds_;
    };
}

#endif	/* MO_ASSETS_H */

