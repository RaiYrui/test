#define GLFW_INCLUDE_NONE
#include <texture.h>
#include <shaderh.h>
#include <skybox.h>
#include <shlobj.h>
#include <obj.h>
#include <pbrRender.h>
#define GL_SILENCE_DEPRECATION
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
const double PI = 3.14159265358979f;
bool show_demo_window = false;
bool p_open = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
void rendergui(ImGuiIO &io);
PBR *pbr1;
GLFWwindow *window;
const int REFLACT = 0;
const int REFRACT = 1;
const int MIXTYPE = 2;
const int PHONG = 0;
const int BLINN = 1;
glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 ViewMatrix = glm::lookAt(
    glm::vec3(0, 0, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 0));
float camerax = 0;
float cameray = 0;
float cameraz = 7;
glm::vec3 oC = glm::vec3(0, 0, 0);
float radius = 20;
float horizontalAngle = PI;
float verticalAngle = -PI;
float initialFoV = 20.0f;
float mouseSpeed = 0.005f;
double scrollYoff = 0.0;
double gYoffset = 0;
int without = 0;
void renderQuad();
std::string readSomething(const char *type);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;
GLfloat lastX = 1024 / 2.0;
GLfloat lastY = 768 / 2.0;
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void movement();
OBJloader *o2;
bool pbrset = false;
PBRTexture *pt;
static void first()
{
    ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
int main()
{

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "R", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(stderr, "Failed to open GLFW window.  \n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    //     glEnable(GL_STENCIL_TEST);
    //   glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    // Shader s("../shaders/cube.vert", "../shaders/cube.frag");
    //  Shader skyshader("../shaders/skybox.vert", "../shaders/skybox.frag");
    //  Shader testshader("../shaders/ltest.vert", "../shaders/ltest.frag");
    //  Shader stencilshader("../shaders/simpletest.vert", "../shaders/simpletest.frag");
    // Shader ntest("../shaders/splight.vert", "../shaders/splight.frag", "../shaders/test.geo");
    // Shader sd("../shaders/simpleshadow.vert", "../shaders/simpleshadow.frag");
    Shader spl("../shaders/splighto.vert", "../shaders/splight.frag");
    Shader qs("../shaders/quad.vert", "../shaders/quad.frag");
    Shader ns("../shaders/normal.vert", "../shaders/normal.frag");
    Shader lc("../shaders/lightcube.vert", "../shaders/lightcube.frag");
    Shader *pl = new Shader("../shaders/plane.vert", "../shaders/plane.frag");
    Shader background("../shaders/background.vert", "../shaders/background.frag");

    // Shader bloom("../shaders/bloom.vert", "../shaders/bloom.frag");
    // Shader blur("../shaders/blur.vert", "../shaders/blur.frag");
    // Shader bloomf("../shaders/bloomf.vert", "../shaders/bloomf.frag");
    OBJloader o1("../3dmesh/cube.obj");
    o2 = new OBJloader("../3dmesh/ball.obj");
    OBJloader gun("../3dmesh/gun.obj");
    OBJloader plane("../3dmesh/plane.obj");
    OBJloader chest("../3dmesh/chest.obj");
    Skybox skybox;
    GLuint sVAO = skybox.Initskybox();
    GLuint cVAO = skybox.Initcube();
    Texture *nt = Loadtexture("../textures/chest-texture.bmp");
    // Texture *boxt = Loadtexture("../textures/sunset1.bmp");
    // Texture *gn = Loadtexture("../textures/ground-normal.bmp");
    Texture *wd = Loadtexture("../textures/patchy-meadow1_albedo.png");
    Texture *wdao = Loadtexture("../textures/patchy-meadow1_ao.png");
    Texture *wdr = Loadtexture("../textures/patchy-meadow1_roughness.png");
    Texture *wn = Loadtexture("../textures/patchy-meadow1_normal-ogl.png");
    Texture *wdm = Loadtexture("../textures/patchy-meadow1_metallic.png");
    Texture *mt = Loadtexture("../textures/gray-granite-flecks-albedo.png");
    Texture *mtr = Loadtexture("../textures/gray-granite-flecks-Roughness.png");
    Texture *mtao = Loadtexture("../textures/gray-granite-flecks-ao.png");
    Texture *mtm = Loadtexture("../textures/gray-granite-flecks-Metallic.png");
    Texture *mtn = Loadtexture("../textures/gray-granite-flecks-Normal-ogl.png");
    Texture *ga = Loadtexture("../textures/Cerberus_A.tga");
    Texture *gao = Loadtexture("../textures/Cerberus_AO.tga");
    Texture *gr = Loadtexture("../textures/Cerberus_R.tga");
    Texture *gm = Loadtexture("../textures/Cerberus_M.tga");
    Texture *gn = Loadtexture("../textures/Cerberus_N.tga");

    pbr1 = new PBR();
    pt = new PBRTexture();
    PBRTexture *groundpt = new PBRTexture();
    PBRTexture *guntexture = new PBRTexture();
    groundpt->albedo = wd;
    groundpt->ao = wdao;
    groundpt->metallic = wdm;
    groundpt->normal = wn;
    groundpt->roughness = wdr;
    pt->albedo = mt;
    pt->ao = mtao;
    pt->metallic = mtm;
    pt->normal = mtn;
    pt->roughness = mtr;
    guntexture->albedo = ga;
    guntexture->ao = gao;
    guntexture->metallic = gm;
    guntexture->normal = gn;
    guntexture->roughness = gr;
    // GLuint skytex = LoadSkytex();
    GLfloat deltaTime, lastFrame;
    GLuint lsec;
    GLuint framecount = 0;
    glm::vec3 Light = glm::vec3(0, 4, -1);
    pbr1->LoadHDR();
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0)
    {

        //-------------------------------------------------------------------
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        GLuint csec = (int)currentFrame;
        ++framecount;
        if (csec != lsec)
        {
            printf("%d\n", framecount);
            lsec = csec;
            framecount = 0;
        }

        glfwSetKeyCallback(window, key_callback);
        glfwSetScrollCallback(window, scroll_callback);
        //  glfwSetCursorPosCallback(window, mouse_callback);
        glClearColor(0.4f, 0.2f, 0.3f, 1.0f);
        movement();
        //  first();

        glm::mat4 model = glm::mat4(1.0);

        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * model;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Light.x += 0.01;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Light.x -= 0.01;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Light.y += 0.01;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Light.y -= 0.01;

        glm::mat4 model1 = model;
        model1[3][0] = Light.x;
        model1[3][1] = Light.y;
        model1[3][2] = Light.z;
        model1 = glm::scale(model1, glm::vec3(0.25f, 0.25f, 0.25f));
        glm::mat4 model2 = glm::translate(model, glm::vec3(0, 0, 0));
        glm::mat4 model3 = glm::translate(model2, glm::vec3(0, -2, 0));
        glm::mat4 model4 = glm::translate(model2, glm::vec3(1.5, 0, 1.5));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glm::vec3 Light1 = Light;
        glm::vec3 Light2 = Light1;
        Light2.y -= 2;
        Light2.x -= 1;
        glm::vec3 Light3 = Light1;
        Light3.y -= 2;
        Light3.x += 1;

        pl->use();

        pl->setvec3("lightpos[0]", Light1);
        pl->setvec3("lightpos[1]", Light2);
        pl->setvec3("lightpos[2]", Light3);
        pl->setvec3("lightcolor[0]", lightcolor);
        pl->setvec3("lightcolor[1]", lightcolor);
        pl->setvec3("lightcolor[2]", lightcolor);
        pl->setInt("flag", without);
        pl->setMat4("model", model3);
        pl->setMat4("view", ViewMatrix);
        pl->setMat4("projection", ProjectionMatrix);
        pl->setvec3("lightpos", Light);
        pl->setvec3("Campos", cameraPos);
        pbr1->BindTexture(groundpt, pl);

        plane.Draw(*pl);
        pl->setMat4("model", model2);
        pbr1->BindTexture(pt, pl);
        o2->Draw(*pl);
        pl->setMat4("model", model4);
        pbr1->BindTexture(guntexture, pl);
        gun.Draw(*pl);
        // spl.use();
        // spl.setMat4("model", model4);
        // spl.setMat4("view", ViewMatrix);
        // spl.setMat4("projection", ProjectionMatrix);
        // spl.setvec3("lightpos", Light);
        // spl.setvec3("Campos", cameraPos);
        // glActiveTexture(GL_TEXTURE0);
        // spl.setInt("diff", 0);
        // glBindTexture(GL_TEXTURE_2D, nt->id);
        // chest.Draw(spl);
        lc.use();
        lc.setMat4("model", model1);
        lc.setMat4("view", ViewMatrix);
        lc.setMat4("projection", ProjectionMatrix);
        o1.Draw(lc);
        glDepthFunc(GL_LEQUAL);
        background.use();
        background.setMat4("projection", ProjectionMatrix);
        background.setMat4("view", ViewMatrix);
        glBindVertexArray(sVAO);
        glActiveTexture(GL_TEXTURE0);
        background.setInt("Map", 0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, pbr1->envCubemap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glfwPollEvents();
        rendergui(io);

        // glViewport(0, 0, 512, 384);

        // qs.use();
        // glBindVertexArray(0);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // qs.setInt("depthMap", 0);
        // glBindVertexArray(qvao);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    // glDeleteProgram(testshader.ID);
    // glDeleteProgram(s.ID);
    // glDeleteProgram(skyshader.ID);
    return 0;
}
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        printf("P is pressed");
    }
    GLfloat cameraSpeed = 0.05f;
    if (key == GLFW_KEY_W)
        cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S)
        cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        without = !without;
}
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollYoff += yoffset;
    gYoffset = yoffset;
    radius -= yoffset;
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
static void movement()
{

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    // glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    horizontalAngle += mouseSpeed * float(1024 / 2 - 511);
    // verticalAngle += mouseSpeed * float(768 / 2 - ypos);
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - PI / 2.0f),
        0,
        cos(horizontalAngle - PI / 2.0f));
    glm::vec3 up = glm::cross(right, direction);
    glm::vec3 pos = oC + radius * direction;
    cameraPos = pos;
    ViewMatrix = glm::lookAt(
        pos,
        oC,
        -up);

    float FoV = initialFoV - scrollYoff;
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
}
void rendergui(ImGuiIO &io)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        static float f = 0.0f;
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Hello, world!", &show_demo_window, window_flags);
        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Option"))
            {
                if (ImGui::MenuItem("change the hdr"))
                {

                    std::string res = readSomething("HDR file(*.hdr)\0*.hdr\0");
                    const char *filePath = res.c_str();
                    printf(filePath);
                    if (filePath)
                    {
                        delete pbr1->hdr;
                        pbr1->hdr = LoadHDRimage(filePath);
                        pbr1->LoadHDR();
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Checkbox("Demo Window", &show_demo_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("Light Color", (float *)&clear_color);
        lightcolor.x = clear_color.x;
        lightcolor.y = clear_color.y;
        lightcolor.z = clear_color.z;
        if (ImGui::Button("LoadOBJ"))
        {

            std::string res = readSomething("OBJ file(*.obj)\0*.obj\0");
            const char *filePath = res.c_str();
            if (filePath)
                o2->Changeobj(filePath);
        }

        ImGui::Text("You can change the obj here");
        if (ImGui::Button("PBR"))
        {
            if (pbrset == false)
                pbrset = true;
        }
        ImGui::Text("change the pbr textures");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
        if (pbrset)
        {
            ImGui::SetNextWindowSize(ImVec2(150, 530), ImGuiCond_Once);
            ImGui::Begin("PBR setting");
            ImGui::Text("Albedo");

            if (ImGui::ImageButton("albedo", (ImTextureID)pt->albedo->id, ImVec2(64, 64)))
            {
                std::string temp = readSomething("PNG file(*.png)\0*.png\0");
                const char *filePath = temp.c_str();
                if (temp != "\0")
                {
                    pt->albedo = Loadtexture(filePath);
                }
            }
            ImGui::Text("Roughness");

            if (ImGui::ImageButton("roughness", (ImTextureID)pt->roughness->id, ImVec2(64, 64)))
            {
                std::string temp = readSomething("PNG file(*.png)\0*.png\0");
                const char *filePath = temp.c_str();
                if (temp != "\0")
                {
                    pt->roughness = Loadtexture(filePath);
                }
            }
            ImGui::Text("AO");

            if (ImGui::ImageButton("ao", (ImTextureID)pt->ao->id, ImVec2(64, 64)))
            {
                std::string temp = readSomething("PNG file(*.png)\0*.png\0");
                const char *filePath = temp.c_str();
                if (temp != "\0")
                {
                    pt->ao = Loadtexture(filePath);
                }
            }
            ImGui::Text("Metallic");

            if (ImGui::ImageButton("metallic", (ImTextureID)pt->metallic->id, ImVec2(64, 64)))
            {
                std::string temp = readSomething("PNG file(*.png)\0*.png\0");
                const char *filePath = temp.c_str();
                if (temp != "\0")
                {
                    pt->metallic = Loadtexture(filePath);
                }
            }
            ImGui::Text("Normal");

            if (ImGui::ImageButton("normal", (ImTextureID)pt->normal->id, ImVec2(64, 64)))
            {
                std::string temp = readSomething("PNG file(*.png)\0*.png\0");
                const char *filePath = temp.c_str();
                if (temp != "\0")
                {
                    pt->normal = Loadtexture(filePath);
                }
            }
            ImGui::Text(" ");
            ImGui::Text(" ");
            ImGui::SameLine(50.0f);
            if (ImGui::Button("Close"))
            {
                pbrset = false;
            }

            ImGui::End();
        }
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
std::string readSomething(const char *type)
{
    TCHAR szBuffer[MAX_PATH] = {0};
    OPENFILENAME file = {0};
    file.hwndOwner = NULL;
    file.lStructSize = sizeof(file);
    file.lpstrFilter = type;   // 要选择的文件后缀
    file.lpstrInitialDir = ""; // 默认的文件路径
    file.lpstrFile = szBuffer; // 存放文件的缓冲区
    file.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
    file.nFilterIndex = 0;
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER; // 标志如果是多选要加上OFN_ALLOWMULTISELECT
    BOOL bSel = GetOpenFileName(&file);
    if (!bSel)
    {
        std::cout << "failed to select the file" << std::endl;
        return "\0";
    }
    printf("file: %s", file.lpstrFile);
    std::string res = file.lpstrFile;
    return res;
}
// glDisable(GL_DEPTH_TEST);
// glBindTexture(GL_TEXTURE_2D, nt->id);
// glBegin(GL_QUADS);
// glTexCoord2f(0.0f, 0.0f);
// glVertex3f(-0.5f, -0.5f, -0.5f);
// glTexCoord2f(1.0f, 0.0f);
// glVertex3f(0.5f, -0.5f, -0.5f);
// glTexCoord2f(1.0f, 1.0f);
// glVertex3f(0.5f, 0.5f, -0.5f);
// glTexCoord2f(0.0f, 1.0f);
// glVertex3f(-0.5f, 0.5f, -0.5f);
// glEnd();