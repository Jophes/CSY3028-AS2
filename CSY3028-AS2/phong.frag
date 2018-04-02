#version 440   

struct LightInfo {
    vec3 Position;
    vec3 La;
    vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform mat4 View;

out vec4 FragColor;

uniform sampler2D Tex;

void main() {
	if (gl_FrontFacing) {
		vec3 n = normalize(Normal), s = normalize(vec3(Light.Position - Position));
		vec3 v = normalize(vec3(-Position)), r = reflect(-s, n );
		float sDotN = max( dot( s, n ), 0.0 );
 
		vec3 ambient = Light.La * Material.Ka;
		vec3 diffuse = Light.Ld * Material.Kd * sDotN;
		vec3 spec = Light.Ls * Material.Ks * pow( max( dot(r,v) , 0.0 ), Material.Shininess ); 
		vec4 texColour = texture(Tex, TexCoord);

		FragColor = vec4(ambient + diffuse, 1) * texColour + vec4(spec, 1); // Illuminated textures
		//FragColor = vec4(ambient + diffuse, 1)  + vec4(spec, 1); // Illuminated
		//FragColor = vec4(1, 1, 1, 1); // Colour
	}
}