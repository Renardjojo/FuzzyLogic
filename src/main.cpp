

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "AI/FuzzyLogic/FuzzyLogic.hpp"

#include "implot.h"


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace std;
using namespace AI::FuzzyLogic;
using namespace AI::FuzzyLogic::FuzzySet;

static void PushStyleCompact()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.70f)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.70f)));
}

static void PopStyleCompact()
{
    ImGui::PopStyleVar(2);
}

void writeLine(string msg, bool stars = false)
{
    if (stars)
    {
        msg = "*** " + msg + " ";
        while (msg.size() < 45)
        {
            msg += "*";
        }
    }
    std::cout << msg << std::endl;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //FuzzyLogic
    // Creation du systeme
    writeLine("Gestion du zoom GPS", true);
    FuzzySystem system("Gestion du zoom GPS");

    writeLine("1) Ajout des variables", true);

    // Ajout de la variable linguistique "Distance" (de 0 a 500 000 m)
    writeLine("Ajout de la variable Distance");
    LinguisticVariable distance("Distance", 0.f, 500000.f, "Km");
    distance.addValue(LinguisticValue("Faible", LeftFuzzySet(0.f, 500000.f, 30.f, 50.f)));
    distance.addValue(LinguisticValue("Moyenne", TrapezoidalFuzzySet(0.f, 500000.f, 40.f, 50.f, 100.f, 150.f)));
    distance.addValue(LinguisticValue("Grande", RightFuzzySet(0.f, 500000.f, 100.f, 150.f)));
    system.addInputVariable(distance);

    // Ajout de la variable linguistique "Vitesse" (de 0 a 200)
    writeLine("Ajout de la variable Vitesse");
    LinguisticVariable vitesse("Vitesse", 0.f, 200.f, "Km/h");
    vitesse.addValue(LinguisticValue("Lente", LeftFuzzySet(0.f, 200.f, 20.f, 30.f)));
    vitesse.addValue(LinguisticValue("PeuRapide", TrapezoidalFuzzySet(0.f, 200.f, 20.f, 30.f, 70.f, 80.f)));
    vitesse.addValue(LinguisticValue("Rapide", TrapezoidalFuzzySet(0.f, 200.f, 70.f, 80.f, 90.f, 110.f)));
    vitesse.addValue(LinguisticValue("TresRapide", RightFuzzySet(0.f, 200.f, 90.f, 110.f)));
    system.addInputVariable(vitesse);

    // Ajout de la variable linguistique "Zoom" (de 0 a 5)
    writeLine("Ajout de la variable Zoom");
    LinguisticVariable zoom("Zoom", 0.f, 5.f, "Level");
    zoom.addValue(LinguisticValue("Petit", LeftFuzzySet(0.f, 5.f, 1.f, 2.f)));
    zoom.addValue(LinguisticValue("Normal", TrapezoidalFuzzySet(0.f, 5.f, 1.f, 2.f, 3.f, 4.f)));
    zoom.addValue(LinguisticValue("Gros", RightFuzzySet(0.f, 5.f, 3.f, 4.f)));
    system.setOutput(zoom);

    writeLine("2) Ajout des regles", true);

    // Creation des regles selon la matrice suivante :
    // Plus le zoom est petit, plus on voit loin (mais moins detaille)
    // V \ D  || F | M | G |
    // Lent   || N | P | P |
    // Peu Ra || N | N | P |
    // Rapide || G | N | P |
    // Tres R || G | G | P |
    system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS Lente THEN Zoom IS Normal");
    system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS Lente THEN Zoom IS Petit");
    system.addFuzzyRule("IF Distance IS Grande AND Vitesse IS Lente THEN Zoom IS Petit");
    system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS PeuRapide THEN Zoom IS Normal");
    system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS PeuRapide THEN Zoom IS Normal");
    system.addFuzzyRule("IF Distance IS Grande AND Vitesse IS PeuRapide THEN Zoom IS Petit");
    system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS Rapide THEN Zoom IS Gros");
    system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS Rapide THEN Zoom IS Normal");
    system.addFuzzyRule("IF Distance IS Grande AND Vitesse IS Rapide THEN Zoom IS Petit");
    system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS TresRapide THEN Zoom IS Gros");
    system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS TresRapide THEN Zoom IS Gros");
    system.addFuzzyRule("IF Distance IS Grande AND Vitesse IS TresRapide THEN Zoom IS Petit");
    writeLine("12 regles ajoutees \n");
    
    writeLine("3) Resolution de cas pratiques", true);
    // Cas pratique 1 : vitesse de 35 kms/h, et prochain changement de direction a 70m
    writeLine("Cas 1 :", true);
    writeLine("V = 35 (peu rapide)");
    writeLine("D = 70 (moyenne)");
    system.setInputVariable(vitesse, 35.f);
    system.setInputVariable(distance, 70.f);
    writeLine("Attendu : zoom normal, centroide a 2.5");
    writeLine("Resultat : " + std::to_string(system.solve()) + "\n");
    
    // Cas pratique 2 : vitesse de 25 kms/h, et prochain changement de direction a 70m
    system.resetCase();
    writeLine("Cas 2 :", true);
    writeLine("V = 25 (50% lente, 50% peu rapide)");
    writeLine("D = 70 (moyenne)");
    system.setInputVariable(vitesse, 25.f);
    system.setInputVariable(distance, 70.f);
    writeLine("Attendu : zoom normal a 50% + zoom petit a 50%");
    writeLine("Resultat : " + std::to_string(system.solve()) + "\n");

    // Cas pratique 3 : vitesse de 72.5 kms/h, et prochain changement de direction a 40m
    system.resetCase();
    writeLine("Cas 3 :", true);
    writeLine("V = 72.5 (75% peu rapide + 25% rapide)");
    writeLine("D = 40 (50% faible)");
    system.setInputVariable(vitesse, 72.5f);
    system.setInputVariable(distance, 40.f);
    writeLine("Attendu : zoom normal a 50% + zoom gros a 25%");
    writeLine("Resultat : " + std::to_string(system.solve()) + "\n");

    // Cas pratique 4 : vitesse de 100 kms/h, et prochain changement de direction a 110m
    system.resetCase();
    writeLine("Cas 4 :", true);
    writeLine("V = 100 (50% rapide + 50% tres rapide)");
    writeLine("D = 110 (80% moyenne, 20% grande)");
    system.setInputVariable(vitesse, 100.f);
    system.setInputVariable(distance, 110.f);
    writeLine("Attendu : zoom petit a 20% + zoom normal a 50% + zoom gros a 50%");
    writeLine("Resultat : " + std::to_string(system.solve()) + "\n");

    // Cas pratique 5 : vitesse de 45 kms/h, et prochain changement de direction a 160m
    system.resetCase();
    writeLine("Cas 5 :", true);
    writeLine("V = 45 (100% peu rapide)");
    writeLine("D = 160 (100% grande)");
    system.setInputVariable(vitesse, 45.f);
    system.setInputVariable(distance, 160.f);
    writeLine("Attendu : zoom petit a 100%");
    writeLine("Resultat : " + std::to_string(system.solve()) + "\n");


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
        {
            ImPlot::ShowDemoWindow(&show_demo_window);
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::Text("Input");
        for (auto&& input : system.getInputs())
        {                
            if (ImGui::CollapsingHeader(input.getName().c_str()))
            {
                static bool show_lines = true;
                static bool show_fills = true;
                static float fill_ref = 0;
                ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
                ImGui::Checkbox("Fills", &show_fills);

                ImPlot::SetNextPlotLimits(input.getMin(), input.getMax(), 0, 1);
                if (ImPlot::BeginPlot(input.getName().c_str(), input.getUnit().c_str(), "Degree of belonging"))
                {
                    for (auto&& value : input.getValues())
                    {
                        std::vector<float> pointX;
                        std::vector<float> pointY;

                        for (auto&& point : value.getFuzzySet().getPoints())
                        {
                            pointX.emplace_back(point.getX());
                            pointY.emplace_back(point.getY());
                        }

                        if (show_fills)
                        {
                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

                             ImPlot::PlotShaded(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());

                            ImPlot::PopStyleVar();
                        }
                        if (show_lines)
                        {
                            ImPlot::PlotLine(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());
                        }
                    }

                    ImPlot::EndPlot();
                }
            }
        }

        ImGui::Text("Output");
        if (ImGui::CollapsingHeader(system.getOutput().getName().c_str()))
        {
            static bool show_lines = true;
            static bool show_fills = true;
            static float fill_ref = 0;
            ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
            ImGui::Checkbox("Fills", &show_fills);

            ImPlot::SetNextPlotLimits(system.getOutput().getMin(), system.getOutput().getMax(), 0, 1);
            if (ImPlot::BeginPlot(system.getOutput().getName().c_str(), system.getOutput().getUnit().c_str(), "Degree of belonging"))
            {
                for (auto&& value : system.getOutput().getValues())
                {
                    std::vector<float> pointX;
                    std::vector<float> pointY;

                    for (auto&& point : value.getFuzzySet().getPoints())
                    {
                        pointX.emplace_back(point.getX());
                        pointY.emplace_back(point.getY());
                    }

                    if (show_fills)
                    {
                        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

                        ImPlot::PlotShaded(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());

                        ImPlot::PopStyleVar();
                    }
                    if (show_lines)
                    {
                        ImPlot::PlotLine(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());
                    }
                }

                ImPlot::EndPlot();
            }
        }
        
    if (ImGui::TreeNode("Borders, background"))
    {
        if (ImGui::BeginTable("##table1", system.getInputs()[0].getValues().size() + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            //Columns
            ImGui::TableSetupColumn((system.getInputs()[0].getName() + "/" + system.getInputs()[1].getName()).c_str());

            for (auto&& input : system.getInputs()[0].getValues())
            {
                float font_size = ImGui::GetFontSize() * input.getName().size() / 2;
                ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
                ImGui::TableSetupColumn(input.getName().c_str());
            }

            //Row
            ImGui::TableHeadersRow();

            for (int row = 0; row < system.getInputs()[1].getValues().size(); ++row)
            {
                ImGui::TableNextRow();
                
                ImGui::TableSetColumnIndex(0);

                ImGui::Text(system.getInputs()[1].getValues()[row].getName().c_str());

                int idElem = 0;

                for (int column = 1; column < system.getInputs()[0].getValues().size() + 1; ++column)
                {
                    ImGui::TableSetColumnIndex(column);
                    
                    idElem = row * system.getInputs()[0].getValues().size() + (column - 1);
                    if (ImGui::Button((system.getRules()[idElem].getConclusion().getName() + "##" + std::to_string(idElem)).c_str(), ImVec2(-FLT_MIN, 0.0f)))
                    { 
                        int currentIndex = 0;
                        while (system.getOutput().getValues()[currentIndex].getName() != system.getRules()[idElem].getConclusion().getName().c_str())
                        {
                            ++currentIndex;
                        }

                        system.getRules()[idElem].setConclusion(FuzzyExpression(system.getOutput(), system.getOutput().getValues()[++currentIndex % system.getOutput().getValues().size()].getName()));
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
