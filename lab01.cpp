//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/ext.hpp>

const char* getVertexShaderSource()
{
    // TODO - Insert Vertex Shaders here ...
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "out vec3 vertexColor;"
        "uniform mat4 model;"
        "void main()"
        "{"
        " vertexColor = aColor;"
        " gl_Position = model* vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}


const char* getFragmentShaderSource()
{
    // TODO - Insert Fragment Shaders here ...
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        " FragColor = vec4(vertexColor.r, vertexColor.g,"
        " vertexColor.b, 1.0f);"
        "}";
}

const char* getWhiteFragmentShaderSource()
{
    // TODO - Insert Fragment Shaders here ...
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        " FragColor = vec4(1.0f, 1.0f,"
        " 1.0f, 1.0f);"
        "}";
}

bool isColoredMode;

int compileAndLinkShaders()
{
    // TODO
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    
    
    //const char* fragmentShaderSource = getFragmentShaderSource();

    const char* fragmentShaderSource;

    if (isColoredMode) {
        // Use the colored fragment shader
        fragmentShaderSource = getFragmentShaderSource();
    }else {
        // Use the white fragment shader
        fragmentShaderSource = getWhiteFragmentShaderSource();
    }

    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);



    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;

    return -1;

}

int createVertexArrayObject()
{
    // TODO
    // Upload geometry to GPU and return the Vertex Buffer Object ID

    glm::vec3 vertexArray[] = {
        glm::vec3(0.0f, 0.5f, 0.0f), // top center position
        glm::vec3(1.0f, 0.0f, 0.0f), // top center color (red)
        glm::vec3(0.5f, -0.5f, 0.0f), // bottom right
        glm::vec3(0.0f, 1.0f, 0.0f), // bottom right color (green)
        glm::vec3(-0.5f, -0.5f, 0.0f), // bottom left
        glm::vec3(0.0f, 0.0f, 1.0f), // bottom left color (blue)
        };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Specify where the data is in the VAO – this allows OpenGL to bind data to vertex shader attributes
    glVertexAttribPointer(0, // attribute 0 matches aPos in Vertex Shader
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(glm::vec3),
        (void*)sizeof(glm::vec3) // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);
    return vertexBufferObject;


    
    return -1;
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.99f/2.55f, 0.21f/2.55f, 0.35f/2.55f, 1.0f);
    
    
    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    
    // Define and upload geometry to the GPU here ...
    GLuint vao = createVertexArrayObject();
    glBindVertexArray(vao);
    bool flipTriangle = false;
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // TODO - draw rainbow triangle

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            isColoredMode = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
            isColoredMode = false;  // Toggle the boolean value
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            flipTriangle = false;  // Toggle the boolean value
        }else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            flipTriangle = true;  // Toggle the boolean value
        }

        glUseProgram(shaderProgram);


        //reverse the triangle
        // Calculate the scaling matrix to flip the object upside down
        glm::mat4 model = glm::mat4(1.0f);
        if (flipTriangle) {
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));  // Flip the triangle
        }


        // Set the model matrix uniform in the shader
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));





        //glBindBuffer(GL_ARRAY_BUFFER, createVertexArrayObject());
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // End frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
  
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}
