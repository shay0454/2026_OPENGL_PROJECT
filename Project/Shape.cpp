#include"Shape.h"
float floatToPixel(float pixel, float halfScreen) {
	return pixel / halfScreen;
}
glm::vec3 floatToPixel(glm::vec3 v) {
	v.x /= HALF_SCREEN_WIDTH;
	v.y /= HALF_SCREEN_HEIGHT;
	return v;
}