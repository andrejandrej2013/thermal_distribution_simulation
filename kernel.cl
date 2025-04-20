__kernel void updateTemp(__global float* temps, __global float* diffusion, int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= width || y >= height) return;

    int idx = y * width + x;
    float center = temps[idx];
    float sum = center;
    int count = 1;

    if (x > 0) { sum += temps[y * width + (x - 1)]; count++; }
    if (x < width - 1) { sum += temps[y * width + (x + 1)]; count++; }
    if (y > 0) { sum += temps[(y - 1) * width + x]; count++; }
    if (y < height - 1) { sum += temps[(y + 1) * width + x]; count++; }

    float avg = sum / count;
    temps[idx] += (avg - center) * diffusion[idx];
}

__kernel void temperatureToColor(__global const float* temperature,
                                 __global uchar* pixelBuffer,
                                 int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int index = y * width + x;

    float temp = temperature[index];
    float3 color;

    if (temp <= -20.0f) {
        color = (float3)(0.0f, 0.0f, 0.545f);
    } else if (temp >= 40.0f) {
        color = (float3)(1.0f, 0.0f, 0.0f);
    } else if (temp < 10.0f) {
        float r = (temp + 20.0f) / 30.0f;
        color = mix((float3)(0.0f, 0.0f, 0.545f), (float3)(0.0f, 1.0f, 1.0f), r);
    } else if (temp < 15.0f) {
        float r = (temp - 10.0f) / 5.0f;
        color = mix((float3)(0.0f, 1.0f, 1.0f), (float3)(0.678f, 1.0f, 0.184f), r);
    } else if (temp < 22.0f) {
        float r = (temp - 15.0f) / 7.0f;
        color = mix((float3)(0.678f, 1.0f, 0.184f), (float3)(0.0f, 1.0f, 0.0f), r);
    } else if (temp < 30.0f) {
        float r = (temp - 22.0f) / 8.0f;
        color = mix((float3)(0.0f, 1.0f, 0.0f), (float3)(1.0f, 0.647f, 0.0f), r);
    } else {
        float r = (temp - 30.0f) / 10.0f;
        color = mix((float3)(1.0f, 0.647f, 0.0f), (float3)(1.0f, 0.0f, 0.0f), r);
    }

    int outIndex = index * 4;
    pixelBuffer[outIndex + 0] = (uchar)(color.z * 255.0f); // blue
    pixelBuffer[outIndex + 1] = (uchar)(color.y * 255.0f); // green
    pixelBuffer[outIndex + 2] = (uchar)(color.x * 255.0f); // red
    pixelBuffer[outIndex + 3] = 255;
}
