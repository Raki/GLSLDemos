
#include "CommonHeaders.h"
#include "GlslProgram.h"
#include "Colors.h"
#include "Utility.h"
#include "GLUtility.h"

using namespace GLUtility;
#pragma region vars

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 1024;

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


GLFWwindow* window;
auto closeWindow = false;

std::shared_ptr<Mesh> fsQuad;
std::shared_ptr<GlslProgram> basicProgram;
std::shared_ptr<FrameBuffer> layer1;
std::shared_ptr<Texture2D> diffuseTex;

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
std::shared_ptr<Framebuffer> blitContainer;

std::map<std::string, std::shared_ptr<Texture2D>> texCache;
std::map < std::string,std::string> texPaths;
const char* tileImages[] = { "Classical Elegant","Colored Classical","Cark Elegant Classic","Elegant Classic Decor","Flat Design" };

GLuint colorTile,imgTile;
int tiles = 3;
int tileMode = 0;
int tileTexture = 0;

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


void createWindow()
{
    if (GLFW_TRUE != glfwInit())
    {
        cout << "Failed to init glfw" << '\n';
    }
    else
        cout << "glfw init success" << '\n';

    glfwSetErrorCallback(error_callback);


    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Step1", NULL, NULL);

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

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    basicProgram = std::make_shared<GlslProgram>(Utility::readFileContents("shaders/vBasic.glsl"), Utility::readFileContents("shaders/fTiles.glsl"));

    colorTile = glGetSubroutineIndex(basicProgram->programID, GL_FRAGMENT_SHADER, "getTileColor");
    imgTile = glGetSubroutineIndex(basicProgram->programID, GL_FRAGMENT_SHADER, "getTileColorTex");

    setupScene();
    setupCamera();

    layer1 = getFboMSA(nullptr, 4);

    auto diffTex = GLUtility::makeTexture("img/tiles/classical_elegant/sample.jpg");
    
    diffuseTex = std::make_shared<GLUtility::Texture2D>();
    diffuseTex->texture = diffTex;
    texCache["img/tiles/classical_elegant/sample.jpg"] = diffuseTex;
}

void cleanGL()
{
}

void initImgui()
{
    // Setup Dear ImGui context
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

void setupCamera() {

   
}

void setupScene()
{
   
    texPaths[tileImages[0]] = "img/tiles/classical_elegant/sample.jpg";
    texPaths[tileImages[1]] = "img/tiles/colored_classical/sample.jpg";
    texPaths[tileImages[2]] = "img/tiles/dark_elegant_classic/sample.jpg";
    texPaths[tileImages[3]] = "img/tiles/elegant_classic_decor/sample.jpg";
    texPaths[tileImages[4]] = "img/tiles/flat_design/sample.jpg";
    fsQuad = GLUtility::getfsQuad();

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

    glBindFramebuffer(GL_FRAMEBUFFER, layer1->fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
    basicProgram->bind();

    if(tileMode==0)
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorTile);
    else
    {
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &imgTile);
        basicProgram->setInt("sampler", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTex->texture);
    }

    basicProgram->setVec2f("resolution", glm::vec2(WIN_WIDTH,WIN_HEIGHT));
    basicProgram->setFloat("tiles", (float)tiles);
    basicProgram->bindAllUniforms();
    fsQuad->draw();

    basicProgram->unbind();


    //blitting
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, layer1->fbo);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void renderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Tiles Example");

        ImGui::InputInt("Tiles Resolution", &tiles, 1);

        const char* tileModes[] = { "Color Tile", "Img Tile" };
        
        //ImGui::BeginCombo("asdf", items[0]);
        
        ImGui::Combo("Tile Mode", &tileMode,&tileModes[0],2);
        
        if (tileMode == 1)
        {
            ImGui::Combo("Tile Texture", &tileTexture, &tileImages[0], 5);

            if(texCache.find(tileImages[tileTexture]) == texCache.end())
            {
                auto tex = GLUtility::makeTexture(texPaths[tileImages[tileTexture]]);
                auto tex2D = std::make_shared<Texture2D>();
                tex2D->texture = tex;
                texCache[tileImages[tileTexture]] = tex2D;
            }
            else
            {
                diffuseTex = texCache[tileImages[tileTexture]];
            }
        }

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

