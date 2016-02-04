#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;

out VS_OUT {
    vec3 _normal;
} vs_out;

uniform mat4 view, proj, model;

void main() {
	gl_Position = proj * view * model * vec4( vertex_position, 1.0 );
    vs_out._normal = vertex_normal;
}
