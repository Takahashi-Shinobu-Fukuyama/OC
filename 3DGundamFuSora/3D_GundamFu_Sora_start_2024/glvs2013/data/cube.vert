uniform vec3 lightVec;
uniform vec4 baseColor;
attribute float boneIndex;
uniform mat4 boneMatrix[2];
void main(void)
{

    mat4 mat = boneMatrix[int(boneIndex)];
    
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * mat * gl_Vertex;
	
	
    
    vec3 normal = normalize(gl_NormalMatrix *  gl_Normal);
    float t = dot(normal,lightVec);
    gl_FrontColor = baseColor * vec4( max(0.0,t) );
        
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
