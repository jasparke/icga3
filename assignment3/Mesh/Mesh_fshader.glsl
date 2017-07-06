#version 330 core
out vec3 color;
in vec4 wpos;
in vec2 uv;

uniform sampler2D height_map;
uniform sampler2D diffuse_map;

void main() {

    float tiling_amount = 5;

    float height_center = texture(height_map, uv).r;

    float height_plus_u = textureOffset(height_map, uv, ivec2(1,0)).r;
    float height_plus_v = textureOffset(height_map, uv, ivec2(0,1)).r;
    vec2 pxs = 2.0/textureSize(height_map, 0);
    vec3 a = vec3(pxs.x,height_plus_u - height_center, 0);
    vec3 b = vec3(0, height_plus_v - height_center, pxs.y);

    vec3 N = normalize(cross(b,a)); // Calculate normal from height differences

    vec3 light = normalize(vec3(1,3,0));

    vec3 ambient = vec3(0.1, 0.1, 0.2);
    vec3 diffuse = texture(diffuse_map, uv * tiling_amount).rgb * clamp(dot(N, light), 0, 1);


    //vec3 specular = vec3(0.0,0.0,0.0);
    // Optional TODO: add specular term
    // Hint: you will need the world space position of each pixel.
    // You can easily get this from the vertex shader where it is
    // already calculated

    color = ambient + diffuse;// + specular;

}
