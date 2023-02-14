
#include "CommonHeaders.h"
#include "GlslProgram.h"
#include "Colors.h"
#include "Utility.h"
#include "GLUtility.h"

using namespace GLUtility;
#pragma region vars

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 768;
const float viewAspectRatio = static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT);

struct TextureInfo
{
    std::string path;
    GLuint texture;
    glm::vec2 dim;
    glm::vec2 aspectScale = glm::vec2(1, 1);
};

TextureInfo textureInfo;

GLFWwindow* window;
auto closeWindow = false;
auto renderFbo1 = true;
auto renderFbo2 = true;

std::shared_ptr<Mesh> fsQuad;
std::shared_ptr<GlslProgram> aspectProgram,blendProgram;
std::shared_ptr<FrameBuffer> fbo1, fbo2;
std::vector<std::shared_ptr<Texture2D>> textures;
std::vector<TextureInfo> texInfos;

int pickedTex1 = 0;
int pickedTex2 = 1;

float bFactor = 0.5;

struct Framebuffer
{
    GLuint fbo;
    std::vector<GLuint> colorAttachments;
    GLuint depth;
    ~Framebuffer()
    {
        if (colorAttachments.size() > 0)
            glDeleteTextures((GLsizei)colorAttachments.size(), colorAttachments.data());

        colorAttachments.clear();

        if (depth > 0)
        {
            depth = 0;
            glDeleteRenderbuffers(1, &depth);
        }
    }
};

const char* images[] = { "img/landscape.jpg","img/portrait.jpg","img/JohnGreen7.png", "img/Lenna.png","img/1_webp_ll.webp"};
const std::vector<std::string> sampleTextures{"img/landscape.jpg","img/portrait.jpg","img/JohnGreen7.png", "img/Lenna.png","img/1_webp_ll.webp"};

GLuint normalColor;
GLuint textureColor;
#pragma endregion 


#pragma region prototypes

void createWindow();
void initGL();
void cleanGL();
void initImgui();
void setupCamera();
void setupScene();
std::shared_ptr<FrameBuffer> getFboMSA(std::shared_ptr<FrameBuffer> refFbo, int samples);
void updateFrame();
void renderFrame();
void renderImgui();


#pragma endregion


#pragma region functions

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {

    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {

    }
    else if (key == GLFW_KEY_UP /*&& action == GLFW_PRESS*/)
    {
    }
    else if (key == GLFW_KEY_DOWN /*&& action == GLFW_PRESS*/)
    {
    }
    else if (key == GLFW_KEY_Z /*&& action == GLFW_PRESS*/)
    {
    }
    else if (key == GLFW_KEY_X /*&& action == GLFW_PRESS*/)
    {
    }
    else if (key == GLFW_KEY_Q /*&& action == GLFW_PRESS*/)
    {
    }
    else if (key == GLFW_KEY_E /*&& action == GLFW_PRESS*/)
    {
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        
    }
}

void calcAspect(float viewAspect, TextureInfo& texInfo) {
    float imgAspect = (float)texInfo.dim.x / (float)texInfo.dim.y;;
    texInfo.aspectScale = glm::vec2(1);
    if (imgAspect > viewAspect) {
        texInfo.aspectScale.y = viewAspect / imgAspect;
    }
    else {
        texInfo.aspectScale.x = imgAspect / viewAspect;
    }
};

void updateTextureInfo(std::shared_ptr<Texture2D> tex,std::string path)
{
    textureInfo.dim = glm::vec2(tex->width, tex->height);
    textureInfo.texture = tex->texture;
    textureInfo.path = path;
    calcAspect(viewAspectRatio, textureInfo);
}
void updateTextureInfo(std::shared_ptr<Texture2D> tex,TextureInfo &texInfo, std::string path)
{
    texInfo.dim = glm::vec2(tex->width, tex->height);
    texInfo.texture = tex->texture;
    texInfo.path = path;
    calcAspect(viewAspectRatio, texInfo);
}

void createWindow()
{
    if (GLFW_TRUE != glfwInit())
    {
        cout << "Failed to init glfw" << '\n';
    }
    else
        cout << "glfw init success" << '\n';

    glfwSetErrorCallback(error_callback);


    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "GLSL Demos", NULL, NULL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwShowWindow(window);

    auto ginit = glewInit();
    if (ginit != GLEW_OK)
    {
        cout << "Failed to init glew" << '\n';
        return;
    }
    else
    {
        auto glVersionBytes = (char*)glGetString(GL_VERSION);
        auto glVendorBytes = (char*)glGetString(GL_VENDOR);
        string glversion((char*)glVersionBytes);
        string glvendor((char*)glVendorBytes);

        cout << "glew init success" << '\n';
        cout << glversion << '\n';
        cout << glvendor << cendl;
        cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << cendl;
    }

}

void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    aspectProgram = std::make_shared<GlslProgram>(Utility::readFileContents("shaders/vAspect.glsl"), Utility::readFileContents("shaders/fTexture.glsl"));
    blendProgram = std::make_shared<GlslProgram>(Utility::readFileContents("shaders/vAspect.glsl"), Utility::readFileContents("shaders/fBlend.glsl"));

    normalColor = glGetSubroutineIndex(aspectProgram->programID, GL_FRAGMENT_SHADER, "getColor");
    textureColor = glGetSubroutineIndex(aspectProgram->programID, GL_FRAGMENT_SHADER, "getTextureColor");

    setupScene();
    setupCamera();

    for (const auto& path : sampleTextures)
    {
        auto tex = GLUtility::makeTextureObject(path);
        textures.push_back(tex);
        TextureInfo tInfo;
        updateTextureInfo(tex, tInfo, path);
        texInfos.push_back(tInfo);
    }
    
    fbo1 = GLUtility::makeFbo(WIN_WIDTH, WIN_HEIGHT, 0);
    fbo2 = GLUtility::makeFbo(WIN_WIDTH, WIN_HEIGHT, 0);

}

void cleanGL()
{
}

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    //"#version 130"
}

void setupCamera()
{
   
}

void setupScene()
{
    fsQuad = GLUtility::getfsQuad();
}

void renderToFbo(std::shared_ptr<GLUtility::FrameBuffer> fboContainer, TextureInfo texInfo)
{
    glClearColor(0, 0, 0, 1.0);
    glViewport(0, 0, fboContainer->width, fboContainer->height);
    glBindFramebuffer(GL_FRAMEBUFFER, fboContainer->fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    aspectProgram->bind();
    
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &textureColor);
    aspectProgram->setInt("sampler", 0);
    aspectProgram->setVec2f("scaleFactrs", texInfo.aspectScale);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texInfo.texture);
    aspectProgram->bindAllUniforms();

    fsQuad->draw();

    aspectProgram->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

std::shared_ptr<FrameBuffer> getFboMSA(std::shared_ptr<FrameBuffer> refFbo, int samples)
{
    auto layer = std::make_shared<FrameBuffer>();
    auto width = WIN_WIDTH;
    auto height = WIN_HEIGHT;
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); //bind both read/write to the target framebuffer


    GLuint color, depth = 0;
    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, color);
    {
        glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color, 0);

    if (refFbo == nullptr)
    {
        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    }
    else
    {
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refFbo->depth);
    }

    const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, transparentDrawBuffers);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    checkFrambufferStatus(fbo);

    layer->fbo = fbo;
    layer->color = color;
    layer->depth = depth;
    return layer;
}


void updateFrame()
{
    auto t = glfwGetTime() * 20;
    auto theta = (int)t % 360;
}

void renderFrame()
{
    if (renderFbo1)
    {
        renderFbo1 = false;
        renderToFbo(fbo1, texInfos.at(pickedTex1));
    }

    if (renderFbo2)
    {
        renderFbo2 = false;
        renderToFbo(fbo2, texInfos.at(pickedTex2));
    }
    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    blendProgram->bind();

    blendProgram->setInt("sampler1", 0);
    blendProgram->setInt("sampler2", 1);
    blendProgram->setFloat("blendFactor", bFactor);
    blendProgram->setVec2f("scaleFactrs", glm::vec2(1));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo1->color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo2->color);
    blendProgram->bindAllUniforms();
    fsQuad->draw();

    blendProgram->unbind();
}

void renderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Display image with aspect ratio");

        auto prevT1 = pickedTex1;
        auto prevT2 = pickedTex2;
        ImGui::Combo("comboSrc", &pickedTex1, images, IM_ARRAYSIZE(images));
        ImGui::Combo("comboDst", &pickedTex2, images, IM_ARRAYSIZE(images));
        if (prevT1 != pickedTex1)
            renderFbo1 = true;
        if (prevT2 != pickedTex2)
            renderFbo2 = true;

        ImGui::SliderFloat("Blend Factor", &bFactor, 0, 1);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}






#pragma endregion





int main()
{
    createWindow();

    initImgui();

    initGL();

    while (!glfwWindowShouldClose(window))
    {
        updateFrame();
        renderFrame();
        renderImgui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

