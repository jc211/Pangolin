/* This file is part of the Pangolin Project.
 * http://github.com/stevenlovegrove/Pangolin
 *
 * Copyright (c) 2011-2018 Steven Lovegrove, Andrey Mnatsakanov
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <pangolin/factory/factory_registry.h>
#include <pangolin/platform.h>
#include <pangolin/gl/glinclude.h>
#include <pangolin/display/display.h>
#include <pangolin/display/display_internal.h>


#include <pangolin/display/device/GLFWWindow.h>
#include <memory>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>




namespace pangolin {
inline void _glfw_error_callback(int error, const char* description)
{
    pango_print_error("Error %i: %s", error, description);
}

static void _initialize_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

}

static bool _imgui_has_control() {
    return ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
}

}

namespace pangolin
{

const char *className = "Pangolin";

extern __thread PangolinGl* context;
  
////////////////////////////////////////////////////////////////////////
// Utils
////////////////////////////////////////////////////////////////////////

unsigned char GetPangoKey(int key, int scancode)
{
    switch (key)
    {
    case GLFW_KEY_F1: return PANGO_SPECIAL + PANGO_KEY_F1;
    case GLFW_KEY_F2: return PANGO_SPECIAL + PANGO_KEY_F2;
    case GLFW_KEY_F3: return PANGO_SPECIAL + PANGO_KEY_F3;
    case GLFW_KEY_F4: return PANGO_SPECIAL + PANGO_KEY_F4;
    case GLFW_KEY_F5: return PANGO_SPECIAL + PANGO_KEY_F5;
    case GLFW_KEY_F6: return PANGO_SPECIAL + PANGO_KEY_F6;
    case GLFW_KEY_F7: return PANGO_SPECIAL + PANGO_KEY_F7;
    case GLFW_KEY_F8: return PANGO_SPECIAL + PANGO_KEY_F8;
    case GLFW_KEY_F9: return PANGO_SPECIAL + PANGO_KEY_F9;
    case GLFW_KEY_F10: return PANGO_SPECIAL + PANGO_KEY_F10;
    case GLFW_KEY_F11: return PANGO_SPECIAL + PANGO_KEY_F11;
    case GLFW_KEY_F12: return PANGO_SPECIAL + PANGO_KEY_F12;
    case GLFW_KEY_LEFT: return PANGO_SPECIAL + PANGO_KEY_LEFT;
    case GLFW_KEY_UP: return PANGO_SPECIAL + PANGO_KEY_UP;
    case GLFW_KEY_RIGHT: return PANGO_SPECIAL + PANGO_KEY_RIGHT;
    case GLFW_KEY_DOWN: return PANGO_SPECIAL + PANGO_KEY_DOWN;
    case GLFW_KEY_HOME: return PANGO_SPECIAL + PANGO_KEY_HOME;
    case GLFW_KEY_END: return PANGO_SPECIAL + PANGO_KEY_END;
    case GLFW_KEY_INSERT: return PANGO_SPECIAL + PANGO_KEY_INSERT;
    case GLFW_KEY_DELETE: return 127;
    default:
        const char* res =  glfwGetKeyName(key, scancode);
        if(res)
            return res[0];
        else 
            return 0;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
// WinWindow Implementation
////////////////////////////////////////////////////////////////////////

GLFWWindow::GLFWWindow(
    const std::string& window_title, int width, int height
)
{
    glfwSetErrorCallback(pangolin::_glfw_error_callback);
    if (!glfwInit())
        throw std::runtime_error("Could not intialize window");
    m_window = glfwCreateWindow(width, height, window_title.c_str(), NULL, NULL);
    if (!m_window)
    {
        std::cerr << "Failed to create glfw window" << std::endl;
        glfwTerminate();
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);
    int winwidth, winheight;
    glfwGetWindowSize(m_window, &winwidth, &winheight);
    PangolinGl::windowed_size[0] = winwidth;
    PangolinGl::windowed_size[1] = winheight;
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        auto self = (GLFWWindow*)glfwGetWindowUserPointer(window);
        self->quit = true;
    });

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        //auto self = (GLFWWindow*)glfwGetWindowUserPointer(m_window);
        process::Resize(width, height);    
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        if (_imgui_has_control())
            return;

		auto self = (GLFWWindow*)glfwGetWindowUserPointer(window);

        unsigned char pangokey = GetPangoKey(key, scancode);
        if(pangokey>0) {
            if (action == GLFW_PRESS) {
             //   self->m_key_pressed = true;
               // process::Keyboard(key, 1, 1);
             //   ImGui::GetKey
            }
            else if (action == GLFW_RELEASE) {
              //  self->m_key_pressed = false;
             //   process::KeyboardUp(key, 1, 1);
            }
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
        if (_imgui_has_control())
            return;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: {
                process::Mouse(0 | mods, action == GLFW_PRESS ? 0 : 1, xpos, ypos);
                break;
            }
            case GLFW_MOUSE_BUTTON_MIDDLE: {
                process::Mouse(1 | mods, action == GLFW_PRESS ? 0 : 1, xpos, ypos);
                break;
            }
            case GLFW_MOUSE_BUTTON_RIGHT: {
                process::Mouse(2 | mods, action == GLFW_PRESS ? 0 : 1, xpos, ypos);
                break;
            }
            default: {
               break;
            }
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        if (_imgui_has_control())
            return;
        int mouse_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) + 
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) + 
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
        if (mouse_pressed) {
            process::MouseMotion(xpos, ypos);
        } else{
            process::PassiveMouseMotion(xpos, ypos);
        }
        
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        if (_imgui_has_control())
            return;
        process::Scroll(5*xoffset, 5*yoffset);
      });

    // Display Window
    PangolinGl::is_double_buffered = true;
    pangolin::_initialize_imgui(m_window);
}

GLFWWindow::~GLFWWindow()
{
    if (m_window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

void GLFWWindow::StartFullScreen() {
  
}

void GLFWWindow::StopFullScreen() {
   
}

void GLFWWindow::InitializeFrame() {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   // Keybord control
   auto& io = ImGui::GetIO();
   if (!_imgui_has_control()) {
       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { 
           if (ImGui::IsKeyDown(i)) {
                process::Keyboard(i, 1, 1);
           }
           else if (ImGui::IsKeyReleased(i)) {
               process::KeyboardUp(i, 1, 1);
           }
       }
   }
   
}

void GLFWWindow::ToggleFullscreen()
{
    if(!context->is_fullscreen) {
        StartFullScreen();
        context->is_fullscreen = true;
    }else{
        StopFullScreen();
        context->is_fullscreen = false;
    }
}

void GLFWWindow::Move(int x, int y)
{
    glfwSetWindowPos(m_window, x, y);
}

void GLFWWindow::Resize(unsigned int w, unsigned int h)
{
    glfwSetWindowSize(m_window, w, h);
}

void GLFWWindow::MakeCurrent()
{
    glfwMakeContextCurrent(m_window);
    context = this;
}

void GLFWWindow::RemoveCurrent()
{
    glfwMakeContextCurrent(nullptr);
}

void GLFWWindow::SwapBuffers()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    auto& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_window);
}

void GLFWWindow::ProcessEvents()
{
    glfwPollEvents();
}

std::unique_ptr<WindowInterface> CreateWinWindowAndBind(std::string window_title, int w, int h)
{
    GLFWWindow* win = new GLFWWindow(window_title, w, h);

    return std::unique_ptr<WindowInterface>(win);
}

PANGOLIN_REGISTER_FACTORY(GLFWWindow)
{
  struct GLFWWindowFactory : public FactoryInterface<WindowInterface> {
    std::unique_ptr<WindowInterface> Open(const Uri& uri) override {
          
      const std::string window_title = uri.Get<std::string>("window_title", "window");
      const int w = uri.Get<int>("w", 640);
      const int h = uri.Get<int>("h", 480);
      return std::unique_ptr<WindowInterface>(CreateWinWindowAndBind(window_title, w, h));
    }
  };
  
  auto factory = std::make_shared<GLFWWindowFactory>();
  FactoryRegistry<WindowInterface>::I().RegisterFactory(factory, 5, "default");

}

}
