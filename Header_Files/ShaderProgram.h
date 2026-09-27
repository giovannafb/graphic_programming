#ifndef SHADER_PROGRAM_H
    #define SHADER_PROGRAM_H

#include <string>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <map>

using std::string;

class ShaderProgram{
    public:
        ShaderProgram();
        ~ShaderProgram();

        enum ShaderType{
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

        bool loadShaders(const char* vsfilename, const char* fsfilename);
        void use();

        void setUniform(const GLchar* name, const glm::vec2& v);
        void setUniform(const GLchar* name, const glm::vec3& v);
        void setUniform(const GLchar* name, const glm::vec4& v);

        GLuint getProgram() const;

        private:
            string fileToString(const string filename);
            void checkCompileErrors(GLuint shader, ShaderType(type));
            GLint getUniformLocation(const GLchar* name);

            GLuint mHandle;
            std::map<string, GLint> mUniformLocations;
};

#endif