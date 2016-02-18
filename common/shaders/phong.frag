#version 430

in VS_OUT{
    vec3 _position_eye;
    vec3 _normal_eye;
} fs_in;

layout ( std140, binding = 0 ) uniform MVPBlock {
	mat4 _view;
	mat4 _proj;
	mat4 _model;
}MVP;

// lighting
layout ( std140, binding = 2 ) uniform PhongBlock {
	vec3 _light_pos_world;
	vec3 _ls, _ld, _la;
}LightingData;

// material
layout ( std140, binding = 3 ) uniform MaterialBlock {
    float _spl_exp;
	vec3 _ks, _kd, _ka;
}MaterialData;




out vec4 frag_color;

void main() {
	// fragColour = inputColor;
    vec3 Ia = LightingData._la * MaterialData._ka;

    // diffusive reflectance
    vec3 light_pos_eye = vec3( MVP._view * vec4( LightingData._light_pos_world, 1.0 ) );
    vec3 dir_surface_to_light_eye = normalize( light_pos_eye - fs_in._position_eye );
    float dot_prod_diff = max( dot( dir_surface_to_light_eye, fs_in._normal_eye ), 0.0 );
    vec3 Id = LightingData._ld * MaterialData._kd * dot_prod_diff;

    // specular reflectance
    // blinn
    vec3 dir_surface_to_viewer_eye = normalize( - fs_in._position_eye );
    vec3 halfway_eye = normalize( dir_surface_to_viewer_eye + dir_surface_to_light_eye );
    float dot_prod_spl =  max( dot( halfway_eye, fs_in._normal_eye ), 0.0 );
    float spl_factor = pow( dot_prod_spl, MaterialData._spl_exp );

    vec3 Is = LightingData._ls * MaterialData._ks * spl_factor;

    frag_color = vec4( Is + Id + Ia, 1.0 );
}
