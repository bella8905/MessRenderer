#version 430

in vec3 vertex_position;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;

const uint maxPointSize = 50;
const uint minPointSize = 10;
const float a = 0;
const float b = 1;
const float c = 1;

void main() {
	gl_Position = MVP._proj * MVP._view * MVP._model * vec4( vertex_position, 1.0 );
	float distToEye = length( MVP._view * MVP._model * vec4( vertex_position, 1.0 ) );	
	// gl_PointSize = clamp( sqrt( 100.0 / ( a + b * distToEye + c * distToEye * distToEye ) ), minPointSize, maxPointSize );
    // gl_PointSize = 100;
}
