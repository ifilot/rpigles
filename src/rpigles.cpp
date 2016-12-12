/*
 * code stolen from openGL-RPi-tutorial-master/encode_OGL/
 * and from OpenGL® ES 2.0 Programming Guide
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "display.h"
#include "shader.h"

// from esUtil.h
#define TRUE 1
#define FALSE 0

typedef struct
{
    // Handle to a program object
    GLuint programObject;
    float angle;
} UserData;

///
// Initialize the shader and program object
//
int Init(UserData *user_data) {
    Shader* shader = new Shader("triangle");

    // Store the program object
    user_data->programObject = shader->get_shader_id();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(UserData *user_data)
{
    GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f};

    GLfloat vColors[] = {1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f};

    // Set the viewport
    glViewport(0, 0, Display::get().get_width(), Display::get().get_height());
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the program object
    glUseProgram(user_data->programObject);

    const glm::mat4 rotmat = glm::rotate(user_data->angle, glm::vec3(0.f, 0.f, 1.0f));
    GLuint loc = glGetUniformLocation(user_data->programObject, "rotmat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &rotmat[0][0]);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColors);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(Display::get().get_display(), Display::get().get_surface());

    user_data->angle += 0.01f;
}

void  esMainLoop (UserData *userdata )
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    float totaltime = 0.0f;
    unsigned int frames = 0;

    gettimeofday ( &t1 , &tz );
    unsigned int count = 0;

    while(count < 10)
    {
        gettimeofday(&t2, &tz);
        deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;

        Draw(userdata);

        totaltime += deltatime;
        frames++;
        if (totaltime >  2.0f)
        {
            printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames/totaltime);
            totaltime -= 2.0f;
            frames = 0;
	    count++;
        }
    }
}

int main(int argc, char *argv[]) {
    Display::get();

    UserData userdata;
    Init(&userdata);
    //esRegisterDrawFunc(p_state, Draw);

    eglSwapBuffers(Display::get().get_display(), Display::get().get_surface());
    esMainLoop(&userdata);
}
