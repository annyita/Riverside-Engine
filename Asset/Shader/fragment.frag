#version 460

layout (location = 0) in vec3 vertexColour;
layout (location = 0) out vec4 outColour;

layout(push_constant) uniform Push{
	float offset;
	vec3 position;
    mat4 cameraMatrix;
    mat4 projection;
	mat4 view;
} push;


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

float sdTorus( vec3 p, vec2 t ){
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

mat2 rot2D(float angle){
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

float scene(vec3 p){
   
   p = p - vec3(2.0, 2.0, 3.0);

   //p.xz *= rot2D(sin(push.offset) * 0.5);

   vec3 range = vec3(max(sin(push.offset) * 50.0, 5.0));

   vec3 modP = mod(p + range * 0.5, range ) - range * 0.5f;

   //modP.xy *= rot2D(1.0);
   modP.yz *= rot2D(sin(push.offset) * 3.0);

    return sdTorus(modP, vec2(0.5));
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

vec3 rayDirfromCam(mat4 projection, mat4 view, vec2 uv){
    vec4 target = projection * vec4(uv.x, uv.y, -1.0, 1.0);
    vec3 rayDirView = normalize(target.xyz / target.w);
    vec3 rayDirWorld = normalize(mat3(view) * rayDirView);

    return normalize(rayDirWorld);
}

void main(){
    vec2 uv = (vertexColour.rg * 2.0 - 1.0);
    uv.x *= 1920 / 1080;

    float sinoff =  sin(push.offset);

    vec3 ro = push.position;// + vec3(0.0 + push.offset, sinoff, -3.0 + push.offset - sinoff * 3.0);
    //vec3 rd = vec3(uv.rg, 1.0);
    vec3 rd = rayDirfromCam(push.projection, push.view, uv);
    //rd = 1.0 - rd;

    //float sinoff = 1.0 - sin(push.offset);

    vec3 sky = vec3(0.5, 0.4, 0.7);

    hitresult hr = raymarchScene(ro, rd, 80, 0.01, 128);

    if (!hr.isHit){
    	outColour = vec4(sky + 0.1, 1.0); return;
    }

    vec3 lightDirection = vec3(0.5f, -1.0f, 0.5f);
    float diffuse = max(dot(hr.normal, lightDirection), 0.0f);

    //vec3(1.0) +  // * vec3(1.0, sinoff * 0.5, 1.0)
    //vec3 final = vec3(push.colour) * mix(sky, vec3(1.0), diffuse);
    vec3 final = vec3(1.0) * mix(vec3(0.1), vec3(1.0), diffuse); // push.colour


	outColour = vec4(final, 1.0);
}