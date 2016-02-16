#version 430

const vec4 vertices[] = vec4[]( vec4( -1.0, -1.0, -1.0, 1.0 ),
								vec4(  1.0, -1.0, -1.0, 1.0 ),
								vec4( -1.0,  1.0, -1.0, 1.0 ),
								vec4(  1.0,  1.0, -1.0, 1.0 ) 
								);
								

void main() {

	gl_Position = vertices[ gl_VertexID ];
}
