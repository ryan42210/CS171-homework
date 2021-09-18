#include "SurfaceRender.hpp"

SurfaceRender::SurfaceRender(const MeshSur& surface) {
    loadSurface(surface);
}

void SurfaceRender::loadEdgeSurface(const MeshSur& mesh) {
    //Load mesh surface bind VAO
    //finish your code here.
    // load rows
    verticeNum = mesh.getVertices().size();
    edgeNum = mesh.getEdgeIndices().size();
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO_e);

    glGenVertexArrays(1, &VAO_e);

    glBindVertexArray(VAO_e);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mesh.getVertices().size(), mesh.getVertices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_e);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getEdgeIndices().size(), mesh.getEdgeIndices().data(), GL_STATIC_DRAW);

	// vertex attribute: position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);
	// vertex attribute: normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load cols
}
void SurfaceRender::loadSurface(const MeshSur& mesh) {
    //Load mesh surface bind VAO
    //finish your code here.
    verticeNum = mesh.getVertices().size();
    faceNum = mesh.getFaceIndices().size();

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO_f);

    glGenVertexArrays(1, &VAO_f);

    glBindVertexArray(VAO_f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mesh.getVertices().size(), mesh.getVertices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_f);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getFaceIndices().size(), mesh.getFaceIndices().data(), GL_STATIC_DRAW);

	// vertex attribute: position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);
	// vertex attribute: normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SurfaceRender::draw(const Shader& shader, unsigned int texture)
{
    shader.use();
    //shader settings.
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO_f);
    glDrawElements(GL_TRIANGLES, faceNum, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SurfaceRender::drawEdge(const Shader& shader,  unsigned int texture)
{
    shader.use();
    //shader settings.
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO_e);
    glDrawElements(GL_LINES, edgeNum, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}