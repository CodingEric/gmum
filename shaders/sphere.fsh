#version 410 core
in vec2 oTexture;
uniform vec2 colorInfo;
uniform bool doGradient;
uniform sampler2D uTexture;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float remap(float t){
    t = (t - colorInfo.x / colorInfo.y ) / (1 - colorInfo.x / colorInfo.y);
    return 3*t*t*(1-t) + t*t*t;
}

void main(){
    if(doGradient) gl_FragColor = vec4(hsv2rgb(vec3((1.0-remap(texture2D(uTexture, oTexture).x))*0.67,1.0,1.0)),1.0);
    else gl_FragColor = texture2D(uTexture, oTexture);
}
