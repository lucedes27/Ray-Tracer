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

    Material() : kd(0, 0, 0), ks(0, 0, 0), shininess(0), emission(0, 0, 0) {}
    Material(const Vector3& kd, const Vector3& ks, float shininess, const Vector3& emission)
            : kd(kd), ks(ks), shininess(shininess), emission(emission) {}


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
};



#endif //RAY_TRACER_MATERIAL_H
