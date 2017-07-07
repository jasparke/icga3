#pragma once
#include "icg_common.h"
#include "OpenGP/Eigen/Image.h"


class Box{
protected:
    GLuint _pid; ///< GLSL shader program ID
    GLuint _skybox_tex;

    GLuint _vao;
    GLuint _vbo;

public:
    GLuint getProgramID(){ return _pid; }

    void init(){
        _pid = OpenGP::load_shaders("Skybox/box_vshader.glsl", "Skybox/box_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        check_error_gl();
        float skybox[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
            };

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), skybox, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);


        OpenGP::EigenImage<vec3> image;
        vector<std::string> faces = {"Skybox/right.tga",
                                     "Skybox/left.tga",
                                     "Skybox/bottom.tga",
                                     "Skybox/top.tga",
                                     "Skybox/back.tga",
                                     "Skybox/front.tga"};

        glGenTextures(1, &_skybox_tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_tex);

        for (int i = 0; i < faces.size(); i++) {
            OpenGP::imread(faces[i].c_str(), image);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, image.cols(), image.rows(), 0, GL_RGB, GL_FLOAT, image.data());
            cout << "Loaded cubemap texture: " << faces[i] << endl;
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        check_error_gl();
		cout << "SUCCESS: Loaded skybox shader." << endl;
    }

    void draw(mat4 View, mat4 Projection){
        glDepthFunc(GL_LEQUAL);
        glUseProgram(_pid);

        glUniform1i(glGetUniformLocation(_pid, "skybox"), 0);
        check_error_gl();
        mat4 noTransView = Eigen::Matrix4f::Zero();
        noTransView.topLeftCorner(3, 3) = View.topLeftCorner(3,3);
        noTransView(3,3) = 1.0; // remove the translation from view...
        glUniformMatrix4fv(glGetUniformLocation(_pid, "VIEW"), 1, GL_FALSE, noTransView.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "PROJ"), 1, GL_FALSE, Projection.data());

        glBindVertexArray(_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_tex);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        ///--- Clean up
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glUseProgram(0);
    }
};
