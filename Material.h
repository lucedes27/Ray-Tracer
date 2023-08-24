//
//
//

#ifndef RAY_TRACER_MATERIAL_H
#define RAY_TRACER_MATERIAL_H


class Material {
public:
    Vector3 kd; // Diffuse reflectance
    Vector3 ks; // Specular reflectance
    float shininess; // Shininess
    Vector3 emission; // Emission
    Vector3 ambient; // Ambient

    Material() : kd(0, 0, 0), ks(0, 0, 0), shininess(0), emission(0, 0, 0) {}
    Material(const Vector3& kd, const Vector3& ks, float shininess, const Vector3& emission, const Vector3& ambient = Vector3(0.2, 0.2, 0.2))
            : kd(kd), ks(ks), shininess(shininess), emission(emission), ambient(ambient) {}


    void setDiffuse(const Vector3& kd) {
        this->kd = kd;
    }

    void setSpecular(const Vector3& ks) {
        this->ks = ks;
    }

    void setShininess(float shininess) {
        this->shininess = shininess;
    }

    void setEmission(const Vector3& emission) {
        this->emission = emission;
    }

    void setAmbient(const Vector3& ambient) {
        this->ambient = ambient;
    }
};

bool operator==(const Material& lhs, const Material& rhs) {
    return lhs.kd == rhs.kd &&
           lhs.ks == rhs.ks &&
           lhs.shininess == rhs.shininess &&
           lhs.emission == rhs.emission &&
           lhs.ambient == rhs.ambient;
}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Material { "
       << "Diffuse: (" << material.kd.x << ", " << material.kd.y << ", " << material.kd.z << "), "
       << "Specular: (" << material.ks.x << ", " << material.ks.y << ", " << material.ks.z << "), "
       << "Shininess: " << material.shininess << ", "
       << "Emission: (" << material.emission.x << ", " << material.emission.y << ", " << material.emission.z << "), "
       << "Ambient: (" << material.ambient.x << ", " << material.ambient.y << ", " << material.ambient.z << ") "
       << "}";
    return os;
}

#endif //RAY_TRACER_MATERIAL_H
