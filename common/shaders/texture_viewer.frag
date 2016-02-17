#version 430

layout ( binding = 1 ) uniform sampler2D s;
out vec4 fragColour;



// window size
layout ( std140, binding = 0 ) uniform VIEWPORT {
	float _width;
	float _height;
}viewport;

layout ( std140, binding = 1 ) uniform IMAGE {
	float _scale;
}image;


void main() {

	/////////////////////////////////////////////////////////////////
	//
	//  OpenGL expects 0 to be bottom and 1 to be top,
	//  adjust the y position so the texture starts from top
	//  
	/////////////////////////////////////////////////////////////////
	float adjustedY = ( gl_FragCoord.y - ( viewport._height - textureSize( s, 0 ).y * image._scale ) ) / image._scale;
	// float adjustedY = gl_FragCoord.y / image._scale;
	float adjustedX = gl_FragCoord.x / image._scale;
    fragColour = texture( s, vec2( adjustedX, adjustedY ) / textureSize( s, 0 ) );
}
