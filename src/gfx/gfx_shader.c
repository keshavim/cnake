#include "gfx.h"

static char *_readFile(char * filepath){
    FILE *file = fopen(filepath, "rb");
    if(!file){
        perror("gfx_shader");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    u64 filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    //*reads the full file
    char *fullstr = calloc(filesize, 1);
    u32 readsize = 0;
    while(readsize != filesize){
        readsize += fread(fullstr, 1, filesize, file);
    }
    fclose(file);

    return fullstr;
}

static void _compile(Shader *self, char *fullstr){
    // //*gets the vertex sources
    u32 vsize = strstr(fullstr, "//VERTEX_END") - fullstr;
    char *vertex_source = calloc(vsize, 1);
    memcpy(vertex_source, fullstr, vsize);
    vsize += strlen("//VERTEX_END");

    //*compile and attach vertex source
    u32 vs_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_handle, 1,  (const GLchar *const *)&vertex_source, (const GLint*)&vsize);
    glCompileShader(vs_handle);
    glAttachShader(self->program, vs_handle);

    //*gets fragment source
    u32 fsize = strstr(fullstr, "//FRAGMENT_END") - fullstr - vsize;
    char *fragment_source = calloc(fsize, 1);
    memcpy(fragment_source, fullstr + vsize, fsize);

    //*compiles and attaches fragment source
    u32 fs_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fs_handle, 1,  (const GLchar *const *)&fragment_source, (const GLint*)&fsize);
    glCompileShader(fs_handle);
    glAttachShader(self->program, fs_handle);

    free(vertex_source);
    free(fragment_source);
}

Shader shader_create(char *filepath)
{
    Shader self;
    self.program = glCreateProgram();
    self.filepath = filepath;
    
    //*open file
    char *fullstr = _readFile(filepath);

    _compile(&self, fullstr);

    glLinkProgram(self.program);

    free(fullstr);
    
    return self;
}

void shader_bind(Shader *self){
    glUseProgram(self->program);
}
void shader_unbind(Shader *self){
    glUseProgram(0);
}
void shader_delete(Shader *self){
    
    glDeleteProgram(self->program);
}