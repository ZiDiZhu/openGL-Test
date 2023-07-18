#include <iostream>
#include <list>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <random>
using namespace glm;
using namespace std;
const char* getVertexShaderSource();
const char* getFragmentShaderSource();
int compileAndLinkShaders();
int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize);

//Vertex Arrays
vec3 cube2Array[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f)
};
vec3 lineArray[] = { vec3(-0,0,0),vec3(1.0f),
            vec3(0,0,0),vec3(1.0f),
            vec3(0,0,0),vec3(1.0f),
            vec3(0,0,0),vec3(1.0f)};
vec3 gizmoArray[] = {
        vec3(0,0,0),vec3(1,0,0),
        vec3(1,0,0),vec3(1,0,0),
        vec3(0,0,0),vec3(0,1,0),
        vec3(0,1,0),vec3(0,1,0),
        vec3(0,0,0),vec3(0,0,1),
        vec3(0,0,1),vec3(0,0,1)
};

int lastX = 0;

//camera
float cameraSpeed = 1.0f;
vec3 cameraPosition(0.0f, 1.0f, 4.0f);
vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
float fov = 70.0f;
//end camera

//object
vec3 v3_objScale(1.0f);
vec3 v3_objTranslate(0, 0, 0);
float objMoveSpeed = 0.005f;
float objScaleSpeed = 0.005f;
float objectAngleY = 0, objectAngleX = 0;
mat4 m4_cubeRotY, m4_cubeRotX, m4_objRot;
//end object

//world
mat4 objMatrix;
mat4 m4_worldRotX,m4_worldRotY,m4_worldRot;
float worldAngleY = 0, worldAngleX = 0;
float rotationSpeed = 150.0f;  // degrees per second
//end world

//view-projection
mat4 viewMatrix;
//end of view-projection

//cursor
double mousePosX, mousePosY;
double lastMousePosX, lastMousePosY;
double dx = 0, dy = 0;
float lastFrameTime;
//end cursor

//shader
GLenum renderMode;
GLuint worldMatrixLocation,projectionMatrixLocation, viewMatrixLocation, colorLocation;
//end shader

bool gridOn = false;

class Cube {
public:
    Cube(vec3 pos, vec3 dime,float z,int sp) {
        mWorldMatrixLocation = glGetUniformLocation(sp, "worldMatrix");
        position = pos;
        dimensions = dime;
        rotZ = z;
    }
    void Draw() {
        mat4 worldMatrix = objMatrix* translate(mat4(1.0f),position)*rotate(mat4(1.0f),radians(rotZ),vec3(0.0f,0.0f,1.0f)) *scale(mat4(1.0f), dimensions);
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(renderMode, 0, 36);
    }
public:
    vec3 position;
    vec3 dimensions;
    float rotZ;
    GLuint mWorldMatrixLocation;
    //mat4 m4transform;
};

//TODO: fix this by using transform matrix to offset 1 constant lineAO instead of making an AO everytime
void drawGrid2(GLfloat size, GLfloat step) {
    glLineWidth(1);
    for (GLfloat i = -size; i <= size; i += step) {
        
        lineArray[0] = vec3( - size, 0, i);
        lineArray[2] = vec3(size, 0, i);
        lineArray[4] = vec3(i, 0, -size);
        lineArray[6] = vec3(i, 0, size);

        GLuint lineAO = createVertexArrayObject(lineArray, sizeof(lineArray));
        glBindVertexArray(lineAO);
        glDrawArrays(GL_LINES, 0, 4);
    }
    

    //TODO: Draw axis
    glLineWidth(3);

    int gizmoAO = createVertexArrayObject(gizmoArray, sizeof(gizmoArray));
    glBindVertexArray(gizmoAO);
    glDrawArrays(GL_LINES, 0,6);
}

void drawGrid1(GLfloat size, GLfloat step, int gizmoAO) {
    //glTranslatef(-size / 2.0, -size / 2.0, 0.0);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (GLfloat i = -size; i <= size; i += step) {
        glVertex3f(i, 0, -size);
        glVertex3f(i, 0, size);

        glVertex3f(-size, 0, i);
        glVertex3f(size, 0, i);
    }
    glEnd();

    glLineWidth(5);
    glBindVertexArray(gizmoAO);
    glDrawArrays(GL_LINES, 0, 6);
}

void updateCursor(GLFWwindow* window) {
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    dx = mousePosX - lastMousePosX;
    dy = mousePosY - lastMousePosY;
    lastMousePosX = mousePosX;
    lastMousePosY = mousePosY;
}

int main() {

    //init
    if (!glfwInit()) {
        return -1;
    }
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL Grid", NULL, NULL);
    if (!window) {
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
    //end of init


    int shaderProgram = compileAndLinkShaders();
    glUseProgram(shaderProgram);


    mat4 projectionMatrix = perspective(fov,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    colorLocation = glGetUniformLocation(shaderProgram, "myColor");
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");


    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    int cubeAO = createVertexArrayObject(cube2Array,sizeof(cube2Array));

    
    lastFrameTime = (float)glfwGetTime();

    
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    glPointSize(5);

    //modes
    renderMode = GL_TRIANGLES;

    double dt; //delta time
    glClearColor(1.0f, 0.7f, 0.7f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);

    //grid
    int gizmoAO = createVertexArrayObject(gizmoArray, sizeof(gizmoArray));

    while (!glfwWindowShouldClose(window)) {
        
        dt = (float)glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(cubeAO);


        m4_worldRot = rotate(mat4(1.0f), radians(worldAngleY), vec3(0.0f, 1.0f, 0.0f));
        m4_worldRot *= rotate(mat4(1.0f), radians(worldAngleX), vec3(1.0f, 0.0f, 0.0f));

        /*worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");*/
        
        //object - scale rotate transform
        objMatrix = scale(mat4(1), v3_objScale);
        objMatrix *= rotate(mat4(1.0f), radians(objectAngleY), vec3(0.0f, 1.0f, 0.0f));
        objMatrix *= rotate(mat4(1.0f), radians(objectAngleX), vec3(1.0f, 0.0f, 0.0f));
        objMatrix *= translate(mat4(1.0), v3_objTranslate); 


        //draw the model
        Cube c1 = Cube(vec3(0, -0.2, 0), vec3(0.8, 0.15, 0.15), 45.0f,shaderProgram);//arm
        Cube c2 = Cube(vec3(0.2,0.3,0), vec3(0.14, 0.6, 0.14),15.0f,shaderProgram);//forearm

        Cube handle = Cube(vec3(0, 1, 0), vec3(0.1, 1, 0.1), 10, shaderProgram);

        //frame
        Cube c3 = Cube(vec3(-0.28, 1.5, 0), vec3(0.5, 0.08, 0.08), -10.0f, shaderProgram);
        Cube c4 = Cube(vec3(0.08, 1.52, 0), vec3(0.5, 0.08, 0.08), -160.0f, shaderProgram);
        Cube c5 = Cube(vec3(-0.45, 1.95, 0), vec3(0.1, 0.9, 0.1), 3.0f, shaderProgram);
        Cube c6 = Cube(vec3(0.22, 2, 0), vec3(0.1, 0.8, 0.1), 1.0f, shaderProgram);
        Cube c7 = Cube(vec3(-0.28, 2.5, 0), vec3(0.5, 0.08, 0.08), 10.0f, shaderProgram);
        Cube c8 = Cube(vec3(0.08, 2.5, 0), vec3(0.5, 0.08, 0.08), 160.0f, shaderProgram);


        for (int i = 0; i < 10; i++) {
            Cube cv = Cube(vec3(0.07*i-0.36, 2, 0), vec3(0.01, 0.9, 0.01), 0, shaderProgram);
            cv.Draw();
            Cube ch = Cube(vec3(-0.1, 2.4-0.08*i, 0), vec3(0.7, 0.01, 0.01), 0, shaderProgram);
            ch.Draw();
        }

        c1.Draw();
        c2.Draw();
        handle.Draw();
        c3.Draw();
        c4.Draw();
        c5.Draw();
        c6.Draw();
        c7.Draw();
        c8.Draw();

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &m4_worldRot[0][0]);

        //if(gridOn)
        drawGrid1(10.0,1.0,gizmoAO);


        glfwSwapBuffers(window);
        glfwPollEvents();


        updateCursor(window);


        //input handling

        
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            v3_objScale += vec3(0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            v3_objScale -= vec3(0.02f);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {

            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> dis(0.0f, 1.0f);

            v3_objTranslate = vec3(dis(gen), dis(gen), dis(gen));
            continue;
        }


        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            worldAngleY = (worldAngleY - rotationSpeed * dt); 
            objectAngleY = (objectAngleY - rotationSpeed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            worldAngleY = (worldAngleY + rotationSpeed * dt);
            objectAngleY = (objectAngleY + rotationSpeed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            worldAngleX= (worldAngleX + rotationSpeed * dt);
            objectAngleX = (objectAngleX + rotationSpeed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            worldAngleX = (worldAngleX - rotationSpeed * dt);
            objectAngleX = (objectAngleX - rotationSpeed * dt);
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            renderMode = GL_POINTS;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            renderMode = GL_LINE_LOOP;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            renderMode = GL_TRIANGLES;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            //rotate
            objectAngleY = (objectAngleY + rotationSpeed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            objectAngleY = (objectAngleY - rotationSpeed * dt);
        }

        //panning left rght
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            cameraPosition -= vec3(0.02f*dx, 0, 0);
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            cameraPosition += vec3(0,0.02f*dy,0);
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            fov += 0.02f * dy;
        }


        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            v3_objTranslate -= vec3(objMoveSpeed,0,0);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        {
            v3_objTranslate += vec3(objMoveSpeed, 0, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        {
            v3_objTranslate -= vec3(0, objMoveSpeed, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        {
            
            v3_objTranslate += vec3(0, objMoveSpeed, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) 
        {
            gridOn = true;
        }


        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));

        projectionMatrix = perspective(fov,            // field of view in degrees
            800.0f / 600.0f,  // aspect ratio
            0.01f, 100.0f);   // near and far (near > 0)
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            objectAngleX = 0;
            objectAngleY = 0;
            v3_objScale = vec3(1.0f);
            v3_objTranslate = vec3(0);
            worldAngleY = 0;
            worldAngleX = 0;
            cameraPosition = vec3(0.0f, 1.0f, 5.0f);
            cameraUp = vec3(0.0f, 1.0f, 0.0f);
            cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
        }
        
    }

    glfwTerminate();
    return 0;
}


//vertex shader code
const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform vec3 myColor;"
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

//return shader program id
int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
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
}

int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(glm::vec3),
        (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}