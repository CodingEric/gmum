#version 410 core
in vec3 vPos;
in vec2 vTexture;
out vec2 oTexture;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool do_displacement;
uniform sampler2D uTexture;

void main(){
    oTexture = vTexture;
    if(!do_displacement){
        gl_Position = projection * view * model * vec4(vPos, 1.0);
        return;
    }
    gl_Position = projection * view * model * vec4(vPos * texture(uTexture, oTexture).r, 1.0);
}
