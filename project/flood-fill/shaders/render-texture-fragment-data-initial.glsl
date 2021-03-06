uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec2 uDTime;
uniform vec3 uStartPosition;

void main(){
    uDTime;
    uPrevTexture;
    vec2 coord = gl_FragCoord.xy / 256.0;
    vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
    gl_FragColor = vec4(clamp(0.9 / (dot(coord - increasePosition, coord - increasePosition) + 0.75), 0.0, 1.5) * min(texture2D(uDataTexture, gl_FragCoord.xy / 256.0).r - 0.5/uSize.y, 1.0 - 1.0/uSize.y), 0.5, 0.0, 1.0);
}
