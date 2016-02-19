#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;
in layout(location = 2) vec2 vertex_texcoord;

out vec2 texture_coordinates;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;


void main() {
	gl_Position = MVP._proj * MVP._view * MVP._model * vec4( vertex_position, 1.0 );
    texture_coordinates = vertex_texcoord;
}
