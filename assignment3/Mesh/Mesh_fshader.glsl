#version 330 core
out vec3 color;
in vec4 wpos;
in vec2 uv;

uniform vec3 light;
uniform sampler2D height_map;
uniform sampler2D grass_map;
uniform sampler2D snow_map;
uniform sampler2D rock_map;

void main() {

    float tiling_amount = 15;

    float height_center = texture(height_map, uv).r;

    float height_plus_u = textureOffset(height_map, uv, ivec2(1,0)).r;
    float height_plus_v = textureOffset(height_map, uv, ivec2(0,1)).r;
    vec2 pxs = 2.0/textureSize(height_map, 0);
    vec3 a = vec3(pxs.x,height_plus_u - height_center, 0);
    vec3 b = vec3(0, height_plus_v - height_center, pxs.y);

    vec3 N = normalize(cross(b,a)); // Calculate normal from height differences

    float normalAngle = acos(dot(vec3(0,1,0), N));

    vec3 light1 = normalize(vec3(1,3,0));

    vec3 ambient = vec3(0.1, 0.1, 0.2);

    float snowBlend = clamp((((height_center - 0.3)/0.5)+1)/2.0, 0, 1);

    vec3 diffuse = texture(rock_map, uv * tiling_amount).rgb * clamp(dot(N, light1), 0, 1) * (1.0-snowBlend)
             + texture(snow_map, uv * tiling_amount).rgb * clamp(dot(N, light1), 0, 1) * snowBlend/1.1;

    if (normalAngle < 1.0) { // if normal is < 20degrees to vertical, 0, show stone.
        diffuse = texture(grass_map, uv * tiling_amount).rgb * clamp(dot(N, light1), 0, 1) * (1.0-snowBlend)
                 + texture(snow_map, uv * tiling_amount).rgb * clamp(dot(N, light1), 0, 1) * snowBlend;
    }

	//vec3 specular = pow(dot(N, H), 100);

    //vec3 specular = vec3(0.0,0.0,0.0);
    // Optional TODO: add specular term
    // Hint: you will need the world space position of each pixel.
    // You can easily get this from the vertex shader where it is
    // already calculated

    color = ambient + diffuse;// + specular;

}
