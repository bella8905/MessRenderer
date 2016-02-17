#version 430

layout ( binding = 1 ) uniform sampler2D s;
out vec4 fragColour;



// window size
layout ( std140, binding = 0 ) uniform VIEWPORT {
	float _width;
	float _height;
}viewport;


void main() {

	/////////////////////////////////////////////////////////////////
	//
	//  OpenGL expects 0 to be bottom and 1 to be top,
	//  adjust the y position so the texture starts from top
	//  
	/////////////////////////////////////////////////////////////////
	float adjustedY = gl_FragCoord.y - ( viewport._height - textureSize( s, 0 ).y );
    fragColour = texture( s, vec2( gl_FragCoord.x, adjustedY ) / textureSize( s, 0 ) );
}
