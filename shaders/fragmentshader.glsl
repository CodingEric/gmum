#version 410 core
in vec2 oTexture;
uniform sampler2D uTexture;
void main(){
    gl_FragColor = texture2D(uTexture, oTexture);
}
