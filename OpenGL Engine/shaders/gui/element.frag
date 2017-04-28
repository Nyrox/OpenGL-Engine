#version 420 core

out vec4 color;

uniform vec4 fillColor;

void main(){
    color = vec4(fillColor);
}