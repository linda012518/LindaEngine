
vec3 aPosition = vec3(0.0);
vec3 aNormal = vec3(0.0);
vec3 aTangent = vec3(0.0);

#ifdef _Skin_Vertex_
    vec4 skinnedPos = vec4(0.0);
    vec3 skinnedNormal = vec3(0.0);
    vec3 skinnedTangent = vec3(0.0);

    for (int i = 0; i < LINDA_MAX_BONE_INFLUENCE; i++)
    {
        int boneIndex = int(round(aBoneID1[i]));

        if(boneIndex == -1) 
            continue;
        if(boneIndex >= LINDA_MAX_BONES) 
        {
            skinnedPos = vec4(aPosition_, 1.0f);
            break;
        }

        float weight = aBoneWeights1[i];
        mat4 boneMatrix = linda_BonesMatrices[boneIndex];

    #ifdef _Vertex_Input_Normal_
        skinnedNormal += weight * aNormal_ * mat3(linda_WorldToLocal);
    #endif
    #ifdef _Vertex_Input_Tangent_
        skinnedTangent += weight * mat3(linda_LocalToWorld) * aTangent_;
    #endif
        skinnedPos += boneMatrix * vec4(aPosition_, 1.0f) * weight;
    }

    aPosition = skinnedPos.xyz;
    aNormal = skinnedNormal;
    aTangent = skinnedTangent;
#else
    #ifdef _Vertex_Input_Position_
        aPosition = aPosition_;
    #endif

    #ifdef _Vertex_Input_Normal_
        aNormal = aNormal_;
    #endif

    #ifdef _Vertex_Input_Tangent_
        aTangent = aTangent_;
    #endif
#endif