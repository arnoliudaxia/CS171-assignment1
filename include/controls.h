#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getMVPCM(glm::mat4 modelTransformPos = glm::mat4(1));
glm::mat4 getMVP();
glm::vec3 getCameraPosition();
glm::vec3 getCameraFront();

#endif