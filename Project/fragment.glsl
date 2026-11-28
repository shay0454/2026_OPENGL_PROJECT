#version 330 core
in vec3 vColor;
in vec2 vUV;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
};

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
	int type;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
};

uniform Material material;
uniform bool useTexture;


uniform vec3 objectColor;
uniform float opacity;

uniform vec3 viewPos;

const int MAX_LIGHTS = 16;
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

void main ()
{
	vec4 color;
	vec3 normalVector = normalize(Normal);
	if(useTexture)
		color = texture(material.texture_diffuse1,vUV);
	else
		color = vec4(vColor, 1.0);

	vec3 lighting = vec3(0.01);
	vec3 viewDir = normalize(viewPos - FragPos);

	int count = clamp(lightCount, 0, MAX_LIGHTS);
	for (int i = 0; i < count; ++i) {
    		vec3 toLight = lights[i].position - FragPos;
    		float lightDistance = length(toLight);
    		vec3 lightDir = toLight / max(lightDistance, 0.0001);

    		float attenuation = 1.0 / (1.0 + 0.14 * lightDistance + 0.07 * lightDistance * lightDistance);
		// 스포트라이트는 빛이 향하는 각도 안에서만 비춘다.
		if (lights[i].type == 1) {
			float spotCos = dot(-lightDir, normalize(lights[i].direction));
			attenuation *= smoothstep(lights[i].outerCutoff, lights[i].innerCutoff, spotCos);
		}

    	float diffuse = max(dot(normalVector, lightDir), 0.0);

    	float specular = 0.0;
    	if (diffuse > 0.0) {
        	vec3 reflectDir = reflect(-lightDir, normalVector);
       		specular = pow(max(dot(viewDir, reflectDir), 0.0), 128.0);
    	}
	
    	lighting += (0.7 * diffuse + 0.15 * specular)
        	      * lights[i].color
	              * lights[i].intensity
        	      * attenuation;
	}
	FragColor = vec4(lighting * color.rgb, color.a * opacity);
}
