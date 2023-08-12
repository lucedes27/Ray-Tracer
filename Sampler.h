//
//
//

#ifndef RAY_TRACER_SAMPLER_H
#define RAY_TRACER_SAMPLER_H


class Sampler {
public:
    Vector3 getSample(int x, int y) const {
        return Vector3(x + 0.5f, y + 0.5f, 0);
    }
};



#endif //RAY_TRACER_SAMPLER_H
