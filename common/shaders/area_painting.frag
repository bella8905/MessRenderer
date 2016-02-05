#version 430

layout ( std140, binding = 1 ) uniform AreaBlock {
	uint _area;
}area;

out vec4 color;
const float max_area = 500000;


void main() {
    float brightness = clamp( float( area._area ) / max_area, 0.0, 1.0 );
	color = vec4( brightness, brightness, brightness, 1.0 );
}
