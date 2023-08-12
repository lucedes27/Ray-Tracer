//
//
//

#ifndef RAY_TRACER_MATERIAL_H
#define RAY_TRACER_MATERIAL_H


class Material {
public:
    Vector3 ka; // Ambient
    Vector3 kd; // Diffuse
    Vector3 ks; // Specular
    float sp;   // Specular exponent
    Vector3 kr; // Reflection

    Material(const Vector3& ka, const Vector3& kd, const Vector3& ks, float sp, const Vector3& kr)
            : ka(ka), kd(kd), ks(ks), sp(sp), kr(kr) {}
};



#endif //RAY_TRACER_MATERIAL_H
