#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;

out VS_OUT {
    vec3 _normal;
} vs_out;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;

uniform mat4 view, proj, model;

void main() {
	gl_Position = MVP._proj * MVP._view * MVP._model * vec4( vertex_position, 1.0 );
    vs_out._normal = vertex_normal;
}
