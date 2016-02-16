#version 430

layout ( binding = 1 ) uniform sampler2D s;
out vec4 fragColour;

void main() {
    fragColour = texture( s, gl_FragCoord.xy / textureSize( s, 0 ) );
}
