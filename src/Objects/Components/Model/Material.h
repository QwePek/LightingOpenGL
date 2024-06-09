#pragma once
#include "glm\glm.hpp"

//Values from - http://devernay.free.fr/cours/opengl/materials.html
namespace Material
{
    enum Type
    {
        None, Custom,
        Emerald, Jade, Obsidian,
        Pearl, Ruby, Turquoise,
        Brass, Bronze, Chrome,
        Copper, Gold, Silver,
        Wood,
        MATERIAL_SIZE
    };

	static glm::vec3 getAmbientOfType(Type type) {
        switch (type) {
        case Type::None:
            return glm::vec3(1.0f, 1.0f, 1.0f);
        case Type::Emerald:
            return glm::vec3(0.0215f, 0.1745f, 0.0215f);
        case Type::Jade:
            return glm::vec3(0.135f, 0.2225f, 0.1575f);
        case Type::Obsidian:
            return glm::vec3(0.05375f, 0.05f, 0.06625f);
        case Type::Pearl:
            return glm::vec3(0.25f, 0.20725f, 0.20725f);
        case Type::Ruby:
            return glm::vec3(0.1745f, 0.01175f, 0.01175f);
        case Type::Turquoise:
            return glm::vec3(0.1f, 0.18725f, 0.1745f);
        case Type::Brass:
            return glm::vec3(0.329412f, 0.223529f, 0.027451f);
        case Type::Bronze:
            return glm::vec3(0.2125f, 0.1275f, 0.054f);
        case Type::Chrome:
            return glm::vec3(0.25f, 0.25f, 0.25f);
        case Type::Copper:
            return glm::vec3(0.19125f, 0.0735f, 0.0225f);
        case Type::Gold:
            return glm::vec3(0.24725f, 0.1995f, 0.0745f);
        case Type::Silver:
            return glm::vec3(0.19225f, 0.19225f, 0.19225f);
        case Type::Wood:
            return glm::vec3(0.3f, 0.2f, 0.1f);
        default:
            return glm::vec3(0.0f);
        }

	}

	static glm::vec3 getDiffuseOfType(Type type) {
        switch (type) {
        case Type::None:
            return glm::vec3(1.0f, 1.0f, 1.0f);
        case Type::Emerald:
            return glm::vec3(0.07568f, 0.61424f, 0.07568f);
        case Type::Jade:
            return glm::vec3(0.54f, 0.89f, 0.63f);
        case Type::Obsidian:
            return glm::vec3(0.18275f, 0.17f, 0.22525f);
        case Type::Pearl:
            return glm::vec3(1.0f, 0.829f, 0.829f);
        case Type::Ruby:
            return glm::vec3(0.61424f, 0.04136f, 0.04136f);
        case Type::Turquoise:
            return glm::vec3(0.396f, 0.74151f, 0.69102f);
        case Type::Brass:
            return glm::vec3(0.780392f, 0.568627f, 0.113725f);
        case Type::Bronze:
            return glm::vec3(0.714f, 0.4284f, 0.18144f);
        case Type::Chrome:
            return glm::vec3(0.4f, 0.4f, 0.4f);
        case Type::Copper:
            return glm::vec3(0.7038f, 0.27048f, 0.0828f);
        case Type::Gold:
            return glm::vec3(0.75164f, 0.60648f, 0.22648f);
        case Type::Silver:
            return glm::vec3(0.50754f, 0.50754f, 0.50754f);
        case Type::Wood:
            return glm::vec3(0.6f, 0.4f, 0.2f);
        default:
            return glm::vec3(0.0f);
        }
	}

	static glm::vec3 getSpecularOfType(Type type) {
        switch (type) {
        case Type::None:
            return glm::vec3(1.0f, 1.0f, 1.0f);
        case Type::Emerald:
            return glm::vec3(0.633f, 0.727811f, 0.633f);
        case Type::Jade:
            return glm::vec3(0.316228f, 0.316228f, 0.316228f);
        case Type::Obsidian:
            return glm::vec3(0.332741f, 0.328634f, 0.346435f);
        case Type::Pearl:
            return glm::vec3(0.296648f, 0.296648f, 0.296648f);
        case Type::Ruby:
            return glm::vec3(0.727811f, 0.626959f, 0.626959f);
        case Type::Turquoise:
            return glm::vec3(0.297254f, 0.30829f, 0.306678f);
        case Type::Brass:
            return glm::vec3(0.992157f, 0.941176f, 0.807843f);
        case Type::Bronze:
            return glm::vec3(0.393548f, 0.271906f, 0.166721f);
        case Type::Chrome:
            return glm::vec3(0.774597f, 0.774597f, 0.774597f);
        case Type::Copper:
            return glm::vec3(0.256777f, 0.137622f, 0.086014f);
        case Type::Gold:
            return glm::vec3(0.628281f, 0.555802f, 0.366065f);
        case Type::Silver:
            return glm::vec3(0.508273f, 0.508273f, 0.508273f);
        case Type::Wood:
            return glm::vec3(0.1f, 0.1f, 0.1f);
        default:
            return glm::vec3(0.0f);
        }
	}

	static float getShininessOfType(Type type) {
        switch (type) {
        case Type::None:
            return 0.25; //do potegi 32
        case Type::Emerald:
            return 0.6f;
        case Type::Jade:
            return 0.1f;
        case Type::Obsidian:
            return 0.3f;
        case Type::Pearl:
            return 0.088f;
        case Type::Ruby:
            return 0.6f;
        case Type::Turquoise:
            return 0.1f;
        case Type::Brass:
            return 0.21794872f;
        case Type::Bronze:
            return 0.2f;
        case Type::Chrome:
            return 0.6f;
        case Type::Copper:
            return 0.1f;
        case Type::Gold:
            return 0.4f;
        case Type::Silver:
            return 0.4f;
        case Type::Wood:
            return 0.078125f;
        default:
            return 32;
        }
	}

    struct Material {
        Material(Type typ) {
            type = typ;
            ambient = getAmbientOfType(type);
            diffuse = getDiffuseOfType(type);
            specular = getSpecularOfType(type);
            shininess = getShininessOfType(type);
        }

        Material(glm::vec3 color) {
            type = Type::Custom;
            ambient = color;
            diffuse = color;
            specular = glm::vec3(0.5f,0.5f,0.5f);
            shininess = 32;
        }

        Material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess) {
            type = Type::Custom;
            ambient = _ambient;
            diffuse = _diffuse;
            specular = _specular;
            shininess = _shininess;
        }

        Type type;
        glm::vec3 ambient, diffuse, specular;
        float shininess;
    };
}