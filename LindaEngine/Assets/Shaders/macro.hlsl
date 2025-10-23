#if defined(NOT_UV)
	float c = 0.0;
    #ifdef VERTEX_COLOR
        c = 1.0;
    #endif
#else
    #if defined(NORMAL_MAP) || defined(SNOW) || defined(Rain)
        #if defined(NORMAL_MAP)
            vec3 normalTex =  texture(normalMapTex,uv).xyz;
        #else
            vec3 normalTex =  vec3(0.5,0.5,1.0);
        #endif
        normalWorld = GetNormalFromColor(normalTex, bumpPower);
    #else
        normalWorld = normalize(normalWorld);
    #endif
#endif

#ifdef PBR_MG_SB
	float a = 0.0;
#else
	float a = 1.0;
#endif