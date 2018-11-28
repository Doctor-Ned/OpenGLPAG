#include "MeshCylinderGeometry.h"

MeshCylinderGeometry::MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount, char *texturePath, glm::vec3 baseCenter)
	: baseCenter(baseCenter), radius(radius), height(height), shader(shader), MeshTexture(shader) {
	texture = createTexture(texturePath);
	setupMesh();
}

void MeshCylinderGeometry::draw(glm::mat4 world, float scale) { draw(shader, world, scale); }

void MeshCylinderGeometry::draw(GeometryShader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setHeight(height);
	shader.setScale(scale);
	shader.setModel(world);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glBindVertexArray(VAO);
	glBindVertexBuffer(0, VBO, 0, sizeof(glm::vec3));
	glDrawArrays(GL_TRIANGLES, 0, vertexAmount);
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshCylinderGeometry::draw(Shader shader, glm::mat4 world, float scale) {
	GeometryShader *geo = dynamic_cast<GeometryShader*>(&shader);
	if (geo != nullptr) {
		draw(*geo, world, scale);
	} else {
		printf("You tried to draw a MeshCylinderGeometry with a shader that is not a GeometryShader. That should not happen. Ever.\n");
		exit(1);
	}
}

void MeshCylinderGeometry::updateValues(float radius, float height, int sideAmount) {
	if (radius <= 0) {
		radius = 0.01f;
	}
	this->radius = radius;
	if (height <= 0) {
		height = 0.01f;
	}
	this->height = height;
	if (sideAmount < 3) {
		sideAmount = 3;
	}
	this->sideAmount = sideAmount;

	bufferData();
}

void MeshCylinderGeometry::drawGui(bool autoUpdate) {
	static float _radius = radius;
	static float _height = height;
	static int _sideAmount = sideAmount;
	ImGui::SliderFloat("GeoCylinder radius", &_radius, 0.01f, 2.0f);
	ImGui::NewLine();
	ImGui::SliderFloat("GeoCylinder height", &_height, 0.01f, 2.0f);
	ImGui::NewLine();
	ImGui::SliderInt("GeoCylinder sides", &_sideAmount, 3, 50);
	ImGui::NewLine();

	if (autoUpdate || ImGui::Button("Apply geocylinder changes")) {
		if (_radius != radius || _height != height || _sideAmount != sideAmount) {
			radius = _radius;
			height = _height;
			sideAmount = _sideAmount;
			updateValues(radius, height, sideAmount);
		}
	}
}

void MeshCylinderGeometry::createBottomTriangle(std::vector<glm::vec3>* data, float angle1, float angle2) {
	glm::vec3 closer = baseCenter, farther = baseCenter;

	closer.x += cos(angle1) * radius;
	closer.z += sin(angle1) * radius;

	farther.x += cos(angle2) * radius;
	farther.z += sin(angle2) * radius;

	data->push_back(baseCenter);
	data->push_back(closer);
	data->push_back(farther);
}

void MeshCylinderGeometry::bufferData() {
	glBindVertexArray(VAO);
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
	}
	glGenBuffers(1, &VBO);

	std::vector<glm::vec3> data;
	float radStep = 2 * M_PI / sideAmount;
	float angle = 0.0f;
	for (int i = 0; i < sideAmount; i++) {
		createBottomTriangle(&data, angle, i == sideAmount - 1 ? 0.0f : angle + radStep);
		angle += radStep;
	}

	vertexAmount = data.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexAmount, &data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshCylinderGeometry::setupMesh() {
	shader.use();
	VBO = 0;
	glGenVertexArrays(1, &VAO);
	updateValues(radius, height, sideAmount);
}
