#include "ConvexHull.h"
#include "GLShader.h"
#include <typeinfo>
#include <fstream>

#define GL_CHECK_ERRORS  assert(glGetError() == GL_NO_ERROR);

void RandomCoordsGen
(std::vector<Point> &randomCoords)
{
    std::srand(std::time(nullptr));
    
    GLfloat xrndm;
    GLfloat yrndm;
    for(long i {0}; i < 100; ++i)
    {
        xrndm = static_cast<GLfloat>((std::rand() / 10000000000) * 1.04);
        if(xrndm == 0)
        {
            xrndm += static_cast<GLfloat>((std::rand() % 90)/100.0);
        }
        yrndm = static_cast<GLfloat>((std::rand() / 10000000000) * 1.04);
        if(yrndm == 0)
        {
            yrndm += static_cast<GLfloat>((std::rand() % 90)/100.0);
        }
        if(i > 50)
        {
            xrndm *= -1.0;
            yrndm *= -1.0;
        }
        Point p {Point(xrndm, yrndm)};
        randomCoords.push_back(p);
    }
}

void FrameBufferSizeCallback
(GLFWwindow* window, 
 int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    
    std::vector<Point> gridCoords;
    // test convex hull is not skewed
    /*
    {Point(-0.75, -0.25), 
     Point(-0.75, 0.4), 
     Point(-0.75, -0.65), 
     Point(-0.65, -0.25), 
     Point(-0.35, -0.8), 
     Point(0.55, 0.85), 
     Point(0.75, -0.25),
     Point(0.75, -0.45), 
     Point(0.75, 0.85)
    };
    */
    RandomCoordsGen(gridCoords);

    ch::ConvexHull convexHull {ch::ConvexHull(gridCoords)};
    std::vector<Point> outerField {convexHull.OuterFieldPoints()};
    //std::set<Point, PointsCompare> hull = convexHull.Hull();
    std::vector<Point> hull = convexHull.Hull_t();
    std::map<std::string, Point> pointsMinMax{convexHull.PolygonField()};
    std::vector<Point> innerField;
    for(const auto &p : pointsMinMax)
    {
        innerField.push_back(p.second);
    }
    std::vector<Point> innerFieldPoints {convexHull.PolygonFieldPoints()};

    if (!glfwInit())
    {
        std::cerr << "GLFW initilisation failed\n";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Convex Hull", NULL, NULL);
    if (!window) 
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: \n" << renderer;
    std::cout << "OpenGL version supported \n" << version;

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    typedef std::vector<Point>::const_reverse_iterator outerIt;
    typedef std::vector<Point>::const_iterator hit;
    typedef std::vector<Point>::const_reverse_iterator innerIt;
    typedef std::vector<Point>::const_iterator inFit;

    // build vertex points
    GLfloat outerPoints[outerField.size() * 3];
    GLfloat *oP = outerPoints;
    for(outerIt it = outerField.rbegin(); it != outerField.rend(); ++it)
    {
        *oP = (GLfloat)it->x;
        *(oP + 1) = (GLfloat)it->y;
        *(oP + 2) = it->z;
        oP += 3;
    }

    GLfloat hullPoints[hull.size() * 3];
    GLfloat *hP = hullPoints;
    for(hit it = hull.begin(); it != hull.end(); ++it)
    {
        *hP = (GLfloat)it->x;
        *(hP + 1) = (GLfloat)it->y;
        *(hP + 2) = it->z;
        hP += 3;
    }

    GLfloat innerPoints [(innerFieldPoints.size() + innerField.size())* 3];
    GLfloat *iP = innerPoints;
    for(innerIt it = innerField.rbegin(); it != innerField.rend(); ++it)
    {
        *iP = (GLfloat)it->x;
        *(iP + 1) = (GLfloat)it->y;
        *(iP + 2) = it->z;
        iP += 3;
    }
      
    for(inFit it = innerFieldPoints.begin(); it != innerFieldPoints.end(); ++it)
    {
        *iP = (GLfloat)it->x;
        *(iP + 1) = (GLfloat)it->y;
        *(iP + 2) = it->z;
        iP += 3;
    }

    GLfloat colors[5][4] = 
    {
        {1.0, 0.0, 0.0, 1.0}, // red
        {0.0, 0.5, 0.0, 1.0}, // green
        {0.0, 0.0, 1.0, 1.0}, // blue
        {1.0, 1.0, 1.0, 1.0}, // white
        {0.0, 0.0, 0.0, 1.0}  // black
    };

    // shaders
    auto shader = std::make_unique<GLShader>();
    shader->LoadFromFile(GL_VERTEX_SHADER, "shaders/vertex.shader");
    shader->LoadFromFile(GL_FRAGMENT_SHADER, "shaders/fragment.shader");
    shader->CreateProgram();
    shader->AttachShaders();
    shader->LinkProgram();
    GL_CHECK_ERRORS;

    // VAOs and VBOs
    GLuint vao[3];
    glGenVertexArrays(3, vao);
    const GLuint *vaoOF = &vao[0];
    const GLuint *vaoIF = &vao[1];
    const GLuint *vaoHull = &vao[2];

    glBindVertexArray(*vaoOF);
    GL_CHECK_ERRORS;

    GLuint vboOF {0};
    glGenBuffers(1, &vboOF);
    glBindBuffer(GL_ARRAY_BUFFER, vboOF);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(outerPoints), 
                 outerPoints, 
                 GL_STATIC_DRAW);
    GL_CHECK_ERRORS;

    GLint ofPos {glGetAttribLocation(shader->ReturnShaderProgram(), 
                                       "vertexPosition")};
    GLint ofCol {glGetUniformLocation(shader->ReturnShaderProgram(), 
                                       "vertexColor")};
    glVertexAttribPointer(ofPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ofPos);
    GL_CHECK_ERRORS;

    glBindVertexArray(0);

    //second vao
    glBindVertexArray(*vaoHull);
    GL_CHECK_ERRORS;

    GLuint vboHull {0};
    glGenBuffers(1, &vboHull);
    glBindBuffer(GL_ARRAY_BUFFER, vboHull);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(hullPoints), 
                 hullPoints, 
                 GL_STATIC_DRAW);
    GL_CHECK_ERRORS;

    GLint hullPos {glGetAttribLocation(shader->ReturnShaderProgram(), 
                                       "vertexPosition")};
    GLint hullCol {glGetUniformLocation(shader->ReturnShaderProgram(), 
                                       "vertexColor")};
    glVertexAttribPointer(hullPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(hullPos);
    GL_CHECK_ERRORS;

    glBindVertexArray(0);

    //third vao
    glBindVertexArray(*vaoIF);
    GLuint vboIF {0};
    GL_CHECK_ERRORS;

    glGenBuffers(1, &vboIF);
    glBindBuffer(GL_ARRAY_BUFFER, vboIF);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(innerPoints), 
                 innerPoints, 
                 GL_STATIC_DRAW);
    GL_CHECK_ERRORS;

    GLint ifPos {glGetAttribLocation(shader->ReturnShaderProgram(), 
                                       "vertexPosition")};
    GLint ifCol {glGetUniformLocation(shader->ReturnShaderProgram(), 
                                        "vertexColor")};
    glVertexAttribPointer(ifPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ifPos);
    GL_CHECK_ERRORS;

    GLuint hullIndicesSize = (hull.size() * 2) - 1;
    GLuint hullIndices[hullIndicesSize];
    hullIndices[0] = 0;
    GLuint hullInsert = 1;
    for(GLuint hpCount {1}; hpCount < hullIndicesSize; ++hpCount)
    {
        hullIndices[hullInsert] = hpCount; 
        hullIndices[hullInsert+1] = hpCount; 
        hullInsert += 2;
    }

    while(!glfwWindowShouldClose(window)) 
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

        glClearColor(0.9, 0.9, 0.9, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	    glEnable(GL_PROGRAM_POINT_SIZE);
	    glPointSize(3.0);
	    glLineWidth(2.0);

        shader->Use();
        glBindVertexArray(*vaoOF);
        glUniform4fv(ofCol, 1, colors[2]);
        glDrawArrays(GL_POINTS, 0, outerField.size() - 1);

        glBindVertexArray(0);

        glBindVertexArray(*vaoHull);
        glUniform4fv(hullCol, 1, colors[0]);
        glDrawElements(GL_LINES, hullIndicesSize-1, GL_UNSIGNED_INT, hullIndices);
        
        glBindVertexArray(0);

        glBindVertexArray(*vaoIF);
        glUniform4fv(ifCol, 1, colors[1]);
        const GLuint indices[8] = {0, 1, 1, 2, 2, 3, 3, 0};
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, indices);
        glUniform4fv(ifCol, 1, colors[0]);
        glDrawArrays(GL_POINTS, 0, innerFieldPoints.size()-1);

        // update other events like input handling 
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
