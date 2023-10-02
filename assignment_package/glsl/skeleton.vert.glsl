#version 130
// ^ Change this to version 130 if you have compatibility issues

uniform mat4 u_Model;       //The matrix that defines the transformation of the
                            //object we're rendering. In this assignment,
                            //this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  //The inverse transpose of the model matrix.
                            //This allows us to transform the object's normals properly
                            //if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    //The matrix that defines the camera's transformation.
                            //We've written a static matrix for you to use for HW2,
                            //but in HW3 you'll have to generate one yourself

uniform mat4 u_TransMats[20]; // A uniform array of matrices (or dual quaternions

uniform mat4 u_BindMats[20]; // A uniform array of matrices that each describe the bind matrix of a particular joint.

in vec4 vs_Pos;             // The array of vertex positions passed to the shader
in vec4 vs_Nor;             // The array of vertex normals passed to the shader
in vec4 vs_Col;             // The array of vertex colors passed to the shader.

in vec2 vs_weights;
in ivec2 vs_jointIDs;

out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.


void main()
{
    fs_Col = vs_Col;

    mat4 bind1 = u_BindMats[vs_jointIDs[0]];
    mat4 bind2 = u_BindMats[vs_jointIDs[1]];

    mat4 currT1 = u_TransMats[vs_jointIDs[0]];
    mat4 currT2 = u_TransMats[vs_jointIDs[1]];

    vec4 joint1WorldPos = currT1 * bind1 * vs_Pos;
    vec4 joint2WorldPos = currT2 * bind2 * vs_Pos;

    vec4 finalPos = vs_weights[0] * joint1WorldPos
                  + vs_weights[1] * joint2WorldPos;

    //vec3 nor1 = normalize(transpose((mat3(currT1))) * mat3(bind1) * vs_Nor.xyz);
    //vec3 nor2 = normalize(transpose((mat3(currT2))) * mat3(bind2) * vs_Nor.xyz);

    //fs_Nor = mix(nor1, nor2, vs_weights[0]);

    vec4 modelposition = u_Model * finalPos;
    gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is used to render the final positions
                                             // of the geometry's vertices
}

