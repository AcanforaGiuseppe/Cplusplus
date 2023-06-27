#version 450 core
out vec4 frag_color;

in vec2 vert_uv_out;
//in vec3 vert_norm_out;
in vec3 world_pos_out;
in vec3 world_norm_out;

layout (binding = 0) uniform sampler2D tex;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;

void main()
{
    vec4 tex = texture(tex, vert_uv_out);
    //vec4 tex = vec4(abs(vert_norm_out), 1.f);
    //frag_color = tex;

    vec3 mesh_color = tex.xyz;

    // Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = mesh_color * ambient_factor;

    // Diffuse
    vec3 light_dir = normalize(point_light_pos - world_pos_out);

    vec3 norm_n = normalize(world_norm_out);
    float diff_str = max(dot(norm_n, light_dir), 0.f);
    vec3 diffuse = mesh_color * diff_str;

    // Specular
    vec3 eye_dir = normalize(camera_pos - world_pos_out);
    vec3 refl_dir = reflect(-light_dir, norm_n);
    float spec_str = max(dot(eye_dir, refl_dir), 0.f);
    float spec_fact = pow(spec_str, 40.f);
    vec3 specular = vec3(1.f, 1.f, 1.f) * spec_fact;

    vec3 phong = ambient + diffuse + specular;

    frag_color = vec4(phong, 1.f);

    /* 
        attenuation = 1.f / contribute 
            Kc = 1.f
            Kl * distance;
            Kd * distance * distance;

        contribute = Kc + Kl + Kq


        phong *= attenuation;
    */
}