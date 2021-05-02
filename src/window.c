#include "window.h"
#include "GLFW/glfw3.h"
#include "ivec2s.h"
#include "state.h"
#include "util.h"

static void err_fn(int code, const char *desc) {
    panic("[GLFW] error#%d: %s", code, desc);
}

// updates the window size
static void size_fn(GLFWwindow *handle, int width, int height) {
    (void)handle;

    glViewport(0, 0, width, height);
    state.window.size = (ivec2s){{width, height}};
}

// updates the keyboard state
static void kbd_fn(GLFWwindow *handle, int key, int scancode, int action,
                   int mod) {
    (void)handle;
    (void)scancode;
    (void)mod;

    if (key < 0) {
        return;
    }

    if (action == GLFW_PRESS) {
        state.window.kbd[key].down = true;
    } else if (action == GLFW_RELEASE) {
        state.window.kbd[key].down = false;
    }
}

void window_create(WinFn init, WinFn destroy, WinFn update, WinFn render) {
    state.window.init = init;
    state.window.destroy = destroy;
    state.window.update = update;
    state.window.render = render;

    glfwSetErrorCallback(err_fn);

    if (!glfwInit()) {
        panic("error initializing GLFW");
    }

    state.window.size = (ivec2s){{640, 480}};

    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    state.window.handle = glfwCreateWindow(
        state.window.size.x, state.window.size.y, "Pong", NULL, NULL);

    if (!state.window.handle) {
        glfwTerminate();
        panic("error creating window");
    }

    glfwMakeContextCurrent(state.window.handle);
    // Load OpenGL extensions and stuff
    gladLoadGL(glfwGetProcAddress);

    // configure GLFW's callbacks
    glfwSetFramebufferSizeCallback(state.window.handle, size_fn);
    glfwSetKeyCallback(state.window.handle, kbd_fn);
}
