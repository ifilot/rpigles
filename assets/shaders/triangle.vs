attribute vec4 vPosition;
attribute vec3 vColor;
varying vec3 frag_color;
uniform mat4 rotmat;

void main() {
    frag_color = vColor;
    gl_Position = rotmat * vPosition;
}
