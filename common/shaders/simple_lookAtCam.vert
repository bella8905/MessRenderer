#version 430

in vec3 vertex_position;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;


void main() {
	gl_Position = MVP._proj * MVP._view * MVP._model * vec4( vertex_position, 1.0 );
    // gl_Position = vec4( vertex_position, 1.0 );
}
