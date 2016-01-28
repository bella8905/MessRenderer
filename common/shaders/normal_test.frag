#version 400

in VS_OUT{
    vec3 _normal;
} fs_in;

out vec4 fragColour;

void main() {
    fragColour = vec4( fs_in._normal, 1.0 );
}
