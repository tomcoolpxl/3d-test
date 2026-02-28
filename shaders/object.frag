#version 450 core

in vec3 fragPos;
in vec3 vNormal;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightDir;    // normalized, points FROM light (toward scene)
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-lightDir);

    float diff = max(dot(N, L), 0.0);
    vec3 ambient  = ambientStrength * lightColor;
    vec3 diffuse  = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}
