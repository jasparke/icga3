#include "icg_common.h"
#include "imshow.h"
#include "imgui/imgui.h"

#include <math.h>
#include <OpenGP/GL/Eigen.h>

#include "Mesh/mesh.h"
#include "Skybox/box.h"

using namespace OpenGP;

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
void doKey(GLFWwindow *window);
void mousemove(GLFWwindow *window, double x, double y);
void scrollwheel(GLFWwindow* window, double xoffset, double yoffset);
float degToRad(float deg);

int window_width = 1280;
int window_height = 1024;
float scRatio = (float) window_width / (float) window_height;
float mid_width = window_width/2.0f;
float mid_height = window_height/2.0f;

//time management
float deltaTime = 0.0f;
float lastTime = 0.0f;

//camera - should probably split this off but oh well
vec3 up(0.0f, 1.0f, 0.0f);
vec3 cam_pos(0.0f, 0.0f, 0.0f);
vec3 cam_up(0.0f, 1.0f, 0.0f);
vec3 cam_look(0.0f, 0.0f, -1.0f);
vec3 cam_right = cam_look.cross(up);
float FoV = 45.0f;
float mouseSpeed = 0.1f;
float speed = 0.1f;
float accel = 0.01f;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = mid_width;
float lastY = mid_height;
bool firstOnScreen = true;

//renderers
Mesh grid;
Box sbox;

void display() {

    //update time
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    doKey(OpenGP::window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Transform modelTransform = Transform::Identity();
    mat4 model = modelTransform.matrix();
    mat4 projection = OpenGP::perspective(FoV, scRatio, 0.1f, 100.0f);
    vec3 lookdir = cam_pos + cam_look;
    mat4 view = OpenGP::lookAt(cam_pos, lookdir, cam_up);
    grid.draw(model, view, projection);
    sbox.draw(view, projection);
}

int main(int, char**) {

    OpenGP::glfwInitWindowSize(window_width, window_height);
    OpenGP::glfwMakeWindow("Assignment 3");

    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    grid.init();
    sbox.init();
    OpenGP::glfwDisplayFunc(&display);
    glfwSetCursorPosCallback(window, mousemove);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    OpenGP::glfwMainLoop();

    return EXIT_SUCCESS;
}

void updateCamera() {
    cam_look = vec3(cos(degToRad(yaw)) * cos(degToRad(pitch)), sin(degToRad(pitch)), sin(degToRad(yaw)) * cos(degToRad(pitch))).normalized();
    cam_right = cam_look.cross(up).normalized();
    cam_up = cam_right.cross(cam_look).normalized();
}

void mousemove(GLFWwindow *window, double x, double y) {
    if (firstOnScreen) {
        lastX = x;
        lastY = y;
        firstOnScreen = false;
    }

    float xoff = (x - lastX)*mouseSpeed;
    float yoff = (lastY - y)*mouseSpeed;

    yaw += xoff;
    pitch += yoff;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCamera();

    lastX = x;
    lastY = y;


}

float degToRad(float deg) {
    return deg * M_PI / 180.0;
}

void doKey(GLFWwindow *window) {

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            FoV -= 1.0f;
            if (FoV <= 1.0f) FoV = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            FoV += 1.0f;
            if (FoV >= 80.0f) FoV = 80.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            speed += accel;
            if (speed >= 1.0f) speed = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            speed -= accel;
            if (speed <= 0.01f) speed = 0.01f;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            float velo = speed * deltaTime;
            cam_pos += cam_look * velo;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            float velo = speed * deltaTime;
            cam_pos -= cam_look * velo;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            float velo = speed * deltaTime;
            cam_pos -= cam_right * velo;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            float velo = speed * deltaTime;
            cam_pos += cam_right * velo;
        }
}
