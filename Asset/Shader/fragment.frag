#version 460

layout (location = 0) in vec3 vertexColour;
layout (location = 0) out vec4 outColour;

struct hitresult{
    vec3 normal;
    vec3 hitpos;
    float dist;
    float totalDistance;
    float iterationsDBG;
    bool isHit;
};

float sdSphere( vec3 p, float r ){
  return length(p) - r;
}

float scene(vec3 p){
   
   p = p - vec3(2.0, 2.0, 3.0);

   vec3 modP = mod(p + vec3(3.0) * 0.5, vec3(3.0) ) - vec3(3.0) * 0.5f;

    return sdSphere(modP, 1.0);
    //return 1.0f;
}

// https://iquilezles.org/articles/normalsSDF/
vec3 CalculateNormal( in vec3 p  ){
    const vec2 k = vec2(1.0,-1.0);
    float eps = 0.01;
    return normalize(
            k.xyy*scene( p + k.xyy * eps).x +
            k.yyx* scene( p + k.yyx * eps).x +
            k.yxy* scene( p + k.yxy * eps).x +
            k.xxx* scene( p + k.xxx * eps).x );
}

hitresult raymarchScene(vec3 ro, vec3 rd, float maxdist, float mindist, int steps){
    hitresult hr;
    hr.isHit = false;
    //hr.lowestDistance = INF;
    float t = 0.0; // total distance travelled
    // raymarching
    for (int i = 0; i < steps; i++){
        vec3 pos = ro + rd* t;// position along the ray

        float dist = scene(pos);
        
        t += dist;
        //if (hr.lowestDistance > dist) hr.lowestDistance = dist;

        if (dist < mindist){ // treat as if hit
            hr.isHit = true;
            hr.dist = dist;
            hr.totalDistance = t;
            hr.iterationsDBG = float(i) / float(steps);
            hr.normal = CalculateNormal(pos);
            hr.hitpos = pos;
            //if (hr.colour.a < 0.5) continue;
            break;// how small dist (radius around march)
        }
        if (dist > maxdist) break; // failed to hit
        //discard; // discard on far for transparency
        // how large dist (radius around march)
    }

    return hr;
}

void main(){
    vec2 uv = (vertexColour.rg * 2.0 - 1.0);
    uv.x *= 1920 / 1080;

    vec3 ro = vec3(0.0, 0.0, -3.0);
    vec3 rd = vec3(uv.rg, 1.0);

    hitresult hr = raymarchScene(ro, rd, 80, 0.01, 80);

	//outColour = vec4(1.0, 1.0, 0.0, 1.0);
	outColour = vec4(hr.normal, 1.0);
}