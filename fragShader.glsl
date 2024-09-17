#version 460 core

uniform vec3 iResolution;
uniform float iTime;
uniform vec3 iPosition;
uniform vec3 iRotation;

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

float mandelbulb(vec3 p) { // http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
  vec3 z = p;
  float dr = 1.0;
  float r = 0.0;
  float bail = 10;
  float power = 9;
  float iterations = 20;

  for (int i = 0; i < iterations; i++) {
    r = length(z);
    if (r > bail) break;

    float theta = acos(z.z / r);
    float phi = atan(z.y, z.x);
    dr = pow(r, power - 1.0) * power * dr + 1.0;

    float zr = pow(r, power);
    theta *= power;
    phi *= power;

    z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
    z += p;

  }
  return 0.5*log(r) * r / dr;
}

float fractal(vec3 z) {
  float r;
  int n = 0;
  float scale = 2;
  float offset = 1;
  int iterations = 10;
  while (n < iterations) {
    if (z.x + z.y < 0) z.xy = -z.xy;
    if (z.x + z.z < 0) z.xz = -z.xz;
    if (z.y + z.z < 0) z.zy = -z.zy;
    z.xy *= rot2D(0.4);
    z.zy *= rot2D(0.1);
    z.zx *= rot2D(0.2);
    z = z * scale - offset * (scale - 1.0);
    n++;
  }
  return length(z) * pow(scale, -float(n));
}

float map(vec3 p) {

  p.xz = fract(p.xz/2) - 0.5;
  //p.z += 2.08;
  //p.x += 0.2;
  //p.y -= 0.5;

  //p.xy *= rot2D(iTime*0.2);
  //p.zy *= rot2D(iTime*0.1);
  //p.xz *= rot2D(iTime*0.3);

  float ground = p.y;
  float box = sdBox(vec3(p.x, p.y-0.2, p.z), vec3(0.2));
  return min(box, ground);
}

float softShadow(vec3 ro, vec3 rd, float mint, float maxt, float w) {
  float res = 1.0;
  float t = mint;
  for (int i = 0; i < 256 && t < maxt; i++) {
    float h = map(ro + t*rd);
    res = min(res, h/(w*t));
    t += clamp(h, 0.005, 0.5);
    if (res < -1.0 || t > maxt) break;
  }
  res = max(res, -1.0);
  return 0.25 * (1.0 + res) * (1.0 + res) * (2.0 - res);
}

void main() {

  vec2 uv = (gl_FragCoord.xy * 2. - iResolution.yz) / iResolution.z;
  float FOV = 0.707106;

  vec3 rayOrigin = vec3(iPosition.x, iPosition.y, iPosition.z);
  
  vec3 rayRotComponent = vec3(uv * FOV, 1);
  rayRotComponent.xy *= rot2D(iRotation.x);
  rayRotComponent.yz *= rot2D(iRotation.y);
  rayRotComponent.zx *= rot2D(iRotation.z);
  vec3 rayDirection = normalize(rayRotComponent);
  vec3 col = vec3(0);

  // Light
  vec3 lightSource = vec3(3, 3, 3);
  vec3 lightDirection = vec3(0, 0, 0);

  float t = 0.;

  int i;
  int iterCount = 512;
  for (i = 0; i < iterCount; i++) {
    vec3 pos = rayOrigin + rayDirection * t;
    float distance = map(pos);
    t += distance;
    //col = vec3(i) / float(iterCount);

    if (distance < .0001 || t > 100.) break;
  }
  
//  float comp = t * 0.01 + float(i)*30/float(iterCount);
  col = vec3(1 - t*0.05 - float(i)/float(iterCount));

  fragColor = vec4(col, 1.0);
  
}
