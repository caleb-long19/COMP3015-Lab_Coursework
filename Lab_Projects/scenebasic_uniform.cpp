#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 50, 50) {}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    //initialise the model matrix
    model = mat4(1.0f);

    //enable this group for torus rendering, make sure you comment the teapot group
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f)); //rotate model on x axis
    model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));  //rotate model on y axis
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)); //sets the view - read in the documentation about glm::lookAt. if still have questions,come an dtalk to me

    //enable this group for teapot rendering, make sure you comment the torus group
    //model = glm::translate(model, vec3(0.0, -1.0, 0.0));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    //view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f);

    //make sure you use the correct name, check your vertex shader
    prog.setUniform("Material.Kd", 0.2f, 0.55f, 0.9f); //seting the Kd uniform
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);     //setting the Ld uniform
    prog.setUniform("Light.Position", view * glm::vec4(5.0f, 5.0f, 2.0f, 0.0f)); //setting Light Position

}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();

    torus.render();
}

void SceneBasic_Uniform::setMatrices() 
{
    glm::mat4 mv = view * model;

    prog.setUniform("ModelViewMatrix", mv);

    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);

}
