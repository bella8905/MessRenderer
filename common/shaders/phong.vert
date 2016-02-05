#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;

out VS_OUT{
    vec3 _position_eye;
    vec3 _normal_eye;
} vs_out;


void main() {
    vs_out._position_eye = vec3( MVP._view * MVP._model * vec4( vertex_position, 1.0 ) );
    // don't forget to normalize normals, we might resize the model by model matrix
    vs_out._normal_eye = normalize( vec3( MVP._view * MVP._model * vec4( vertex_normal, 0.0 ) ) );
	gl_Position = MVP._proj * MVP._view * MVP._model * vec4( vertex_position, 1.0 );
}
