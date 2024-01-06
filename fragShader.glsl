#version 460 core

uniform vec3 iResolution;
uniform float iTime;

out vec4 fragColor;
//in vec2 fragCoord;
//layout (origin_upper_left) in vec4 gl_FragCoord;

vec3 rot3D(vec3 p, vec3 axis, float angle) {
  return ((min(min(dot(axis, p) * axis, p), cos(angle))) + cross(axis, p) * sin(angle));
}

mat2 rot2D(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mat2(c, -s, s, c);
}

float sdSphere(vec3 p, float s) {
  return length(p) - s;
}

float sdBox(vec3 p, vec3 b) {
  vec3 q = abs(p) - b;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdOctahedron(vec3 p, float s) {
  p = abs(p);
  return (p.x + p.y + p.z - s) * 0.57735027;
}

float map(vec3 p) {
//  p.z += iTime * 0.4;

//  p = fract(p) - .5;

  vec3 q = p;

  q.xy *= rot2D(iTime * 2.);
  q.yz *= rot2D(iTime);
  q.xz *= rot2D(iTime / 3.);

  float box = sdOctahedron(q, 0.5);
  return box;
}

void main() {

  vec2 uv = (gl_FragCoord.xy * 2. - iResolution.yz) / iResolution.z;
  float FOV = 0.7;

  vec3 rayOrigin = vec3(0, 0, -3);
  vec3 rayDirection = normalize(vec3(uv * FOV, 1));
  vec3 col = vec3(0);

  float t = 0.;

  int i;
  int iterCount = 100;
  for (i = 0; i < iterCount; i++) {
    vec3 pos = rayOrigin + rayDirection * t;
    float distance = map(pos);
    t += distance;
    col = vec3(i) / float(iterCount);

    if (distance < .001 || t > 100.) break;
  }
  
  col = vec3(t * 0.04 + float(i)/float(iterCount));

  fragColor = vec4(col, 1);
  
}
