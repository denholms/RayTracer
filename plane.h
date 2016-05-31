#ifndef PLANEH
#define PLANEH

#include "hitable.h"

class plane : public hitable  {
public:
	plane() {}
	plane(vec3 point0, vec3 point1, vec3 point2, material *mat) : v0(point0), v1(point1), v2(point2) {
		normal = cross(point1 - point2, point2 - point0);
		mat_ptr = mat;
	};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 v0;
	vec3 v1;
	vec3 v2;
	vec3 normal;
	material *mat_ptr;

};

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 norm = unit_vector(normal);
	float denom = dot(norm, r.direction());
	if (fabsf(denom) > 0.00001f){						//Ensures ray is not parallel to plane
		
		vec3 v = r.origin() - v0;						//Ray to some point on plane
		vec3 p0 = r.origin() - (dot(v, norm)*norm);		//Point on plane nearest original ray

		float t = dot((p0-r.origin()), norm) / denom;
		if (t < t_max && t > t_min){
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = norm;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

#endif


