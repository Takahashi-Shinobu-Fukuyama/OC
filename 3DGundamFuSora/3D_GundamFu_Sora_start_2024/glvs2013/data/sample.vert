uniform vec3 lightVec;
uniform vec4 baseColor;
uniform vec3 testVec;
void main(void)
{
    vec3 normal = normalize(gl_NormalMatrix *  gl_Normal);
    
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    float t = dot(normal,lightVec);
    gl_FrontColor = baseColor * vec4( max(0.0,t) );
        
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
