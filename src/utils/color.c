#include "color.h"
#include <math.h>

// https://contrastchecker.online/color-relative-luminance-calculator
double calculate_color_perceived_luminance(Color color)
{
  Vector4 normalizedColor = get_vec4_for_color(color);

  double R = normalizedColor.x <= 0.03928
    ? normalizedColor.x / 12.92
    : pow(((normalizedColor.x + 0.055)/1.055), 2.4);

  double G = normalizedColor.y <= 0.03928
    ? normalizedColor.y / 12.92
    : pow(((normalizedColor.y + 0.055)/1.055), 2.4);

  double B = normalizedColor.z <= 0.03928
    ? normalizedColor.z / 12.92
    : pow(((normalizedColor.z + 0.055)/1.055), 2.4);

  return 0.2126 * R + 0.7152 * G + 0.0722 * B;
}

Vector4 get_vec4_for_color(Color color)
{
  return (Vector4){
    .x = color.r / 255.0f,
    .y = color.g / 255.0f,
    .z = color.b / 255.0f,
    .w = color.a / 255.0f
  };
}
