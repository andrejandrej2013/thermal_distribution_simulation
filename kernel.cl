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
